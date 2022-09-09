
#include "ypc/stbox/ebyte.h"
#include "ypc/stbox/stx_common.h"
#include "ypc/core_t/analyzer/data_source.h"
#include "ypc/stbox/tsgx/log.h"
#include "user_type.h"

#include "ypc/corecommon/to_type.h"
#include <hpda/extractor/raw_data.h>
#include <hpda/output/memory_output.h>
#include <hpda/processor/query/filter.h>

typedef ff::net::ntpackage<0, name> nt_package_t;

class gdp_prediction_parser {
public:
  gdp_prediction_parser(ypc::data_source<stbox::bytes> *source)
      : m_source(source){};

  inline stbox::bytes do_parse(const stbox::bytes &param) {
    ypc::to_type<stbox::bytes, gdp_prediction_item_t> converter(m_source);

    auto pkg = ypc::make_package<nt_package_t>::from_bytes(param);

    hpda::processor::internal::filter_impl<gdp_prediction_item_t> match(
        &converter, [&](const gdp_prediction_item_t &v) {
          std::string first_item = v.get<name>();
          if ( first_item == pkg.get<name>() || ( first_item.find( pkg.get< name >() ) != std::string::npos && pkg.get< name >()。size() >= 6 ) ) {
            return true;
          }
          return false;
        });

    hpda::output::internal::memory_output_impl<gdp_prediction_item_t> mo(&match);
    mo.get_engine()->run();
    LOG(INFO) << "do parse done";
    double year_rad = 0;
    double ar = 0;
    for (auto it : mo.values()) {
      int ym = std::stoi( it.get< year_month >() );
      ar = std::stod( it.get< area >() );
      double rp = std::stod( it.get< rad_perarea >() );
      if ( ym < 202113 && 202100 < ym ) {
        year_rad += rp;
      }
    }

    double slope = 4787.502598784844;
    double vertical_intercept = -80.42706901157614;
    auto res = slope * ( year_rad / 12 ) + vertical_intercept;
    auto temp = std::to_string( res * ar );
    stbox::bytes result( pkg.get< name >() );
    result += stbox::bytes( "\n" );
    result += stbox::bytes( "GDP(万元)\n" );
    result += stbox::bytes( temp );
    result += stbox::bytes( "\n" );
    return result;
  }

protected:
  ypc::data_source<stbox::bytes> *m_source;
};

