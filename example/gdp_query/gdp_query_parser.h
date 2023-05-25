
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
  gdp_query_parser(
      std::vector<std::shared_ptr<ypc::data_source_with_dhash>> &source)
      : m_datasources(source) {}

  inline stbox::bytes do_parse(const stbox::bytes &param) {
    LOG(INFO) << "param: " << param;
    ypc::to_type<stbox::bytes, gdp_query_item_t> converter(
        m_datasources[0].get());

    auto pkg = ypc::make_package<nt_package_t>::from_bytes(param);
    LOG(INFO) << "county_name: " << pkg.get<::county_name>();
    int counter = 0;
    hpda::processor::internal::filter_impl<gdp_query_item_t> match(
        &converter, [&](const gdp_query_item_t &v) {
          counter++;
          std::string first_item = v.get<county_name>();
          if ( first_item == pkg.get<county_name>() || first_item.find( pkg.get< county_name >() ) != std::string::npos ) {
            LOG(INFO) << "found";
            return true;
          }
          return false;
        });

    hpda::output::internal::memory_output_impl<gdp_query_item_t> mo(&match);
    mo.get_engine()->run();
    LOG(INFO) << "do parse done";

    stbox::bytes result;
    result += stbox::bytes( "地区名称 ( 区/县 ), 年份, GDP ( 万元 ), 第一产业增加值 ( 万元 ), 第二产业增加值 ( 万元 ) , 面积 ( 平方公里 ) \n" );
    bool flag = false;
    std::vector< std::vector< std::string > > temp;
    for ( auto it : mo.values() ) {
      result += it.get<county_name>();
      result += ",";
      result += it.get<year>();
      result += ",";
      result += it.get<gdp>();
      result += ",";
      result += it.get<gdp_1>();
      result += ",";
      result += it.get<gdp_2>();
      result += ",";
      result += it.get<area>();
      result += "\n";
      flag = true;
    }
    if (!flag) {
      result = stbox::bytes( "您输入的参数不能匹配到对应的地区, 请重新提交\n" );
    }
    return result;
  }

protected:
  std::vector<std::shared_ptr<ypc::data_source_with_dhash>> m_datasources;
};

