
#include "ypc/stbox/ebyte.h"
#include "ypc/stbox/stx_common.h"
#include "ypc/core_t/analyzer/data_source.h"
#include "ypc/stbox/tsgx/log.h"
#include "user_type.h"

#include "ypc/corecommon/to_type.h"
#include <hpda/extractor/raw_data.h>
#include <hpda/output/memory_output.h>
#include <hpda/processor/query/filter.h>

typedef ff::net::ntpackage<0, county_name, year> nt_package_t;

class gdp_query_parser {
public:
  gdp_query_parser(ypc::data_source<stbox::bytes> *source)
      : m_source(source){};

  inline stbox::bytes do_parse(const stbox::bytes &param) {
    ypc::to_type<stbox::bytes, gdp_query_item_t> converter(m_source);

    auto pkg = ypc::make_package<nt_package_t>::from_bytes(param);
    int counter = 0;
    hpda::processor::internal::filter_impl<gdp_query_item_t> match(
        &converter, [&](const gdp_query_item_t &v) {
          counter++;
          std::string first_item = v.get<county_name>();
          std::string second_item = v.get<year>();
          if ( first_item == pkg.get<county_name>() && second_item == pkg.get<year>() ) {
            return true;
          }
          return false;
        });

    hpda::output::internal::memory_output_impl<gdp_query_item_t> mo(&match);
    mo.get_engine()->run();
    LOG(INFO) << "do parse done";

    stbox::bytes result("not found\n");
    // bool flag = false;
    for (auto it : mo.values()) {
      result = stbox::bytes(it.get<gdp>());
      break;
    }
    /*
    if (flag) {
      result = stbox::bytes("found\n");
    }
    */
    return result;
  }

protected:
  ypc::data_source<stbox::bytes> *m_source;
};

