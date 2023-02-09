
#include "user_type.h"
#include "ypc/core_t/analyzer/data_source.h"
#include "ypc/corecommon/to_type.h"
#include "ypc/stbox/ebyte.h"
#include "ypc/stbox/stx_common.h"
#include "ypc/stbox/tsgx/log.h"

#include <hpda/extractor/raw_data.h>
#include <hpda/output/memory_output.h>
#include <hpda/processor/query/filter.h>

#include <sgx_tcrypto.h>
#include <sgx_trts.h>

class person_tel_parser {
public:
  person_tel_parser(ypc::data_source<stbox::bytes> *source) : m_source(source) {
    m_tel_starts = {"139", "138", "137", "136", "135", "134",
                    "159", "150", "151", "158", "157", "188",
                    "187", "152", "182", "147", "178"};
  }

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
    ypc::to_type<stbox::bytes, china_mobile_item_t> converter(m_source);
    hpda::processor::internal::filter_impl<china_mobile_item_t> match(
        &converter, [&](const china_mobile_item_t &v) { return false; });

    hpda::output::internal::memory_output_impl<china_mobile_item_t> mo(&match);
    mo.get_engine()->run();
    LOG(INFO) << "do parse done";

    stbox::bytes result("风险区域驻留和漫出人员号码分析:\n");
    int counter = 100;
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
  std::vector<std::string> m_tel_starts;
  ypc::data_source<stbox::bytes> *m_source;
};
