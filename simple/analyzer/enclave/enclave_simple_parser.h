#include "stbox/ebyte.h"
#include "stbox/stx_common.h"
#include "stbox/tsgx/log.h"
#include "user_type.h"
#include <hpda/extractor/raw_data.h>
#include <hpda/output/memory_output.h>
#include <hpda/processor/query/filter.h>
#include <string.h>

class enclave_simple_parser {
public:
  enclave_simple_parser() {}
  enclave_simple_parser(
      ::hpda::extractor::internal::extractor_base<user_item_t> *source)
      : m_source(source){};

  inline stbox::bytes do_parse(const stbox::bytes &param) {
    LOG(INFO) << "do parse";
    int counter = 0;
    hpda::processor::internal::filter_impl<user_item_t> match(
        m_source, [&](const user_item_t &v) {
          counter++;
          std::string key = v.get<::first_item>();
          if (memcmp(key.c_str(), param.data(), key.size()) == 0) {
            return true;
          }
          return false;
        });

    hpda::output::internal::memory_output_impl<user_item_t> mo(&match);
    mo.run();
    LOG(INFO) << "do parse done";

    stbox::bytes result;
    for (auto it : mo.values()) {
      result += it.get<::others>();
      result += stbox::bytes("\n");
    }
    stbox::printf("counter: %d\n", counter);
    stbox::printf("result size: %d\n", result.size());
    stbox::printf("result: %s\n", result.data());
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
