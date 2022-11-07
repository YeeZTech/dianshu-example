#pragma once
#include <ff/util/ntobject.h>
#include <string>
define_nt(province_name, std::string);
define_nt(gdp_quarters, std::string);
typedef ff::util::ntobject< province_name, gdp_quarters > gdp_quarter_query_item_t;
