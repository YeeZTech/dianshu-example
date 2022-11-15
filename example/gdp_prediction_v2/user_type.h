#pragma once
#include <ff/util/ntobject.h>
#include <string>
define_nt(name, std::string);
define_nt(gdp, std::string);
typedef ff::util::ntobject<name, gdp> gdp_prediction_item_t;
