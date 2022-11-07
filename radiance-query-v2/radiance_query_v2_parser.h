
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

class radiance_query_v2_parser {
public:
  radiance_query_v2_parser(ypc::data_source<stbox::bytes> *source)
      : m_source(source){};
  
  bool is_ordered_subset( std::string s, std::string p ) {
    if ( s.size() < p.size() )
      return false;
    
    int ps = 0;
    int pp = 0;
    while ( pp < p.size() && ps < s.size() ) {
      if ( pp == p.size() - 1 && s[ ps ] == p[ pp ] )
        return true;
      if ( p[ pp ] == s[ ps ] ) {
        pp ++;
        ps ++;
      } else {
        ps ++;
      }
    }
    return false;
  }

  inline stbox::bytes do_parse(const stbox::bytes &param) {
    LOG(INFO) << param;
    ypc::to_type<stbox::bytes, radiance_query_v2_item_t> converter(m_source);
    LOG(INFO) << "1";
    auto pkg = ypc::make_package<nt_package_t>::from_bytes(param);
    int counter = 0;
    LOG(INFO) << "2";
    hpda::processor::internal::filter_impl<radiance_query_v2_item_t> match(
        &converter, [&](const radiance_query_v2_item_t &v) {
          std::string first_item = v.get<name>();
          if ( counter < 246 && is_ordered_subset( first_item, pkg.get< name >() ) ) {
            counter ++;
            return true;
          }
          return false;
        });

    hpda::output::internal::memory_output_impl<radiance_query_v2_item_t> mo(&match);
    mo.get_engine()->run();
    LOG(INFO) << "do parse done";

    stbox::bytes result;
    result += stbox::bytes("地区名称, 年份月份( 如: 202206 表示 2022年06月 ), 行政等级 ( 1为省级行政区, 2为地级行政区, 3为县级行政区 ) , 地区首府经度, 地区首府纬度, 地区行政编码, 上一级行政单位行政编码, 面积 ( 平方公里 ), 灯光亮度总和 ( nW/cm^2/sr ), 单位面积灯光亮度总和 ( nW/cm^2/sr / 平方公里 )\n");
    bool flag = false;
    std::vector< std::vector< std::string > > temp;
    int count = 0;
    for ( auto it : mo.values() ) {
      if ( count++ >= 246 )
        break;
      std::vector< std::string > temp_temp;
      temp_temp.push_back( std::string( it.get< name >() ) );
      temp_temp.push_back( std::string( it.get< year_month >() ) );
      temp_temp.push_back( std::string( it.get< level >() ) );
      temp_temp.push_back( std::string( it.get< center_longitude >() ) );
      temp_temp.push_back( std::string( it.get< center_latitude >() ) );
      temp_temp.push_back( std::string( it.get< code >() ) );
      temp_temp.push_back( std::string( it.get< parent_code >() ) );
      temp_temp.push_back( std::string( it.get< area >() ) );
      temp_temp.push_back( std::string( it.get< rad >() ) );
      temp_temp.push_back( std::string( it.get< rad_per_area >() ) );
      temp.emplace_back( temp_temp );
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
      result += stbox::bytes(",");
      result += stbox::bytes( it[ 6 ] );
      result += stbox::bytes(",");
      result += stbox::bytes( it[ 7 ] );
      result += stbox::bytes(",");
      result += stbox::bytes( it[ 8 ] );
      result += stbox::bytes(",");
      result += stbox::bytes( it[ 9 ] );
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

