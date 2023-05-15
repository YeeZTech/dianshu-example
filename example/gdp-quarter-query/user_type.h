#pragma once
#include <ff/util/ntobject.h>
#include <string>
define_nt(province_name, std::string);
define_nt(area_code, std::string);
define_nt(gdp_quarters, std::string);
typedef ff::util::ntobject<province_name, area_code, gdp_quarters>
    gdp_quarter_query_item_t;
