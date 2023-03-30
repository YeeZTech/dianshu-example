#pragma once
#include <ff/util/ntobject.h>
#include <string>
define_nt(ts, std::string);
define_nt(tel, std::string);
define_nt(uli, std::string);
define_nt(from, std::string);
define_nt(code, std::string);
define_nt(lng, std::string);
define_nt(lat, std::string);
define_nt(desc, std::string);
typedef ff::util::ntobject<ts, tel, uli, from, code, lng, lat, desc>
    user_item_t;
