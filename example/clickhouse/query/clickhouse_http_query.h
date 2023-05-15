#pragma once

#include <stdint.h>

extern "C" {
uint32_t ocall_clickhouse_http_query(const char *request, uint32_t req_size,
                                     char *response, uint32_t resp_size);
}
