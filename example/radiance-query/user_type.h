#pragma once
#include <ff/util/ntobject.h>
#include <string>
define_nt(year_month, std::string);
define_nt(name, std::string);
define_nt(area, std::string);
define_nt(rad, std::string);
define_nt(rad_perarea, std::string);
typedef ff::util::ntobject<year_month, name, area, rad, rad_perarea> radiance_query_item_t;
