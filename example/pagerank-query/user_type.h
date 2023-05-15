#pragma once
#include <ff/util/ntobject.h>
#include <string>
define_nt( addr, std::string );
define_nt( pr_value, std::string );
define_nt( date_span, std::string );
typedef ff::util::ntobject< addr, pr_value, date_span > pagerank_query_item_t;
