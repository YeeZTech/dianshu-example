#include "common_type.h"
#include "ypc/core_t/analyzer/data_source.h"
#include "ypc/corecommon/data_source.h"
#include "ypc/corecommon/package.h"
#include "ypc/corecommon/to_type.h"
#include "ypc/stbox/ebyte.h"
#include "ypc/stbox/stx_common.h"
#include "ypc/stbox/tsgx/log.h"

#include <hpda/extractor/raw_data.h>
#include <hpda/output/memory_output.h>
#include <hpda/processor/query/filter.h>
#include <hpda/processor/set/intersection.h>
#include <hpda/processor/transform/concat.h>

#include <sgx_tcrypto.h>
#include <sgx_trts.h>

class company_info_parser {
public:
  company_info_parser() {}
  company_info_parser(
      std::vector<std::shared_ptr<ypc::data_source_with_dhash>> &source)
      : m_datasources(source){};

  inline stbox::bytes do_parse(const stbox::bytes &param) {
    LOG(INFO) << "do parse";
    if (m_datasources.size() != 2) {
      return stbox::bytes("Should include two data sources");
    }

    // set intersection
    hpda::processor::internal::ordered_intersection_impl<summary_item_t,
                                                         std::string>
        oi;
    ypc::to_type<stbox::bytes, company_item_t> conv_a(m_datasources[0].get());
    oi.add_upper_stream<::company_code>(&conv_a);
    ypc::to_type<stbox::bytes, tax_item_t> conv_b(m_datasources[1].get());
    oi.add_upper_stream<::company_code>(&conv_b);

    hpda::output::internal::memory_output_impl<summary_item_t> mo(&oi);
    mo.get_engine()->run();
    LOG(INFO) << "do parse done";

    stbox::bytes result;
    if (mo.values().empty()) {
      result = stbox::bytes("not found!");
    }
    LOG(INFO) << "mo.values size: " << mo.values().size();
    summary_pkg_t pkg;
    pkg.set<::summary_table_t>(mo.values());
    return ypc::make_bytes<stbox::bytes>::for_package(pkg);
  }

protected:
  std::vector<std::shared_ptr<ypc::data_source_with_dhash>> m_datasources;
};
