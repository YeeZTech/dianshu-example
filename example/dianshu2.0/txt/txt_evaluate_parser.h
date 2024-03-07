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

    hpda::processor::internal::filter_impl<txt_batch_item_t> match(
        &converter, [&](const txt_batch_item_t &v) {
          std::string batch = v.get<::txt_batch>();
          return true;
        });

    hpda::output::internal::memory_output_impl<txt_batch_item_t> mo(&match);
    mo.get_engine()->run();

    stbox::bytes result;

    for (auto it : mo.values()) {
        std::string batch = it.get<txt_batch>();
        size_t batch_length = batch.length();

        std::string first_100 = batch.substr(0, 100);
    
        size_t middle_start = batch.length() / 2 - 50;
        std::string middle_100 = batch.substr(middle_start, 100);
    
        std::string last_100 = batch.substr(batch.length() - 100);
    
        result += "{\"head\":\"" + first_100 + "\",\"tail\":\"" + last_100 + "\",\"mid\":\"" + middle_100 + "\"}";

    }

    return result;
  }

protected:
  std::vector<std::shared_ptr<ypc::data_source_with_dhash>> m_datasources;
};
