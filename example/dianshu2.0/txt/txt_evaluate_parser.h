#include "user_type.h"
#include "ypc/core_t/analyzer/data_source.h"
#include "ypc/stbox/ebyte.h"
#include "ypc/stbox/stx_common.h"
#include "ypc/stbox/tsgx/log.h"

#include "ypc/corecommon/to_type.h"
#include <hpda/extractor/raw_data.h>
#include <hpda/output/memory_output.h>
#include <hpda/processor/query/filter.h>

define_nt(input_param, std::string);
typedef ff::net::ntpackage<0, input_param> nt_package_t;

class txt_evaluate_parser {
public:
  txt_evaluate_parser(
      std::vector<std::shared_ptr<ypc::data_source_with_dhash>> &source)
      : m_datasources(source) {}

  inline stbox::bytes do_parse(const stbox::bytes &param) {
    ypc::to_type<stbox::bytes, txt_batch_item_t> converter(
        m_datasources[0].get());

    const int batch_max_size = 32768;
    int counter = 0;
    int total_size = 0; // 总长度
    int end_size; // 最后一个batch的长度

    hpda::processor::internal::filter_impl<txt_batch_item_t> match(
        &converter, [&](const txt_batch_item_t &v) {
          counter++;
          std::string batch = v.get<::txt_batch>();
          total_size += batch.length(); // 每个batch的长度累加
          end_size = batch.length(); // 最后一个batch的长度会在这里覆盖上去
          return true;
        });
    
    hpda::output::internal::memory_output_impl<txt_batch_item_t> mo(&match);
    mo.get_engine()->run();
    
    LOG(INFO) << "Total Size: " << total_size;
    LOG(INFO) << "End Size: " << end_size;

    int batch_num = total_size / batch_max_size; // 一共有多少个batch
    bool need_next_batch = false; // 100个字节是否需要下一个batch
    int extral_size = 0; // 下一个batch的所需长度
    int start_pos = total_size / 2 - 50; // 中间的100字节的起始位置
    int mid_batch_no = start_pos / batch_max_size; // 中间的100字节所在的batch的编号
    
    start_pos = start_pos % batch_max_size; // 中间的100字节的起始位置在batch中的位置

    LOG(INFO) << "Batch Num: " << batch_num;
    LOG(INFO) << "Start Position: " << start_pos;
    LOG(INFO) << "Mid Batch No: " << mid_batch_no;

    if(start_pos > (batch_max_size - 100)){
      need_next_batch = true;
      extral_size = start_pos - (batch_max_size - 100);
    }

    stbox::bytes result;
    std::string first_100;
    std::string middle_100;
    std::string last_100;

    int tempNO = 0;
    // TODO  对于‘/’符号怎么处理 -- json问题
    
    LOG(INFO) << "Middle Start Position: " << total_size;
    for (auto it : mo.values()) {
        std::string batch = it.get<txt_batch>();
        size_t batch_length = batch.length();

        if(total_size < 100) {
          first_100 = batch;
          middle_100 = batch;
          last_100 = batch;
          break;
        }

        if(tempNO == 0) { 
          first_100 = batch.substr(0, 100);
        }
        
        if(need_next_batch) {
          if(tempNO == mid_batch_no) {
            middle_100 = batch.substr(batch.length(), 100 - extral_size);
          }
          if(tempNO == mid_batch_no+1) {
            middle_100 += batch.substr(0, extral_size);
          }
        } else if(tempNO == mid_batch_no) {
            middle_100 = batch.substr(start_pos, 100);
          }

        
        if(tempNO==counter-1) {
          last_100 = batch.substr(batch.length() - 100);
        }
        
        tempNO++;
    }
    result += "{\"head\":\"" + first_100 + "\",\"tail\":\"" + last_100 + "\",\"mid\":\"" + middle_100 + "\"}";
    return result;
  }

protected:
  std::vector<std::shared_ptr<ypc::data_source_with_dhash>> m_datasources;
};
