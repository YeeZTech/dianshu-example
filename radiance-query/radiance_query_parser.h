
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
    for (auto it : mo.values()) {
      flag = true;
      result += stbox::bytes(it.get<year_month>());
      result += stbox::bytes(",");
      result += stbox::bytes(it.get<name>());
      result += stbox::bytes(",");
      result += stbox::bytes(it.get<area>());
      result += stbox::bytes(",");
      result += stbox::bytes(it.get<rad>());
      result += stbox::bytes(",");
      result += stbox::bytes(it.get<rad_perarea>());
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

