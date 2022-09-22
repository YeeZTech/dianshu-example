
#include "ypc/stbox/ebyte.h"
#include "ypc/stbox/stx_common.h"
#include "ypc/core_t/analyzer/data_source.h"
#include "ypc/stbox/tsgx/log.h"
#include "user_type.h"

#include "ypc/corecommon/to_type.h"
#include <hpda/extractor/raw_data.h>
#include <hpda/output/memory_output.h>
#include <hpda/processor/query/filter.h>

#include <cstring>
#include <string.h>

typedef ff::net::ntpackage<0, province_name > nt_package_t;

class gdp_quarter_query_parser {
public:
  gdp_quarter_query_parser(ypc::data_source<stbox::bytes> *source)
      : m_source(source){};

  std::vector< std::string > split( std::string str, std::string delim ) {
    std::vector< std::string > res;
    if ( str.size() == 0 ) return res;
    //先将要切割的字符串从string类型转换为char*类型
    char *strs = new char[ str.size() + 1 ]; //不要忘了
    for ( int i = 0; i < str.size(); ++ i ) {
      strs[ i ] = str[ i ];
    }
    strs[ str.size() ] = '\0';
    // strcpy( strs, str.c_str() );
  
    char *d = new char[ delim.size() + 1 ];
    for ( int i = 0; i < delim.size(); ++ i ) {
      d[ i ] = delim[ i ];
    }
    d[ delim.size() ] = '\0';
    // strcpy( d, delim.c_str() );
  
    char *p = strtok( strs, d );
    while ( p ) {
      std::string s = p; //分割得到的字符串转换为string类型
      res.push_back( s ); //存入结果数组
      p = strtok( NULL, d );
    }
  
    return res;
  }

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

    stbox::bytes result( "结果:\n" );
    result += stbox::bytes( "地区名称 ( 省级行政区划 / 中国 ), 年份季度 ( A、B、C、D 分别代表第一、第二、第三、第四季度 --- 如 2022B 表示 2022 年第二季度 ), GDP ( 亿元 ) \n" );
    bool is_found = false;
    for ( auto it : mo.values() ) {
      is_found = true;
      std::string pn = it.get< province_name >();
      LOG(INFO) << pn;
      std::string gq = it.get< gdp_quarters >();
      auto strs = split( gq, ";" );
      for ( auto item : strs ) {
        LOG(INFO) << item;
        auto info = split( item, ":" );
        std::string tmp = pn + "," + info[ 0 ] + "," + info[ 1 ] + "\n";
        LOG(INFO) << tmp;
        result += stbox::bytes( tmp );
      }
    }
    if ( !is_found ) {
      result = stbox::bytes( "您输入的参数不能匹配到对应的地区, 请重新提交\n" );
    }
    return result;
  }

protected:
  ypc::data_source<stbox::bytes> *m_source;
};

