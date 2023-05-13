#pragma once
#include <ff/util/ntobject.h>
#include <string>
define_nt(name, std::string);
define_nt(area_code, std::string);
define_nt(gdp, std::string);
typedef ff::util::ntobject<name, area_code, gdp> gdp_prediction_item_t;
