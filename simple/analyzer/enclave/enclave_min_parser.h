#include "stbox/ebyte.h"
#include "stbox/stx_common.h"
#include "stbox/tsgx/log.h"
#include "user_type.h"
#include <hpda/extractor/raw_data.h>
#include <hpda/output/memory_output.h>
#include <hpda/processor/query/filter.h>
#include <string.h>

class enclave_min_parser {
public:
  enclave_min_parser() {}
  enclave_min_parser(
      ::hpda::extractor::internal::extractor_base<user_item_t> *source)
      : m_source(source){};

  inline stbox::bytes do_parse(const stbox::bytes &param) {
    LOG(INFO) << "do parse";
    int counter = 0;
    stbox::bytes result;
    hpda::processor::internal::filter_impl<user_item_t> match(
        m_source, [&](const user_item_t &v) {
          // TODO first line has been read twice
          if (counter < 2) {
            counter++;
            return false;
          }
          std::string key = v.get<::first_item>();
          // initialize result
          if (2 == counter) {
            result = stbox::bytes(key);
          }
          counter++;
          size_t size = std::max(key.size(), result.size());
          if (memcmp(key.c_str(), result.data(), size) < 0) {
            result = stbox::bytes(key);
            return true;
          }
          return false;
        });

    hpda::output::internal::memory_output_impl<user_item_t> mo(&match);
    mo.run();
    LOG(INFO) << "do parse done";

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
