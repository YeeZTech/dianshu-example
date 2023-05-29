#pragma once
#include <ff/util/ntobject.h>
#include <string>
define_nt(id, std::string);
define_nt(shxydm, std::string);
define_nt(qymc, std::string);
define_nt(hylx, std::string);
define_nt(qylx, std::string);
define_nt(jxmc, std::string);
define_nt(zcdz, std::string);
define_nt(jydz, std::string);
typedef ff::util::ntobject<id, shxydm, qymc, hylx, qylx, jxmc, zcdz, jydz>
    t_org_info_item_t;

define_nt(tax, float);
define_nt(qjsr, float);
define_nt(sjrq, std::string);
define_nt(zzjgdm, std::string);
typedef ff::util::ntobject<id, shxydm, tax, qjsr, sjrq, zzjgdm> t_tax_item_t;

define_nt(qylx_dl_id, std::string);
define_nt(qylx_dl_name, std::string);
define_nt(qylx_xl_id, std::string);
define_nt(qylx_xl_name, std::string);
define_nt(qylx_name, std::string);
typedef ff::util::ntobject<id, qylx_dl_id, qylx_dl_name, qylx_xl_id,
                           qylx_xl_name, qylx_name>
    dic_ent_type_item_t;

define_nt(hylx_ml_id, std::string);
define_nt(hylx_ml_name, std::string);
define_nt(hylx_dl_id, std::string);
define_nt(hylx_dl_name, std::string);
define_nt(hylx_zl_id, std::string);
define_nt(hylx_zl_name, std::string);
define_nt(hylx_xl_id, std::string);
define_nt(hylx_xl_name, std::string);
typedef ff::util::ntobject<id, hylx_ml_id, hylx_ml_name, hylx_dl_id,
                           hylx_dl_name, hylx_zl_id, hylx_zl_name, hylx_xl_id,
                           hylx_xl_name>
    dic_industry_item_t;

define_nt(jx_id, std::string);
define_nt(jx_name, std::string);
typedef ff::util::ntobject<id, jx_id, jx_name> dic_region_item_t;

define_nt(tax_l, float);
define_nt(qjsr_l, float);
typedef ff::util::ntobject<
    id, shxydm, qymc, hylx, qylx, jxmc, zcdz, jydz, tax, qjsr, sjrq, zzjgdm,
    qylx_dl_id, qylx_dl_name, qylx_xl_id, qylx_xl_name, qylx_name, hylx_ml_id,
    hylx_ml_name, hylx_dl_id, hylx_dl_name, hylx_zl_id, hylx_zl_name,
    hylx_xl_id, hylx_xl_name, jx_id, jx_name, tax_l, qjsr_l>
    summary_org_info_item_t;
