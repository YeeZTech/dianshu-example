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
#include <hpda/processor/set/intersection.h>
#include <hpda/processor/transform/concat.h>

#include <cctype>
#include <sgx_tcrypto.h>
#include <sgx_trts.h>

define_nt(company_label_table, std::vector<stbox::bytes>);
define_nt(industry_table, std::vector<stbox::bytes>);
define_nt(county_table, std::vector<stbox::bytes>);
typedef ff::net::ntpackage<0, company_label_table, industry_table, county_table>
    param_pkg_t;

typedef ypc::cast_obj_to_package<company_label_item_t>::type
    company_label_item_pkg_t;
typedef ypc::cast_obj_to_package<industry_item_t>::type industry_item_pkg_t;
typedef ypc::cast_obj_to_package<county_item_t>::type county_item_pkg_t;

template <typename T> struct company_join_helper {};
template <> struct company_join_helper<company_label_item_pkg_t> {
  static void join(const summary_item_t &item_sum,
                   const std::vector<company_label_item_pkg_t> &v,
                   cbd_joined_item_t &item) {
    auto company_type = item_sum.get<::company_type>();
    item.set<::company_type>(company_type);
    for (const auto &it : v) {
      auto label_name = it.get<::company_label_name>();
      if (company_type == label_name) {
        item.set<::company_major_id>(it.get<::company_major_id>());
        item.set<::company_major_name>(it.get<::company_major_name>());
        item.set<::company_minor_id>(it.get<::company_minor_id>());
        item.set<::company_minor_name>(it.get<::company_minor_name>());
        break;
      }
    }
  }
};
template <> struct company_join_helper<industry_item_pkg_t> {
  static void join(const summary_item_t &item_sum,
                   const std::vector<industry_item_pkg_t> &v,
                   cbd_joined_item_t &item) {
    auto industry_name = item_sum.get<::industry_name>();
    item.set<::industry_patch_name>(industry_name);
    for (const auto &it : v) {
      auto patch_name = it.get<::industry_patch_name>();
      if (industry_name == patch_name) {
        item.set<::industry_id>(it.get<::industry_id>());
        item.set<::industry_name>(it.get<::industry_name>());
        item.set<::industry_major_id>(it.get<::industry_major_id>());
        item.set<::industry_minor_id>(it.get<::industry_minor_id>());
        item.set<::industry_minor_name>(it.get<::industry_minor_name>());
        item.set<::industry_patch_id>(it.get<::industry_patch_id>());
        break;
      }
    }
  }
};
template <> struct company_join_helper<county_item_pkg_t> {
  static void join(const summary_item_t &item_sum,
                   const std::vector<county_item_pkg_t> &v,
                   cbd_joined_item_t &item) {
    auto located_in = item_sum.get<::located_in>();
    item.set<::located_in>(located_in);
    for (const auto &it : v) {
      auto county_name = it.get<::county_name>();
      if (located_in == county_name) {
        item.set<::county_id>(it.get<::county_id>());
        break;
      }
    }
  }
};

void step_3(std::string &s) {
  if (!s.empty() && s != "-") {
    if (s.size() == 18) {
      s = s.substr(8, 9);
    } else if (s.size() == 15) {
      s = s.substr(6);
    } else {
      s = std::string();
    }
  }
}

void upper(std::string &s) {
  for (int i = 0; i < s.size(); i++) {
    s[i] = toupper(s[i]);
  }
}
void replace(std::string &str, const std::string &s, const std::string &t) {
  if (s.empty()) {
    return;
  }
  size_t start_pos = 0;
  while ((start_pos = str.find(s, start_pos)) != std::string::npos) {
    str.replace(start_pos, s.length(), t);
    start_pos += t.length();
  }
}

void step_standard(std::string &s) {
  upper(s);
  replace(s, " ", "");
  replace(s, "＜＜", "《");
  replace(s, "＞＞", "》");
  replace(s, "＜ＮＵＬＬ＞", "");
  replace(s, "〈ＮＵＬＬ〉", "");
  replace(s, "．", "");
  replace(s, "＂", "");
  replace(s, "　", "");
  replace(s, "\r\n", "");
  replace(s, "＊", "");
}

void step_4(std::string &s) {
  step_standard(s);
  replace(s, "＃", "号");
  replace(s, "	北", "北");
}

void step_5(std::string &s) {
  step_standard(s);
  replace(s, "其他会议、展览及相关服务", "其他会议、会展及相关服务");
}

void step_6(std::string &s) { step_standard(s); }
void step_7(std::string &s) { step_standard(s); }
void step_9(std::string &s) { step_standard(s); }

void step_15(std::string &s) { step_3(s); }

class company_join_parser {
public:
  company_join_parser() {}
  company_join_parser(
      std::vector<std::shared_ptr<ypc::data_source_with_dhash>> &source)
      : m_datasources(source){};

  inline stbox::bytes do_parse(const stbox::bytes &param) {
    LOG(INFO) << "do parse";
    if (m_datasources.size() < 2) {
      return stbox::bytes("Should include two data sources at least!");
    }

    // set intersection
    hpda::processor::internal::ordered_intersection_impl<summary_item_t,
                                                         std::string>
        oi;
    ypc::to_type<stbox::bytes, company_item_t> conv_a(m_datasources[0].get());
    oi.add_upper_stream<::company_code>(&conv_a);
    ypc::to_type<stbox::bytes, tax_item_t> conv_b(m_datasources[1].get());
    oi.add_upper_stream<::company_code>(&conv_b);
    hpda::output::internal::memory_output_impl<summary_item_t> mo(&oi);
    mo.get_engine()->run();
    const auto &v_sum = mo.values();

    // init cbd_joined_item_t
    std::vector<cbd_joined_item_t> v_join;
    for (const auto &it : v_sum) {
      cbd_joined_item_t item;
      item.set<::id>(it.get<::id>());
      item.set<::company_code>(it.get<::company_code>());
      item.set<::orgnization_code>(it.get<::company_code>().substr(8, 9));
      item.set<::company_name>(it.get<::company_name>());
      item.set<::register_address>(it.get<::register_address>());
      item.set<::working_address>(it.get<::working_address>());
      item.set<::tax_income>(it.get<::tax_income>());
      item.set<::district_income>(it.get<::district_income>());
      v_join.push_back(item);
    }

    // get table company_label, industry and county
    auto package = ypc::make_package<param_pkg_t>::from_bytes(param);
    auto company_label_table = package.get<::company_label_table>();
    auto industry_table = package.get<::industry_table>();
    auto county_table = package.get<::county_table>();
    LOG(INFO) << "do parse done";

    // construct company_label
    std::vector<company_label_item_pkg_t> v_comlab;
    for (const auto &b : company_label_table) {
      auto p = ypc::make_package<company_label_item_pkg_t>::from_bytes(b);
      v_comlab.push_back(p);
    }
    LOG(INFO) << "v_comlab size: " << v_comlab.size();
    for (int i = 0; i < v_join.size(); i++) {
      company_join_helper<company_label_item_pkg_t>::join(v_sum[i], v_comlab,
                                                          v_join[i]);
    }

    // construct industry
    std::vector<industry_item_pkg_t> v_ind;
    for (const auto &b : industry_table) {
      auto p = ypc::make_package<industry_item_pkg_t>::from_bytes(b);
      v_ind.push_back(p);
    }
    LOG(INFO) << "v_ind size: " << v_ind.size();
    for (int i = 0; i < v_join.size(); i++) {
      company_join_helper<industry_item_pkg_t>::join(v_sum[i], v_ind,
                                                     v_join[i]);
    }

    // construct county
    std::vector<county_item_pkg_t> v_cou;
    for (const auto &b : county_table) {
      auto p = ypc::make_package<county_item_pkg_t>::from_bytes(b);
      v_cou.push_back(p);
    }
    LOG(INFO) << "v_cou size: " << v_cou.size();
    for (int i = 0; i < v_join.size(); i++) {
      company_join_helper<county_item_pkg_t>::join(v_sum[i], v_cou, v_join[i]);
    }

    std::string ret;
    for (const auto &it : v_join) {
      ret += (std::to_string(it.get<::id>()) + ',');
      ret += (it.get<::company_code>() + ',');
      ret += (it.get<::orgnization_code>() + ',');
      ret += (it.get<::company_name>() + ',');
      ret += (it.get<::register_address>() + ',');
      ret += (it.get<::working_address>() + ',');
      ret += (std::to_string(it.get<::company_major_id>()) + ',');
      ret += (it.get<::company_major_name>() + ',');
      ret += (std::to_string(it.get<::company_minor_id>()) + ',');
      ret += (it.get<::company_type>() + ',');
      ret += (std::to_string(it.get<::industry_id>()) + ',');
      ret += (it.get<::industry_name>() + ',');
      ret += (std::to_string(it.get<::industry_major_id>()) + ',');
      ret += (std::to_string(it.get<::industry_minor_id>()) + ',');
      ret += (it.get<::industry_minor_name>() + ',');
      ret += (std::to_string(it.get<::industry_patch_id>()) + ',');
      ret += (it.get<::industry_patch_name>() + ',');
      ret += (std::to_string(it.get<::county_id>()) + ',');
      ret += (it.get<::located_in>() + ',');
      ret += (std::to_string(it.get<::tax_income>()) + ',');
      ret += (std::to_string(it.get<::district_income>()) + '\n');
    }
    // LOG(INFO) << ret;
    return stbox::bytes(ret);
  }

protected:
  std::vector<std::shared_ptr<ypc::data_source_with_dhash>> m_datasources;
};
