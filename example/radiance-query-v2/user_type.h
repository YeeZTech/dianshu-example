#pragma once
#include <ff/util/ntobject.h>
#include <string>
define_nt(name, std::string);
define_nt(year_month, std::string);
define_nt(level, std::string);
define_nt(center_longitude, std::string);
define_nt(center_latitude, std::string);
define_nt(code, std::string);
define_nt(parent_code, std::string);
define_nt(area, std::string);
define_nt(rad, std::string);
define_nt(rad_per_area, std::string);
typedef ff::util::ntobject< name, year_month, level, center_longitude, center_latitude, code, parent_code, area, rad, rad_per_area> radiance_query_v2_item_t;
