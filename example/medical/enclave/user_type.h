#pragma once
#include <ff/util/ntobject.h>
#include <string>
define_nt(Department, std::string);
define_nt(Title, std::string);
define_nt(Ask, std::string);
define_nt(Answer, std::string);
typedef ff::util::ntobject<Department, Title, Ask, Answer> erke_fixed_item_t;
