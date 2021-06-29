#include "common/limits.h"
#include "eparser_t.h" // ocall_http_request
#include "stbox/ebyte.h"
#include "stbox/scope_guard.h"
#include "stbox/stx_common.h"
#include "stbox/tsgx/ocall.h"
#ifdef EXAMPLE_FM_NORMAL
#include "glog/logging.h"
#else
#include "stbox/tsgx/log.h"
#endif
#include "common/limits.h"
#include "user_type.h"
#include <hpda/extractor/raw_data.h>
#include <hpda/output/memory_output.h>
#include <hpda/processor/query/filter.h>
#include <string.h>

class ip_search_parser {
public:
  ip_search_parser() {}
  ip_search_parser(
      ::hpda::extractor::internal::extractor_base<user_item_t> *source)
      : m_source(source){};

  inline stbox::bytes do_parse(const stbox::bytes &param) {
    LOG(INFO) << "do parse";
    hpda::processor::internal::filter_impl<user_item_t> match(
        m_source, [&](const user_item_t &v) { return false; });
    hpda::output::internal::memory_output_impl<user_item_t> mo(&match);
    mo.run();
    uint32_t resp_size = ypc::utc::max_item_size;
    char *resp;
    ff::scope_guard _resp_desc([&]() {
      resp = new char[resp_size]; },
                               [&]() {
      delete[] resp; });
    uint32_t ret = stbox::ocall_cast<uint32_t>(ocall_http_request)(
        (const char *)param.data(), param.size(), resp, resp_size);
    if (ret) {
      return stbox::bytes();
    }
    LOG(INFO) << "do parse done";

    stbox::printf("resp_size: %d\n", strlen(resp));
    stbox::printf("resp: %s\n", resp);
    stbox::bytes result(resp, strlen(resp));
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
