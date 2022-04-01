#include "corecommon/package.h"
#include "stbox/ebyte.h"
#include "stbox/stx_common.h"
#include "stbox/tsgx/log.h"
#include "user_type.h"
#include <hpda/extractor/raw_data.h>
#include <hpda/output/memory_output.h>
#include <hpda/processor/query/filter.h>
#include <string.h>

typedef ff::net::ntpackage<0, ::date_item, ::region_item> date_and_region_t;

class date_and_region_search {
public:
  date_and_region_search() {}
  template <typename ET>
  date_and_region_search(
      ::hpda::extractor::internal::extractor_base<user_item_t> *source,
      ET &&ignore)
      : m_source(source){};

  inline stbox::bytes do_parse(const stbox::bytes &param) {
    LOG(INFO) << "do parse";
    auto pkg = ypc::make_package<date_and_region_t>::from_bytes(param);
    auto date = pkg.get<::date_item>();
    auto region = pkg.get<::region_item>();

    int counter = 0;
    hpda::processor::internal::filter_impl<user_item_t> match(
        m_source, [&](const user_item_t &v) {
          counter++;
          if (date == v.get<::date_item>() &&
              region == v.get<::region_item>()) {
            return true;
          }
          return false;
        });

    hpda::output::internal::memory_output_impl<user_item_t> mo(&match);
    mo.get_engine()->run();
    LOG(INFO) << "do parse done";

    stbox::bytes header("The analysis result with parameter, date: " + date +
                        ", region: " + region + " is");
    stbox::bytes result;
    for (auto it : mo.values()) {
      result += std::string("\ntotal_r: ");
      result += it.get<::total_r>();
      result += std::string("\nsample_cnt: ");
      result += it.get<::sample_cnt>();
      result += std::string("\n");
    }
    if (result.empty()) {
      result += std::string("\nnothing");
    }
    return header + result;
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
