
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
    LOG(INFO) << "into converter";
    ypc::to_type<stbox::bytes, gdp_prediction_item_t> converter(m_source);
    LOG(INFO) << "into from_bytes";
    auto pkg = ypc::make_package<nt_package_t>::from_bytes(param);
    LOG(INFO) << "filter_impl";
    hpda::processor::internal::filter_impl<gdp_prediction_item_t> match(
        &converter, [&](const gdp_prediction_item_t &v) {
          std::string first_item = v.get<name>();
          // LOG(INFO) << "first: " << first_item;
          // LOG(INFO) << "input: " << v.get<name>();
          if ( first_item == pkg.get< name >() ) {
            LOG(INFO) << "-->first: " << first_item;
            LOG(INFO) << "-->input: " << v.get<name>();
            return true;
          }
          return false;
        });
    LOG(INFO) << "into mo";
    hpda::output::internal::memory_output_impl<gdp_prediction_item_t> mo(&match);
    LOG(INFO) << "into run";
    mo.get_engine()->run();
    LOG(INFO) << "do parse done";
    double year_rad = 0.0;
    double ar = 0;
    bool is_found = false;
    bool is_month_covered[ 12 ];
    for ( int i = 0; i < 12; ++ i ) {
      is_month_covered[ i ] = false;
    }
    for (auto it : mo.values()) {
      is_found = true;
      int ym = std::stoi( it.get< year_month >() );
      ar = std::stod( it.get< area >() );
      double rp = std::stod( it.get< rad >() );
      if ( ym < 202113 && 202100 < ym ) {
        if ( !is_month_covered[ ym % 100 - 1 ] ) {
          year_rad += rp;
          LOG(INFO) << "-----: " << ym % 100 << " year_rad " << year_rad;
          LOG(INFO) << "month: " << ym % 100 << " monthrad " << rp;
          is_month_covered[ ym % 100 - 1 ] = true;
        }
      }
    }

    double slope = 477.64200343479285;
    double vertical_intercept = -550.769860260704;
    auto res = slope * ( year_rad / 12.0 ) + vertical_intercept * ar;
    auto temp = std::to_string( res );
    stbox::bytes result( "地区名称 ( 区县 ) , 2021年GDP预估 ( 万元 ) \n" );
    result += stbox::bytes( pkg.get< name >() );
    result += stbox::bytes( "," );
    result += stbox::bytes( temp );
    result += stbox::bytes( "\n" );
    if ( !is_found )
      result = stbox::bytes( "您输入的参数不能匹配到对应的地区, 请重新提交\n" );
    return result;
  }

protected:
  ypc::data_source<stbox::bytes> *m_source;
};

