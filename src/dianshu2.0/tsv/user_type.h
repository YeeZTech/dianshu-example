#pragma once
#include "ypc/stbox/ebyte.h"
#include <ff/util/ntobject.h>
#include <string>
define_nt(csv_line, std::string);
define_nt(data_slice, stbox::bytes);
typedef ff::util::ntobject<csv_line> csv_line_item_t;
typedef ff::util::ntobject<::data_slice> data_slice_item_t;
