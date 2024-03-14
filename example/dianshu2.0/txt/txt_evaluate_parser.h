#include "user_type.h"
#include "string_process.h"
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
    int total_size = 0;
    int total_without_fo = 0; // 不包含文件操作符的总长度 
    int end_size; 

    LOG(INFO) << "本地测试内容:" << param;
    
    hpda::processor::internal::filter_impl<txt_batch_item_t> match(
        &converter, [&](const txt_batch_item_t &v) {
          counter++;
          std::string batch = v.get<::txt_batch>();
          total_size += batch.length();
          total_without_fo += get_string_length(batch);
          end_size = batch.length();
          return true;
        });
    
    hpda::output::internal::memory_output_impl<txt_batch_item_t> mo(&match);
    mo.get_engine()->run();
    
    LOG(INFO) << "Total Size: " << total_size;
    LOG(INFO) << "Total Size Without File Operator: " << total_without_fo;
    LOG(INFO) << "End Size: " << end_size;

    int batch_num = total_size / batch_max_size;
    bool need_next_batch = false; 
    int extral_size = 0; 
    int start_pos = total_size / 2 - 150; 
    int mid_batch_no = start_pos / batch_max_size; 
    
    start_pos = start_pos % batch_max_size; 

    LOG(INFO) << "Batch Num: " << batch_num;
    LOG(INFO) << "Start Position: " << start_pos;
    LOG(INFO) << "Mid Batch No: " << mid_batch_no;

    if(start_pos > (batch_max_size - 150)){
      need_next_batch = true;
      extral_size = start_pos - (batch_max_size - 150);
    }

    std::string result;
    std::string first_150;
    std::string middle_300;
    std::string last_150;

    int tempNO = 0;
    // TODO  对于‘/’符号怎么处理 -- json问题
    
    LOG(INFO) << "Middle Start Position: " << total_size;
    for (auto it : mo.values()) {
        std::string batch = it.get<txt_batch>();
        size_t batch_length = batch.length();

        if(total_size < 1500) {
          // 小于1500个字节时只取前后的10%与中间20%作为结果，最小为1个字节
          if(batch_length < 10) {
            first_150 = batch.substr(0, 1);
            middle_300 = batch.substr(batch_length / 2, batch_length/5);
            last_150 = batch.substr(batch_length-1);
          } else {
            first_150 = batch.substr(0, batch_length / 10);
            middle_300 = batch.substr(batch_length / 2 - batch_length / 20, batch_length / 5);
            last_150 = batch.substr(batch_length - batch_length / 10);
          }
          break;
        }

        if(tempNO == 0) { 
          first_150 = batch.substr(0, 150);
        }
        
        if(need_next_batch) {
          if(tempNO == mid_batch_no) {
            middle_300 = batch.substr(batch.length(), 300 - extral_size);
          }
          if(tempNO == mid_batch_no+1) {
            middle_300 += batch.substr(0, extral_size);
          }
        } else if(tempNO == mid_batch_no) {
            middle_300 = batch.substr(start_pos, 300);
          }

        
        if(tempNO==counter-1) {
          last_150 = batch.substr(batch.length() - 150);
        }
        
        tempNO++;
    }

    result += "{\"head\":\"" + process_string(first_150) + "\",\"tail\":\"" + process_string(last_150) + "\",\"mid\":\"" + process_string(middle_300) + "\",\"total\":\"" + std::to_string(total_size) + "\"}";
    
    // result += "引号测试:\",反斜线引号测试\\\"";

    LOG(INFO) << "函数内返回Reulst:" << result;
    
    return stbox::bytes(result);
  }

protected:
  std::vector<std::shared_ptr<ypc::data_source_with_dhash>> m_datasources;
};
