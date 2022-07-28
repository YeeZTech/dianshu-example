#pragma once
#include <ff/util/ntobject.h>
#include <string>
define_nt(county_name, std::string);
define_nt(year, std::string);
define_nt(gdp, std::string);
define_nt(gdp_1, std::string);
define_nt(gdp_2, std::string);
define_nt(area, std::string);
typedef ff::util::ntobject<county_name, year, gdp, gdp_1, gdp_2, area> gdp_query_item_t;
