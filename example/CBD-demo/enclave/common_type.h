#pragma once
#include <ff/net/middleware/ntpackage.h>
#include <ff/util/ntobject.h>
#include <string>
define_nt(id, int);
define_nt(company_code, std::string);
define_nt(company_name, std::string);
define_nt(industry_name, std::string);
define_nt(company_type, std::string);
define_nt(located_in, std::string);
define_nt(register_address, std::string);
define_nt(working_address, std::string);
typedef ff::util::ntobject<id, company_code, company_name, industry_name,
                           company_type, located_in, register_address,
                           working_address>
    company_item_t;

define_nt(tax_income, float);
define_nt(district_income, float);
define_nt(year_month, std::string);
typedef ff::util::ntobject<id, company_code, tax_income, district_income,
                           year_month>
    tax_item_t;

typedef ff::util::ntobject<
    id, company_code, company_name, industry_name, company_type, located_in,
    register_address, working_address, tax_income, district_income, year_month>
    summary_item_t;

define_nt(summary_table_t, std::vector<summary_item_t>);
typedef ff::net::ntpackage<0, summary_table_t> summary_pkg_t;

define_nt(industry_id, char);
define_nt(industry_major_id, int);
define_nt(industry_major_name, std::string);
define_nt(industry_minor_id, int);
define_nt(industry_minor_name, std::string);
define_nt(industry_patch_id, int);
define_nt(industry_patch_name, std::string);
typedef ff::util::ntobject<industry_id, industry_name, industry_major_id,
                           industry_major_name, industry_minor_id,
                           industry_minor_name, industry_patch_id,
                           industry_patch_name>
    industry_item_t;

define_nt(company_count, int);
define_nt(company_tax_count, int);
typedef ff::util::ntobject<industry_name, company_count, company_tax_count,
                           tax_income, district_income>
    industry_summary_item_t;
define_nt(industry_summary_table_t, std::vector<industry_summary_item_t>);
typedef ff::net::ntpackage<1, industry_summary_table_t> industry_summary_pkg_t;

define_nt(company_major_id, int);
define_nt(company_major_name, std::string);
define_nt(company_minor_id, int);
define_nt(company_minor_name, std::string);
define_nt(company_label_name, std::string);
typedef ff::util::ntobject<company_major_id, company_major_name,
                           company_minor_id, company_minor_name,
                           company_label_name>
    company_label_item_t;

define_nt(county_id, int);
define_nt(county_name, std::string);
typedef ff::util::ntobject<county_id, county_name> county_item_t;

define_nt(orgnization_code, std::string);
// clang-format off
typedef ff::util::ntobject<id, company_code, orgnization_code, company_name, register_address, working_address,
                           company_major_id, company_major_name, company_minor_id, company_minor_name, company_type,
                           industry_id, industry_name, industry_major_id, industry_minor_id, industry_minor_name, industry_patch_id, industry_patch_name,
                           county_id, located_in,
                           tax_income, district_income
                           >
    cbd_joined_item_t;
// clang-format on
