#include "cbd_parser_t.h"
#include "common_type.h"
#include "ypc/common/crypto_prefix.h"
#include "ypc/core_t/analyzer/data_source.h"
#include "ypc/corecommon/crypto/stdeth.h"
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

using ecc = ypc::crypto::eth_sgx_crypto;

namespace internal {
template <typename T> struct join_helper {};
template <> struct join_helper<dic_ent_type_item_t> {
  static void join(const t_org_info_item_t &org_info,
                   const std::vector<dic_ent_type_item_t> &v,
                   summary_org_info_item_t &item) {
    auto qylx = org_info.get<::qylx>();
    for (const auto &it : v) {
      auto qylx_name = it.get<::qylx_name>();
      if (qylx == qylx_name) {
        item = it;
        break;
      }
    }
  }
};
template <> struct join_helper<dic_industry_item_t> {
  static void join(const t_org_info_item_t &org_info,
                   const std::vector<dic_industry_item_t> &v,
                   summary_org_info_item_t &item) {
    auto hylx = org_info.get<::hylx>();
    for (const auto &it : v) {
      auto hylx_name = it.get<::hylx_xl_name>();
      if (hylx == hylx_name) {
        item = it;
        break;
      }
    }
  }
};
template <> struct join_helper<dic_region_item_t> {
  static void join(const t_org_info_item_t &org_info,
                   const std::vector<dic_region_item_t> &v,
                   summary_org_info_item_t &item) {
    auto jxmc = org_info.get<::jxmc>();
    for (const auto &it : v) {
      auto jx_name = it.get<::jx_name>();
      if (jxmc == jx_name) {
        item = it;
        break;
      }
    }
  }
};

void init_summary_org_info_v(
    const std::vector<t_org_info_item_t> &t_org_info_v,
    std::vector<summary_org_info_item_t> &summary_org_info_v) {
  for (const auto &it : t_org_info_v) {
    summary_org_info_v.push_back(it);
  }
}

void calculate_tax_sum(
    const std::vector<t_tax_item_t> &t_tax_v,
    std::vector<summary_org_info_item_t> &summary_org_info_v) {
  std::unordered_map<std::string, float> tax_sum;
  std::unordered_map<std::string, float> qjsr_sum;
  for (const auto &it : t_tax_v) {
    auto shxydm = it.get<::shxydm>();
    auto tax = it.get<::tax>();
    auto qjsr = it.get<::qjsr>();
    if (tax_sum.find(shxydm) != tax_sum.end()) {
      tax_sum[shxydm] += tax;
      qjsr_sum[shxydm] += qjsr;
    } else {
      tax_sum.insert(std::make_pair(shxydm, tax));
      qjsr_sum.insert(std::make_pair(shxydm, qjsr));
    }
  }
  for (auto &it : summary_org_info_v) {
    auto shxydm = it.get<::shxydm>();
    it.set<::tax_l>(tax_sum[shxydm]);
    it.set<::qjsr_l>(qjsr_sum[shxydm]);
  }
}

std::string hex_str(const stbox::bytes &b) {
  std::string s;
  for (int i = 0; i < b.size(); i++) {
    uint8_t high, low;
    ypc::utc::internal::to_hex(b[i], &high, &low);
    s += char(high);
    s += char(low);
  }
  return s;
}

std::string hash_hex(const stbox::bytes &b) {
  stbox::bytes hash(32);
  ypc::crypto::eth_sgx_crypto::hash_t::hash_256(b.data(), b.size(),
                                                hash.data());
  return hex_str(hash).substr(0, 32);
}

void encrypt_message(const stbox::bytes &pkey, float msg, stbox::bytes &r) {
  auto se_ret = (sgx_status_t)ecc::encrypt_message_with_prefix(
      pkey, stbox::bytes(std::to_string(msg)),
      ypc::utc::crypto_prefix_arbitrary, r);
  if (se_ret) {
    throw std::runtime_error("encrypt_message_with_prefix failed!");
  }
}

void insert_into_t_summary_org_info(const stbox::bytes &pkey,
                                    const summary_org_info_item_t &it,
                                    std::string &sql) {
  std::string s =
      "insert into t_summary_org_info(`id`, `shxydm`, `qymc`, `hylx`, `qylx`, "
      "`jxmc`, `zcdz`, `jydz`, `zzjgdm`, `qylx_dl_id`, `qylx_dl_mc`, "
      "`hylx_ml_id`, `hylx_ml_name`, `hylx_dl_id`, `hylx_dl_name`, "
      "`hylx_zl_id`, `hylx_zl_name`, `hylx_xl_id`, `hylx_xl_name`, `jx_id`, "
      "`sjrq`, `tax`, `qjsr`, `tax_l`, `qjsr_l`) values ";
  s += ("('" + hash_hex(stbox::bytes(it.get<::shxydm>())));
  s += ("','" + it.get<::shxydm>());
  s += ("','" + (it.get<::qymc>()));
  s += ("','" + (it.get<::hylx>()));
  s += ("','" + (it.get<::qylx>()));
  s += ("','" + it.get<::jxmc>());
  s += ("','" + (it.get<::zcdz>()));
  s += ("','" + (it.get<::jydz>()));
  s += ("','" + it.get<::zzjgdm>());
  s += ("','" + it.get<::qylx_dl_id>());
  s += ("','" + it.get<::qylx_dl_name>());
  s += ("','" + it.get<::hylx_ml_id>());
  s += ("','" + it.get<::hylx_ml_name>());
  s += ("','" + it.get<::hylx_dl_id>());
  s += ("','" + it.get<::hylx_dl_name>());
  s += ("','" + it.get<::hylx_zl_id>());
  s += ("','" + it.get<::hylx_zl_name>());
  s += ("','" + it.get<::hylx_xl_id>());
  s += ("','" + it.get<::hylx_xl_name>());
  s += ("','" + it.get<::jx_id>());
  s += ("','" + it.get<::sjrq>());

  // encrypt tax
  stbox::bytes enc_tax;
  encrypt_message(pkey, it.get<::tax>(), enc_tax);
  // encrypt qjsr
  stbox::bytes enc_qjsr;
  encrypt_message(pkey, it.get<::qjsr>(), enc_qjsr);
  // encrypt tax_l
  stbox::bytes enc_tax_l;
  encrypt_message(pkey, it.get<::tax_l>(), enc_tax_l);
  // encrypt qjsr
  stbox::bytes enc_qjsr_l;
  encrypt_message(pkey, it.get<::qjsr_l>(), enc_qjsr_l);

  s += ("','" + hex_str(enc_tax));
  s += ("','" + hex_str(enc_qjsr));
  s += ("','" + hex_str(enc_tax_l));
  s += ("','" + hex_str(enc_qjsr_l));
  s += "');\n";
  // s += ("'," + std::to_string(it.get<::tax>()));
  // s += ("," + std::to_string(it.get<::qjsr>()));
  // s += ("," + std::to_string(it.get<::tax_l>()));
  // s += ("," + std::to_string(it.get<::qjsr_l>()));
  // s += ");\n";
  sql += s;
}

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

} // namespace internal

class summary_org_info_parser {
public:
  summary_org_info_parser() {}
  summary_org_info_parser(
      std::vector<std::shared_ptr<ypc::data_source_with_dhash>> &source)
      : m_datasources(source){};

  inline stbox::bytes do_parse(const stbox::bytes &param) {
    LOG(INFO) << "do parse";
    if (m_datasources.size() != 5) {
      return stbox::bytes("Should include five data sources");
    }
    LOG(INFO) << "param: " << param;

    // t_org_info
    std::vector<t_org_info_item_t> t_org_info_v;
    ypc::to_type<stbox::bytes, t_org_info_item_t> conv_t_org_info(
        m_datasources[0].get());
    hpda::processor::internal::filter_impl<t_org_info_item_t> match_t_org_info(
        &conv_t_org_info, [&](const t_org_info_item_t &v) {
          t_org_info_v.push_back(v.make_copy());
          return false;
        });
    // t_tax
    std::vector<t_tax_item_t> t_tax_v;
    ypc::to_type<stbox::bytes, t_tax_item_t> conv_t_tax(m_datasources[1].get());
    hpda::processor::internal::filter_impl<t_tax_item_t> match_t_tax(
        &conv_t_tax, [&](const t_tax_item_t &v) {
          t_tax_v.push_back(v.make_copy());
          return false;
        });
    // dic_ent_type
    std::vector<dic_ent_type_item_t> dic_ent_type_v;
    ypc::to_type<stbox::bytes, dic_ent_type_item_t> conv_dic_ent_type(
        m_datasources[2].get());
    hpda::processor::internal::filter_impl<dic_ent_type_item_t>
        match_dic_ent_type(&conv_dic_ent_type,
                           [&](const dic_ent_type_item_t &v) {
                             dic_ent_type_v.push_back(v.make_copy());
                             return false;
                           });
    // dic_industry
    std::vector<dic_industry_item_t> dic_industry_v;
    ypc::to_type<stbox::bytes, dic_industry_item_t> conv_dic_industry(
        m_datasources[3].get());
    hpda::processor::internal::filter_impl<dic_industry_item_t>
        match_dic_industry(&conv_dic_industry,
                           [&](const dic_industry_item_t &v) {
                             dic_industry_v.push_back(v.make_copy());
                             return false;
                           });
    // dic_region
    std::vector<dic_region_item_t> dic_region_v;
    ypc::to_type<stbox::bytes, dic_region_item_t> conv_dic_region(
        m_datasources[4].get());
    hpda::processor::internal::filter_impl<dic_region_item_t> match_dic_region(
        &conv_dic_region, [&](const dic_region_item_t &v) {
          dic_region_v.push_back(v.make_copy());
          return false;
        });
    // dummy intersection
    hpda::processor::internal::ordered_intersection_impl<
        summary_org_info_item_t, std::string>
        oi;
    oi.add_upper_stream<::id>(&match_t_org_info);
    oi.add_upper_stream<::id>(&match_t_tax);
    oi.add_upper_stream<::id>(&match_dic_ent_type);
    oi.add_upper_stream<::id>(&match_dic_industry);
    oi.add_upper_stream<::id>(&match_dic_region);
    hpda::output::internal::memory_output_impl<summary_org_info_item_t> mo(&oi);
    mo.get_engine()->run();
    LOG(INFO) << "t_org_info_v size: " << t_org_info_v.size();
    LOG(INFO) << "t_tax_v size: " << t_tax_v.size();
    LOG(INFO) << "dic_ent_type_v size: " << dic_ent_type_v.size();
    LOG(INFO) << "dic_industry_v size: " << dic_industry_v.size();
    LOG(INFO) << "dic_region_v size: " << dic_region_v.size();

    // init summary_org_info_item_t
    std::vector<summary_org_info_item_t> sum_v;
    internal::init_summary_org_info_v(t_org_info_v, sum_v);
    // join dic_ent_type
    for (int i = 0; i < t_org_info_v.size(); i++) {
      internal::join_helper<dic_ent_type_item_t>::join(
          t_org_info_v[i], dic_ent_type_v, sum_v[i]);
    }
    // join dic_industry
    for (int i = 0; i < t_org_info_v.size(); i++) {
      internal::join_helper<dic_industry_item_t>::join(
          t_org_info_v[i], dic_industry_v, sum_v[i]);
    }
    // join dic_region
    for (int i = 0; i < t_org_info_v.size(); i++) {
      internal::join_helper<dic_region_item_t>::join(t_org_info_v[i],
                                                     dic_region_v, sum_v[i]);
    }
    // calculate tax sum
    internal::calculate_tax_sum(t_tax_v, sum_v);

    std::string sql;
    for (const auto &it : sum_v) {
      internal::insert_into_t_summary_org_info(param, it, sql);
    }
    LOG(INFO) << "do parse done";
    stbox::ocall_cast<uint32_t>(ocall_exec_sql)((uint8_t *)sql.c_str(),
                                                sql.size());
    return stbox::bytes("sql size: " + std::to_string(sql.size()));
  }

protected:
  std::vector<std::shared_ptr<ypc::data_source_with_dhash>> m_datasources;
};
