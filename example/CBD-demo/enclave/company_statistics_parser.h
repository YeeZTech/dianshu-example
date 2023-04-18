#include "common_type.h"
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

class company_statistics_parser {
public:
  company_statistics_parser() {}
  company_statistics_parser(ypc::data_source<stbox::bytes> *source)
      : m_source(source){};

  inline stbox::bytes do_parse(const stbox::bytes &param) {
    LOG(INFO) << "do parse";
    auto pkg = ypc::make_package<summary_pkg_t>::from_bytes(param);
    std::vector<summary_item_t> company_list = pkg.get<summary_table_t>();

    ypc::to_type<stbox::bytes, industry_item_t> converter(m_source);
    hpda::processor::internal::filter_impl<industry_item_t> match(
        &converter, [&](const industry_item_t &v) { return true; });
    hpda::output::internal::memory_output_impl<industry_item_t> mo(&match);
    mo.get_engine()->run();
    LOG(INFO) << "do parse done";

    std::vector<industry_summary_item_t> s;
    industry_summary_pkg_t p;
    for (auto &c : company_list) {
      auto industry_name = c.get<::industry_name>();
      for (auto &it : mo.values()) {
        auto patch_name = it.get<::industry_patch_name>();
        if (patch_name == industry_name) {
          auto name = it.get<::industry_name>();
          bool flag = false;
          for (auto &v : s) {
            if (v.get<::industry_name>() == name) {
              v.set<::company_count>(v.get<::company_count>() + 1);
              if (c.get<::tax_income>() > 0) {
                v.set<::company_tax_count>(v.get<::company_tax_count>() + 1);
              }
              v.set<::tax_income>(v.get<::tax_income>() +
                                  c.get<::tax_income>());
              v.set<::district_income>(v.get<::district_income>() +
                                       c.get<::district_income>());
              flag = true;
            }
          }
          if (!flag) {
            industry_summary_item_t item;
            item.set<::industry_name>(name);
            item.set<::company_count>(1);
            item.set<::company_tax_count>(0);
            if (c.get<::tax_income>() > 0) {
              item.set<::company_tax_count>(1);
            }
            item.set<::tax_income>(c.get<::tax_income>());
            item.set<::district_income>(c.get<::district_income>());
            s.push_back(item);
          }
          break;
        }
      }
    }
    p.set<industry_summary_table_t>(s);
    return ypc::make_bytes<stbox::bytes>::for_package(p);
  }

protected:
  ypc::data_source<stbox::bytes> *m_source;
};
