#include "eparser_t.h"
#include "user_type.h"
#include "ypc/common/limits.h"
#include "ypc/core_t/analyzer/data_source.h"
#include "ypc/corecommon/package.h"
#include "ypc/corecommon/to_type.h"
#include "ypc/stbox/ebyte.h"
#include "ypc/stbox/scope_guard.h"
#include "ypc/stbox/stx_common.h"
#include "ypc/stbox/tsgx/log.h"
#include "ypc/stbox/tsgx/ocall.h"
#include <hpda/extractor/raw_data.h>
#include <hpda/output/memory_output.h>
#include <hpda/processor/query/filter.h>
#include <hpda/processor/transform/all.h>
#include <string.h>
#include <unordered_map>
class tax_parser {
public:
  tax_parser(ypc::data_source<stbox::bytes> *source) : m_source(source){};
  inline stbox::bytes do_parse(const stbox::bytes &param) {
    LOG(INFO) << "param size: " << param.size();
    LOG(INFO) << "param: " << param;
    ypc::to_type<stbox::bytes, tax_item_t> converter(m_source);
    hpda::processor::internal::filter_impl<tax_item_t> match(
        &converter, [&](const tax_item_t &v) {
          if (((v.get<tax_income>() > 1000000) &&
               (v.get<tax_income>() < 3000000))) {
            return true;
          }
          return false;
        });
    LOG(INFO) << "match done";
    hpda::processor::internal::all_impl<tax_item_t> a(&match);
    LOG(INFO) << "all done";
    hpda::output::internal::memory_output_impl<tax_item_t> mo(&a);
    LOG(INFO) << "memory output done";
    mo.get_engine()->run();
    LOG(INFO) << "do parse done";

    stbox::bytes result;
    for (auto &it : mo.values()) {
      result += (it.get<company_code>() + ",");
      result += (std::to_string(it.get<tax_income>()) + ",");
      result += "\n";
    }

    return result;
  }
protected:
  ypc::data_source<stbox::bytes> *m_source;
};
