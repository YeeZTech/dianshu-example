#pragma once
#include <ff/util/ntobject.h>
#include <string>

define_nt(first_item, std::string);
define_nt(others, std::string);

typedef ff::util::ntobject<first_item, others> csv_item_t;
typedef csv_item_t user_item_t;
