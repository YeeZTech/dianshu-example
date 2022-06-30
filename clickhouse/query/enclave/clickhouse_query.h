#include "eparser_t.h" // ocall_http_request
#include "ypc/common/limits.h"
#include "ypc/corecommon/package.h"
#include "ypc/stbox/ebyte.h"
#include "ypc/stbox/scope_guard.h"
#include "ypc/stbox/tsgx/log.h"
#include "ypc/stbox/tsgx/ocall.h"
#include <string.h>

define_nt(msisdn, std::string);
typedef ff::net::ntpackage<0, msisdn> msisdn_t;

class clickhouse_query {
public:
  clickhouse_query() {}

  inline stbox::bytes do_parse(const stbox::bytes &param) {
    LOG(INFO) << "param size: " << param.size();
    LOG(INFO) << "param: " << param;
    // auto pkg = ypc::make_package<msisdn_t>::from_bytes(param);
    // std::string req = pkg.get<msisdn>();
    std::string req((char *)param.data(), param.size());
    LOG(INFO) << "req: " << req;
    uint32_t resp_size = ypc::utc::max_item_size;
    char *resp;
    ff::scope_guard _resp_desc([&]() { resp = new char[resp_size]; },
                               [&]() { delete[] resp; });

    uint32_t ret = stbox::ocall_cast<uint32_t>(ocall_clickhouse_http_query)(
        req.c_str(), req.size(), resp, resp_size);
    if (ret) {
      return stbox::bytes("error");
    }
    stbox::printf("resp_size: %d\n", strlen(resp));
    stbox::printf("resp: %s\n", resp);
    if (!strlen(resp)) {
      return stbox::bytes("result is empty!");
    }
    stbox::bytes result(resp, strlen(resp));
    // LOG(INFO) << "result: " << result;
    return result;
  }

};
