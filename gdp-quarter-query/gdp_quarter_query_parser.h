
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

    stbox::bytes result( "结果:\n" );
    result += stbox::bytes( "地区名称 ( 省级行政区划 / 中国 ), 年份季度 ( A、B、C、D 分别代表第一、第二、第三、第四季度, 如 2022B 表示 2022 年第二季度 ), GDP ( 亿元 ) \n" );
    bool is_found = false;
    for (auto it : mo.values()) {
      is_found = true;
      result += stbox::bytes( it.get< province_name >() );
      result += stbox::bytes( "\n" );
      result += stbox::bytes( it.get< gdp_quarters >() );
      result += stbox::bytes( "\n" );
    }
    if ( !is_found ) {
      result = stbox::bytes( "您输入的参数不能匹配到对应的地区, 请重新提交\n" );
    }
    return result;
  }

protected:
  ypc::data_source<stbox::bytes> *m_source;
};

