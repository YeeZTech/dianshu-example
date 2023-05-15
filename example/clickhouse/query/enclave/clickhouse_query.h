#include "eparser_t.h" // ocall_http_request
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
#include <string.h>
#include <unordered_map>

#include "user_type.h"

// typedef ff::net::ntpackage<0, msisdn> nt_package_t;

class clickhouse_query {
public:
  clickhouse_query(ypc::data_source<stbox::bytes> *source) : m_source(source){};

  inline stbox::bytes do_parse(const stbox::bytes &param) {
    LOG(INFO) << "param size: " << param.size();
    LOG(INFO) << "param: " << param;
    ypc::to_type<stbox::bytes, msisdn_item_t> converter(m_source);

    // auto pkg = ypc::make_package<nt_package_t>::from_bytes(param);
    std::string req((char *)param.data(), param.size());
    LOG(INFO) << "req: " << req;
    int counter = 0;
    if (m_index.empty()) {
      LOG(INFO) << "indexing...";
      hpda::processor::internal::filter_impl<msisdn_item_t> match(
          &converter, [&](const msisdn_item_t &v) {
            counter++;
            m_index.insert(
                std::make_pair(v.get<msisdn>(), v.get<msisdn_hash>()));
            return false;
          });
      hpda::output::internal::memory_output_impl<msisdn_item_t> mo(&match);
      mo.get_engine()->run();
    }
    LOG(INFO) << "do parse done";

    if (m_index.find(req) != m_index.end()) {
      LOG(INFO) << "found!";
      return stbox::bytes("found\n");
    }
    LOG(INFO) << "not found!";
    return stbox::bytes("not found\n");
  }

protected:
  ypc::data_source<stbox::bytes> *m_source;
  std::unordered_map<std::string, std::string> m_index;
};

