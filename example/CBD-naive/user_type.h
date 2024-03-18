#pragma once
#include <ff/net/middleware/ntpackage.h>
#include <ff/util/ntobject.h>
#include <string>
define_nt(id, int);
define_nt(company_code, std::string);
define_nt(tax_income, float);
define_nt(district_income, float);
define_nt(year_month, std::string);
define_nt(city, std::string);
typedef ff::util::ntobject<id,company_code,tax_income,district_income,year_month> tax_item_t;
