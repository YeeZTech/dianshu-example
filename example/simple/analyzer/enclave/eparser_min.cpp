#include "eparser_t.h"
#include "sgx_plugin.h"
#include <stdarg.h>
#include <stdio.h> /* vsnprintf */
#include <string.h>

#include "enclave_min_parser.h"
#include "sgx_utils.h"
#include "ypc_t/analyzer/macro.h"
#include "ypc_t/analyzer/parser_wrapper.h"

ypc::parser_wrapper<user_item_t, enclave_min_parser> pw;

YPC_PARSER_IMPL(pw);
