#include "eparser_t.h"
#include "ip_search_district.h"
#include "sgx_plugin.h"
#include "sgx_utils.h"
#include "stbox/tsgx/log.h"
#include "ypc_t/analyzer/macro.h"
#include "ypc_t/analyzer/parser_wrapper.h"
#include <stdarg.h>
#include <stdio.h> /* vsnprintf */
#include <string.h>

ypc::parser_wrapper<user_item_t, ip_search_district> pw;

YPC_PARSER_IMPL(pw);
