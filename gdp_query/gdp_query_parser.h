
#include "ypc/stbox/ebyte.h"
#include "ypc/stbox/stx_common.h"
#include "ypc/core_t/analyzer/data_source.h"
#include "ypc/stbox/tsgx/log.h"
#include "user_type.h"

#include "ypc/corecommon/to_type.h"
#include <hpda/extractor/raw_data.h>
#include <hpda/output/memory_output.h>
#include <hpda/processor/query/filter.h>

typedef ff::net::ntpackage<0, county_name > nt_package_t;

class gdp_query_parser {
public:
  gdp_query_parser(ypc::data_source<stbox::bytes> *source)
      : m_source(source){};

  inline stbox::bytes do_parse(const stbox::bytes &param) {
    ypc::to_type<stbox::bytes, gdp_query_item_t> converter(m_source);
    LOG(INFO) << "into from_bytes";
    auto pkg = ypc::make_package<nt_package_t>::from_bytes(param);
    int counter = 0;
    hpda::processor::internal::filter_impl<gdp_query_item_t> match(
        &converter, [&](const gdp_query_item_t &v) {
          counter++;
          std::string first_item = v.get<county_name>();
          if ( first_item == pkg.get<county_name>() || first_item.find( pkg.get< county_name >() ) != std::string::npos ) {
            return true;
          }
          return false;
        });

    hpda::output::internal::memory_output_impl<gdp_query_item_t> mo(&match);
    mo.get_engine()->run();
    LOG(INFO) << "do parse done";

    stbox::bytes result( pkg.get< county_name >() );
    result += stbox::bytes( "\n" );
    result += stbox::bytes( "地区名称 ( 区/县 ), 年份, GDP ( 万元 ), 第一产业增加值 ( 万元 ), 第二产业增加值 ( 万元 ) , 面积 ( 平方公里 ) \n" );
    bool flag = false;
    int count = 0;
    std::vector< std::vector< std::string > > temp;
    for ( auto it : mo.values() ) {
      if ( count >= 50 )
        break;
      std::vector< std::string > temp_temp;
      temp_temp.push_back( std::string( it.get< county_name >() ) );
      temp_temp.push_back( std::string( it.get< year >() ) );
      temp_temp.push_back( std::string( it.get< gdp >() ) );
      temp_temp.push_back( std::string( it.get< gdp_1 >() ) );
      temp_temp.push_back( std::string( it.get< gdp_2 >() ) );
      temp_temp.push_back( std::string( it.get< area >() ) );
      temp.emplace_back( temp_temp );
      count ++;
    }
    std::sort( temp.begin(), temp.end(), []( const std::vector< std::string >& a, const std::vector< std::string >& b ){ 
          if ( a[ 0 ] == b[ 0 ] )
            return a[ 1 ] > b[ 1 ];
          else
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
      result += stbox::bytes(",");
      result += stbox::bytes( it[ 5 ] );
      result += stbox::bytes("\n");
    }
    if (!flag) {
      result = stbox::bytes( "您输入的参数不能匹配到对应的地区, 请重新提交\n" );
    }
    return result;
  }

protected:
  ypc::data_source<stbox::bytes> *m_source;
};

