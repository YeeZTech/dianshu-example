#pragma once
#include <ff/util/ntobject.h>
#include <string>
define_nt(msisdn, std::string);
define_nt(msisdn_hash, std::string);
typedef ff::util::ntobject<msisdn, msisdn_hash> msisdn_item_t;
