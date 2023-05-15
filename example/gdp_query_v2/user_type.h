#pragma once
#include <ff/util/ntobject.h>
#include <string>
define_nt(county_name, std::string);
define_nt(area_code, std::string);
define_nt(year, std::string);
define_nt(gdp, std::string);
typedef ff::util::ntobject<county_name, area_code, year, gdp> gdp_query_item_t;
