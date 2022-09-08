
#include "ypc/stbox/ebyte.h"
#include "ypc/stbox/stx_common.h"
#include "ypc/core_t/analyzer/data_source.h"
#include "ypc/stbox/tsgx/log.h"
#include "user_type.h"

#include "ypc/corecommon/to_type.h"
#include <hpda/extractor/raw_data.h>
#include <hpda/output/memory_output.h>
#include <hpda/processor/query/filter.h>
#include <vector>

typedef ff::net::ntpackage<0, name> nt_package_t;

class radiance_query_parser {
public:
  radiance_query_parser(ypc::data_source<stbox::bytes> *source)
      : m_source(source){};

  inline stbox::bytes do_parse(const stbox::bytes &param) {
    ypc::to_type<stbox::bytes, radiance_query_item_t> converter(m_source);

    auto pkg = ypc::make_package<nt_package_t>::from_bytes(param);
    int counter = 0;
    hpda::processor::internal::filter_impl<radiance_query_item_t> match(
        &converter, [&](const radiance_query_item_t &v) {
          counter++;
          std::string first_item = v.get<name>();
          if ( first_item == pkg.get<name>() ) {
            return true;
          }
          return false;
        });

    hpda::output::internal::memory_output_impl<radiance_query_item_t> mo(&match);
    mo.get_engine()->run();
    LOG(INFO) << "do parse done";

    stbox::bytes result;
    bool flag = false;
    std::vector< std::vector< std::string > > temp;
    for ( auto it : mo.values() ) {
      std::vector< std::string > temp_temp;
      temp_temp.push_back( std::string( it.get< year_month >() ) );
      temp_temp.push_back( std::string( it.get< name >() ) );
      temp_temp.push_back( std::string( it.get< area >() ) );
      temp_temp.push_back( std::string( it.get< rad >() ) );
      temp_temp.push_back( std::string( it.get< rad_perarea >() ) );
      temp.emplace_back( temp_temp );
    }
    std::sort( temp.begin(), temp.end(), []( const std::vector< std::string >& a, const std::vector< std::string >& b ){ 
		      return a[ 0 ] > b[ 0 ]; 
		    } );
    for (auto it : temp ) {
      flag = true;
      result += stbox::bytes( it[ 0 ] );
      result += stbox::bytes(",");
      result += stbox::bytes( it[ 1 ] );
      result += stbox::bytes(",");
      result += stbox::bytes( it[ 2 ] );
      result += stbox::bytes(",");
      result += stbox::bytes( it[ 3 ] );
      result += stbox::bytes(",");
      result += stbox::bytes( it[ 4 ] );
      result += stbox::bytes("\n");
    }
    if (!flag) {
      result = stbox::bytes("not found\n");
    }
    return result;
  }

protected:
  ypc::data_source<stbox::bytes> *m_source;
};

