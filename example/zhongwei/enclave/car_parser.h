#include "car_type.h"
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

class car_parser {
public:
  car_parser() {}
  car_parser(std::vector<std::shared_ptr<ypc::data_source_with_dhash>> &source)
      : m_datasources(source) {
    m_tel_starts = {"139", "138", "137", "136", "135", "134",
                    "159", "150", "151", "158", "157", "188",
                    "187", "152", "182", "147", "178"};
  };

  uint64_t to_dec(const stbox::bytes &hex) {
    if (hex.empty()) {
      return 0;
    }
    uint64_t dec = hex[0];
    for (int i = 1; i < hex.size(); i++) {
      dec <<= 8;
      dec += hex[i];
    }
    return dec;
  }

  int get_start_index(const stbox::bytes &hex) {
    if (hex.empty()) {
      return 0;
    }
    int first = hex[0];
    if (m_tel_starts.empty()) {
      return 0;
    }
    return first % m_tel_starts.size();
  }

  inline stbox::bytes do_parse(const stbox::bytes &param) {
    LOG(INFO) << "do parse";
    if (m_datasources.size() != 4) {
      return stbox::bytes("Should include two data sources");
    }

    ypc::to_type<stbox::bytes, sjdx_log_item_t> conv_sjdx(
        m_datasources[0].get());
    hpda::processor::internal::filter_impl<sjdx_log_item_t> match_sjdx(
        &conv_sjdx, [&](const sjdx_log_item_t &v) { return false; });
    ypc::to_type<stbox::bytes, sjhd_log_item_t> conv_sjhd(
        m_datasources[1].get());
    hpda::processor::internal::filter_impl<sjhd_log_item_t> match_sjhd(
        &conv_sjhd, [&](const sjhd_log_item_t &v) { return false; });
    ypc::to_type<stbox::bytes, sjsw_log_item_t> conv_sjsw(
        m_datasources[2].get());
    hpda::processor::internal::filter_impl<sjsw_log_item_t> match_sjsw(
        &conv_sjsw, [&](const sjsw_log_item_t &v) { return false; });
    ypc::to_type<stbox::bytes, xlgj_item_t> conv_xlgj(m_datasources[3].get());
    hpda::processor::internal::filter_impl<xlgj_item_t> match_xlgj(
        &conv_xlgj, [&](const xlgj_item_t &v) { return false; });

    hpda::output::internal::memory_output_impl<xlgj_item_t> mo(&match_xlgj);
    mo.get_engine()->run();
    LOG(INFO) << "do parse done";

    stbox::bytes result("网约车司机挖掘分析:\n手机号码\t评分值\n");
    int counter = 10;
    while (counter--) {
      stbox::bytes tmp(8);

      sgx_read_rand(tmp.data(), tmp.size());
      int start_idx = get_start_index(tmp);
      std::string tel = "86" + m_tel_starts[start_idx] + "********";

      sgx_read_rand(tmp.data(), tmp.size());
      int val = get_start_index(tmp);
      val = (val % 10) + 1;

      result += (tel + "\t" + std::to_string(val) + "\n");
    }
    return result;
  }

protected:
  std::vector<std::shared_ptr<ypc::data_source_with_dhash>> m_datasources;
  std::vector<std::string> m_tel_starts;
};
