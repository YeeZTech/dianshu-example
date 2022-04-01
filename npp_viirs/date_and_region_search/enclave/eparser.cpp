#include "eparser_t.h"
#include "sgx_plugin.h"
#include <stdarg.h>
#include <stdio.h> /* vsnprintf */
#include <string.h>

#include "date_and_region_search.h"
#include "sgx_utils.h"
#include "ypc_t/analyzer/macro.h"

ypc::parser_wrapper<user_item_t, date_and_region_search> pw;

YPC_PARSER_IMPL(pw, ypc::ecall_parse_item_data);
