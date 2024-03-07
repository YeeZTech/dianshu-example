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

class download_parser {
public:
  download_parser(
      std::vector<std::shared_ptr<ypc::data_source_with_dhash>> &source)
      : m_datasources(source) {}

  inline stbox::bytes do_parse(const stbox::bytes &param) {
    LOG(INFO) << "param: " << param;
    ypc::to_type<stbox::bytes, download_batch_item_t> converter(
        m_datasources[0].get());

    int counter = 0;
    hpda::processor::internal::filter_impl<download_batch_item_t> match(
        &converter, [&](const download_batch_item_t &v) {
          counter++;
          std::string batch = v.get<::download_batch>();
          LOG(INFO) << batch;
          return true;
        });

    hpda::output::internal::memory_output_impl<download_batch_item_t> mo(
        &match);
    mo.get_engine()->run();
    LOG(INFO) << "batch: " << counter;
    LOG(INFO) << "do parse done";

    stbox::bytes result("Fidelius");
    for (auto it : mo.values()) {
    }
    return result;
  }

protected:
  std::vector<std::shared_ptr<ypc::data_source_with_dhash>> m_datasources;
};
