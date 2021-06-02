#include "stbox/ebyte.h"
#include "stbox/stx_common.h"
#ifdef EXAMPLE_FM_NORMAL
#include "glog/logging.h"
#else
#include "stbox/tsgx/log.h"
#endif
#include "user_type.h"
#include <hpda/extractor/raw_data.h>
#include <hpda/output/memory_output.h>
#include <hpda/processor/query/filter.h>
#include <string.h>

class enclave_us_covid19_parser {
public:
  enclave_us_covid19_parser() {}
  enclave_us_covid19_parser(
      ::hpda::extractor::internal::extractor_base<user_item_t> *source)
      : m_source(source){};

  inline stbox::bytes do_parse(const stbox::bytes &param) {
    LOG(INFO) << "do parse";
    int counter = 0;
    hpda::processor::internal::filter_impl<user_item_t> match(
        m_source, [&](const user_item_t &v) {
          counter++;
          std::string date = v.get<::date>();
          if (memcmp(date.c_str(), param.data(), date.size()) == 0) {
            return true;
          }
          return false;
        });

    hpda::output::internal::memory_output_impl<user_item_t> mo(&match);
    mo.run();
    LOG(INFO) << "do parse done";

    stbox::bytes result;
    stbox::printf("checked %d items\n", counter);
    return result;
  }
  inline bool merge_parse_result(const std::vector<stbox::bytes> &block_result,
                                 const stbox::bytes &param,
                                 stbox::bytes &result) {
    stbox::bytes s;
    for (auto k : block_result) {
      s = s + k;
    }
    result = s;
    return false;
  }

protected:
  hpda::extractor::internal::extractor_base<user_item_t> *m_source;
};
