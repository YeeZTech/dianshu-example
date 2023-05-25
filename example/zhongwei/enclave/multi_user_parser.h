#include "user_type.h"
#include "ypc/core_t/analyzer/data_source.h"
#include "ypc/corecommon/data_source.h"
#include "ypc/corecommon/package.h"
#include "ypc/corecommon/to_type.h"
#include "ypc/stbox/ebyte.h"
#include "ypc/stbox/stx_common.h"
#include "ypc/stbox/tsgx/log.h"

#include <hpda/extractor/raw_data.h>
#include <hpda/output/memory_output.h>
#include <hpda/processor/query/filter.h>
#include <hpda/processor/set/intersection.h>
#include <hpda/processor/transform/concat.h>

#include <sgx_tcrypto.h>
#include <sgx_trts.h>

define_nt(input_buf, std::string);
typedef ff::net::ntpackage<0, input_buf> input_buf_t;

class multi_user_parser {
public:
  multi_user_parser() {}
  multi_user_parser(
      std::vector<std::shared_ptr<ypc::data_source_with_dhash>> &source)
      : m_datasources(source) {
  };

  inline stbox::bytes do_parse(const stbox::bytes &param) {
    LOG(INFO) << "do parse";
    LOG(INFO) << "param: " << param;
    auto pkg = ypc::make_package<input_buf_t>::from_bytes(param);
    LOG(INFO) << "make package done, input_buf: " << pkg.get<::input_buf>();

    typedef ypc::nt<stbox::bytes> ntt;
    hpda::processor::concat<ntt::data> concator(m_datasources[0].get());
    for (size_t i = 1; i < m_datasources.size(); i++) {
      concator.add_upper_stream(m_datasources[i].get());
    }
    LOG(INFO) << "add all upper streams";
    ypc::to_type<stbox::bytes, user_item_t> converter(&concator);
    hpda::processor::internal::filter_impl<user_item_t> match(
        &converter, [&](const user_item_t &v) {
          std::string tel = v.get<::ts>();
          if (tel == pkg.get<input_buf>()) {
            return true;
          }
          return false;
        });
    hpda::output::internal::memory_output_impl<user_item_t> mo(&match);
    LOG(INFO) << "start to run";
    mo.get_engine()->run();
    LOG(INFO) << "do parse done";

    stbox::bytes result;
    bool flag = false;
    for (auto &it : mo.values()) {
      flag = true;
      result += it.get<::ts>();
      result += ",";
      result += it.get<::tel>();
      result += ",";
      result += it.get<::uli>();
      result += ",";
      result += it.get<::from>();
      result += ",";
      result += it.get<::code>();
      result += ",";
      result += it.get<::lng>();
      result += ",";
      result += it.get<::lat>();
      result += ",";
      result += it.get<::desc>();
      result += "\n";
    }
    if (!flag) {
      result = stbox::bytes("not found!");
    }
    return result;
  }

protected:
  std::vector<std::shared_ptr<ypc::data_source_with_dhash>> m_datasources;
};
