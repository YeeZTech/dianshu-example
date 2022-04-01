#pragma once
#include <ff/util/ntobject.h>
#include <string>

define_nt(date_item, std::string);
define_nt(region_item, std::string);
define_nt(total_r, std::string);
define_nt(sample_cnt, std::string);

typedef ff::util::ntobject<date_item, region_item, total_r, sample_cnt>
    csv_item_t;
typedef csv_item_t user_item_t;
