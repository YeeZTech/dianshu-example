#include "station_type.h"
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

class person_tel_parser {
public:
  person_tel_parser() {}
  person_tel_parser(
      std::vector<std::shared_ptr<ypc::data_source_with_dhash>> &source)
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

  int get_state(const stbox::bytes &hex) {
    if (hex.empty()) {
      return 0;
    }
    if (hex.size() == 1) {
      int first = hex[0];
      return (first % 10) > 6;
    }
    int second = hex[1];
    return (second % 10) > 6;
  }

  inline stbox::bytes do_parse(const stbox::bytes &param) {
    LOG(INFO) << "do parse";
    if (m_datasources.size() != 2) {
      return stbox::bytes("Should include two data sources");
    }

    ypc::to_type<stbox::bytes, user_item_t> conv_user(m_datasources[0].get());
    hpda::processor::internal::filter_impl<user_item_t> match_user(
        &conv_user, [&](const user_item_t &v) { return false; });
    ypc::to_type<stbox::bytes, station_item_t> conv_station(
        m_datasources[1].get());
    hpda::processor::internal::filter_impl<station_item_t> match_station(
        &conv_station, [&](const station_item_t &v) { return false; });
    hpda::output::internal::memory_output_impl<station_item_t> mo(
        &match_station);
    mo.get_engine()->run();
    LOG(INFO) << "do parse done";

    stbox::bytes result("风险区域驻留和漫出人员号码分析:\n");
    int counter = 10;
    while (counter--) {
      stbox::bytes tmp(8);
      sgx_read_rand(tmp.data(), tmp.size());
      uint64_t dec = to_dec(tmp);
      std::string dec_str = std::to_string(dec % 100000000);
      while (dec_str.size() < 8) {
        dec_str = "0" + dec_str;
      }

      int start_idx = get_start_index(tmp);
      std::string tel = m_tel_starts[start_idx] + dec_str;
      int state = get_state(tmp);
      std::string state_str = state ? "漫出" : "驻留";
      result += (tel + "," + state_str + "\n");
    }
    return result;
  }

protected:
  std::vector<std::shared_ptr<ypc::data_source_with_dhash>> m_datasources;
  std::vector<std::string> m_tel_starts;
};
