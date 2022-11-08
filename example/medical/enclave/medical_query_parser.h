#include "stbox/ebyte.h"
#include "stbox/stx_common.h"
#include "ypc_t/analyzer/data_source.h"
#include "stbox/tsgx/log.h"
#include "user_type.h"

#include "corecommon/to_type.h"
#include <hpda/extractor/raw_data.h>
#include <hpda/output/memory_output.h>
#include <hpda/processor/query/filter.h>

typedef erke_fixed_item_t user_item_t;
typedef ff::net::ntpackage<0, Title> title_t;

class medical_query_parser {
public:
  medical_query_parser(ypc::data_source<stbox::bytes> *source)
      : m_source(source){};

  inline stbox::bytes do_parse(const stbox::bytes &param) {
    ypc::to_type<stbox::bytes, user_item_t> converter(m_source);

    auto pkg = ypc::make_package<title_t>::from_bytes(param);
    int counter = 0;
    hpda::processor::internal::filter_impl<user_item_t> match(
        &converter, [&](const user_item_t &v) {
          counter++;
          std::string title = v.get<Title>();
          if (title == pkg.get<Title>()) {
            return true;
          }
          return false;
        });

    hpda::output::internal::memory_output_impl<user_item_t> mo(&match);
    mo.get_engine()->run();
    LOG(INFO) << "do parse done";

    stbox::bytes result("find result is:\n");
    for (auto it : mo.values()) {
      stbox::printf("found\n");
      result += it.get<Department>();
      result += "\n\n";
      result += it.get<Title>();
      result += "\n\n";
      result += it.get<Ask>();
      result += "\n\n";
      result += it.get<Answer>();
      result += "\n\n";
    }
    return result;
  }

protected:
  ypc::data_source<stbox::bytes> *m_source;
};
