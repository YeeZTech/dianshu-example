#pragma once
#include <ff/util/ntobject.h>
#include <string>

define_nt(oneline, std::string);

typedef ff::util::ntobject<oneline> line_t;
typedef line_t user_item_t;
