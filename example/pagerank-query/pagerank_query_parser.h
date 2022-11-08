
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
#include <algorithm>

typedef ff::net::ntpackage< 0, addr > nt_package_t;

class pagerank_query_parser {
public:
  pagerank_query_parser(ypc::data_source<stbox::bytes> *source)
      : m_source(source){};

  inline stbox::bytes do_parse(const stbox::bytes &param) {
    ypc::to_type<stbox::bytes, pagerank_query_item_t> converter(m_source);

    auto pkg = ypc::make_package<nt_package_t>::from_bytes( param );
    int counter = 0;
    hpda::processor::internal::filter_impl<pagerank_query_item_t> match(
        &converter, [&](const pagerank_query_item_t &v) {
          counter++;
          std::string first_item = v.get< addr >();
          auto input = pkg.get< addr >();
          std::transform( input.begin(), input.end(), input.begin(), std::tolower );
          if ( first_item == input ) {
            return true;
          }
          return false;
        });

    hpda::output::internal::memory_output_impl<pagerank_query_item_t> mo(&match);
    mo.get_engine()->run();
    LOG(INFO) << "do parse done";

    stbox::bytes result;
    result += stbox::bytes( "以太坊账户地址( 如: 0xddfabcdc4d8ffc6d5beaf154f18b778f892a0740 ), pagerank算法对应数值, 时间跨度 ( 如: 2021-08-08-00:00:00---2021-09-19-00:00:00 )\n");
    bool flag = false;
    std::vector< std::vector< std::string > > temp;
    int count = 0;
    for ( auto it : mo.values() ) {
      std::vector< std::string > temp_temp;
      temp_temp.push_back( std::string( it.get< addr >() ) );
      temp_temp.push_back( std::string( it.get< pr_value >() ) );
      temp_temp.push_back( std::string( it.get< date_span >() ) );
      temp.emplace_back( temp_temp );
    }
    std::sort( temp.begin(), temp.end(), []( const std::vector< std::string >& a, const std::vector< std::string >& b ){ 
          return a[ 2 ] > b[ 2 ];
		    } );
    for (auto it : temp ) {
      flag = true;
      result += stbox::bytes( it[ 0 ] );
      result += stbox::bytes(",");
      result += stbox::bytes( it[ 1 ] );
      result += stbox::bytes(",");
      result += stbox::bytes( it[ 2 ] );
      result += stbox::bytes("\n");
    }
    if ( !flag ) {
      result = stbox::bytes( "您输入的参数不能匹配到对应的以太坊账户地址, 请重新提交\n" );
    }
    return result;
  }

protected:
  ypc::data_source<stbox::bytes> *m_source;
};

