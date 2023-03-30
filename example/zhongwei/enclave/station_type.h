#pragma once
#include <ff/util/ntobject.h>
#include <string>
define_nt(ULI, std::string);
define_nt(LAC, std::string);
define_nt(longitude, std::string);
define_nt(latitude, std::string);
define_nt(position, std::string);
define_nt(area_code, std::string);
define_nt(area_name, std::string);
typedef ff::util::ntobject<ULI, LAC, longitude, latitude, position, area_code,
                           area_name>
    station_item_t;
