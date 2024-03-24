#include "user_type.h"
#include "ypc/core_t/analyzer/data_source.h"
#include "ypc/stbox/ebyte.h"
#include "ypc/stbox/stx_common.h"
#include "ypc/stbox/tsgx/log.h"
#include "../json.hpp"

#include "ypc/corecommon/to_type.h"
#include <hpda/extractor/raw_data.h>
#include <hpda/output/memory_output.h>
#include <hpda/processor/query/filter.h>

define_nt(input_param, std::string);
typedef ff::net::ntpackage<0, input_param> nt_package_t;

class txt_evaluate_parser {
public:
  txt_evaluate_parser(
      std::vector<std::shared_ptr<ypc::data_source_with_dhash>> &source)
      : m_datasources(source) {}

  inline stbox::bytes do_parse(const stbox::bytes &param) {
    ypc::to_type<stbox::bytes, txt_batch_item_t> converter(
        m_datasources[0].get());

    hpda::processor::internal::filter_impl<txt_batch_item_t> match(
        &converter, [&](const txt_batch_item_t &v) {
          return true;
        });
    hpda::output::internal::memory_output_impl<txt_batch_item_t> mo(&match);
    mo.get_engine()->run();

    std::string s;
    for (auto &it : mo.values()) {
      s += it.get<::txt_batch>();
    }
    LOG(INFO) << "utf8 len: " << s.size();
    auto u32s = to_u32string((uint8_t *)s.c_str(), s.size());

    json::JSON res;
    res["total"] = std::to_string(u32s.size());
    handle_txt_evaluate(u32s, res);
    return stbox::bytes(res.dump());
  }

protected:
  std::string from_u32string(char32_t c) {
    std::string s;
    if (c <= 0x7F) {
      s += static_cast<char>(c & 0x7F);
    } else if (c <= 0x7FF) {
      s += static_cast<char>(0xC0 | ((c >> 6) & 0x1F));
      s += static_cast<char>(0x80 | (c & 0x3F));
    } else if (c <= 0xFFFF) {
      s += static_cast<char>(0xE0 | ((c >> 12) & 0x0F));
      s += static_cast<char>(0x80 | ((c >> 6) & 0x3F));
      s += static_cast<char>(0x80 | (c & 0x3F));
    } else if (c <= 0x10FFFF) {
      s += static_cast<char>(0xF0 | ((c >> 18) & 0x07));
      s += static_cast<char>(0x80 | ((c >> 12) & 0x3F));
      s += static_cast<char>(0x80 | ((c >> 6) & 0x3F));
      s += static_cast<char>(0x80 | (c & 0x3F));
    }
    return s;
  }
  std::string get_u32s_interval(const std::u32string &u32s, size_t idx_s,
                                size_t idx_e) {
    std::string s;
    for (size_t i = idx_s; i < idx_e; i++) {
      s += from_u32string(u32s[i]);
    }
    return s;
  }
  void handle_txt_evaluate_with_offset(const std::u32string &u32s,
                                       json::JSON &res, size_t head_s,
                                       size_t head_e, size_t mid_s,
                                       size_t mid_e, size_t tail_s,
                                       size_t tail_e) {
    res["head"] = get_u32s_interval(u32s, head_s, head_e);
    res["mid"] = get_u32s_interval(u32s, mid_s, mid_e);
    res["tail"] = get_u32s_interval(u32s, tail_s, tail_e);
  }
  void handle_txt_evaluate(const std::u32string &u32s, json::JSON &res) {
    size_t words_cnt = u32s.size();
    size_t actual_words_cnt = std::min(size_t(150), size_t(0.1 * words_cnt));
    handle_txt_evaluate_with_offset(u32s, res, 0, actual_words_cnt,
                                    words_cnt / 2 - actual_words_cnt,
                                    words_cnt / 2 + actual_words_cnt,
                                    words_cnt - actual_words_cnt, words_cnt);
  }

  std::u32string to_u32string(uint8_t *arr, size_t len) {
    char32_t val = 0;
    std::vector<char32_t> v;
    for (size_t i = 0; i < len;) {
      char32_t val = 0;
      if ((arr[i] & 0x80) == 0x00) {
        val = arr[i++];
      } else if ((arr[i] & 0xE0) == 0xC0) {
        val = ((arr[i++] & 0x1F) << 6) | (arr[i++] & 0x3F);
      } else if ((arr[i] & 0xF0) == 0xE0) {
        val = ((arr[i++] & 0x0F) << 12) | ((arr[i++] & 0x3F) << 6) |
              (arr[i++] & 0x3F);
      } else if ((arr[i] & 0xF8) == 0xF0) {
        val = ((arr[i++] & 0x07) << 18) | ((arr[i++] & 0x3F) << 12) |
              ((arr[i++] & 0x3F) << 6) | (arr[i++] & 0x3F);
      }
      v.push_back(val);
    }
    return std::u32string(v.begin(), v.end());
  }

protected:
  std::vector<std::shared_ptr<ypc::data_source_with_dhash>> m_datasources;
};
