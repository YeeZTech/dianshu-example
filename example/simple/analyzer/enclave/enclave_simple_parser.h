#include "corecommon/package.h"
#include "stbox/ebyte.h"
#include "stbox/stx_common.h"
#include "stbox/tsgx/log.h"
#include "user_type.h"
#include <hpda/extractor/raw_data.h>
#include <hpda/output/memory_output.h>
#include <hpda/processor/query/filter.h>
#include <string.h>

define_nt(csv_first_item, std::string);
typedef ff::net::ntpackage<0, csv_first_item> csv_first_item_t;

class enclave_simple_parser {
public:
  enclave_simple_parser() {}
  template <typename ET>
  enclave_simple_parser(
      ::hpda::extractor::internal::extractor_base<user_item_t> *source,
      ET &&ignore)
      : m_source(source){};

  inline stbox::bytes do_parse(const stbox::bytes &param) {
    LOG(INFO) << "do parse";
    LOG(INFO) << "param size: " << param.size();
    LOG(INFO) << "param: " << param;
    auto pkg = ypc::make_package<csv_first_item_t>::from_bytes(param);
    LOG(INFO) << "make package, data: " << pkg.get<csv_first_item>();
    auto k = pkg.get<csv_first_item>();
    LOG(INFO) << "raw: " << stbox::bytes(k);
    int counter = 0;
    hpda::processor::internal::filter_impl<user_item_t> match(
        m_source, [&](const user_item_t &v) {
          counter++;
          std::string key = v.get<::first_item>();
          auto k = pkg.get<csv_first_item>();
          if (key == pkg.get<csv_first_item>()) {
            LOG(INFO) << "true";
            return true;
          }
          // LOG(INFO) << "false: " << stbox::bytes(key) << ":" << key.size()
          //<< ", " << stbox::bytes(k);
          return false;
          // param may ignore quotes
          // stbox::bytes quote_param =
          // stbox::bytes("\"") + param + stbox::bytes("\"");
          // if (memcmp(key.c_str(), param.data(), key.size()) == 0 ||
          // memcmp(key.c_str(), quote_param.data(), key.size()) == 0) {
          // return true;
          //}
          // return false;
        });

    hpda::output::internal::memory_output_impl<user_item_t> mo(&match);
    mo.get_engine()->run();
    LOG(INFO) << "do parse done";

    stbox::bytes result;
    for (auto it : mo.values()) {
      result += pkg.get<csv_first_item>();
      result += stbox::bytes(",");
      result += it.get<::others>();
      result += stbox::bytes("\n");
    }
    if (result.size() == 0) {
      result = stbox::bytes("found nothing");
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
