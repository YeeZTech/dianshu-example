#include "user_type.h"
#include "ypc/core_t/analyzer/data_source.h"
#include "ypc/stbox/ebyte.h"
#include "ypc/stbox/stx_common.h"
#include "ypc/stbox/tsgx/log.h"

#include "ypc/corecommon/to_type.h"
#include <hpda/extractor/raw_data.h>
#include <hpda/output/memory_output.h>
#include <hpda/processor/query/filter.h>

class ip_search_street {
public:
  ip_search_street() {}
  ip_search_street(ypc::data_source<stbox::bytes> *source) : m_source(source) {}

  inline stbox::bytes do_parse(const stbox::bytes &param) {
    LOG(INFO) << "do parse";
    ypc::to_type<stbox::bytes, user_item_t> converter(m_source);
    hpda::processor::internal::filter_impl<user_item_t> match(
        &converter, [&](const user_item_t &v) { return false; });
    hpda::output::internal::memory_output_impl<user_item_t> mo(&match);
    mo.get_engine()->run();
    uint32_t resp_size = ypc::utc::max_item_size;
    char *resp;
    ff::scope_guard _resp_desc([&]() { resp = new char[resp_size]; },
                               [&]() { delete[] resp; });

    std::string service("IP_SEARCH_STREET");
    // uint32_t ret = stbox::ocall_cast<uint32_t>(ocall_http_request)(
    //&service[0], service.size(), (const char *)param.data(), param.size(),
    // resp, resp_size);
    // if (ret) {
    // return stbox::bytes();
    //}
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
  ypc::data_source<stbox::bytes> *m_source;
};
