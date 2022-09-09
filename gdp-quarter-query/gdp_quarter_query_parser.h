
#include "ypc/stbox/ebyte.h"
#include "ypc/stbox/stx_common.h"
#include "ypc/core_t/analyzer/data_source.h"
#include "ypc/stbox/tsgx/log.h"
#include "user_type.h"

#include "ypc/corecommon/to_type.h"
#include <hpda/extractor/raw_data.h>
#include <hpda/output/memory_output.h>
#include <hpda/processor/query/filter.h>

typedef ff::net::ntpackage<0, province_name > nt_package_t;

class gdp_quarter_query_parser {
public:
  gdp_quarter_query_parser(ypc::data_source<stbox::bytes> *source)
      : m_source(source){};

  inline stbox::bytes do_parse(const stbox::bytes &param) {
    ypc::to_type<stbox::bytes, gdp_quarter_query_item_t> converter(m_source);

    auto pkg = ypc::make_package<nt_package_t>::from_bytes(param);
    hpda::processor::internal::filter_impl<gdp_quarter_query_item_t> match(
        &converter, [&](const gdp_quarter_query_item_t &v) {
          std::string first_item = v.get<province_name>();
          if ( first_item == pkg.get<province_name>() || first_item.find( pkg.get< province_name >() ) != std::string::npos ) {
            return true;
          }
          return false;
        });

    hpda::output::internal::memory_output_impl<gdp_quarter_query_item_t> mo(&match);
    mo.get_engine()->run();
    LOG(INFO) << "do parse done";

    stbox::bytes result( "" );
    bool is_found = false;
    for (auto it : mo.values()) {
      is_found = true;
      result += stbox::bytes( pkg.get< province_name >() );
      result += stbox::bytes( '\n' );
      result += stbox::bytes(it.get< gdp_quarters >());
      break;
    }
    if ( !is_found ) {
      result = stbox::bytes( "NOT FOUND!" );
    }
    return result;
  }

protected:
  ypc::data_source<stbox::bytes> *m_source;
};

