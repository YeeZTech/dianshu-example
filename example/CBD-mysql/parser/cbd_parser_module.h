#pragma once
#include "sgx_eid.h"
#include "sgx_error.h"
#include "ypc/common/parser_type.h"
#include "ypc/core/byte.h"
#include "ypc/core/ref.h"
#include "ypc/corecommon/nt_cols.h"
#include "ypc/stbox/usgx/sgx_module.h"
#include <string>

using stx_status = stbox::stx_status;
using parser_type_t = ypc::utc::parser_type_t;

class cbd_parser_module : public stbox::sgx_module {
public:
  explicit cbd_parser_module(const char *mod_path);

  cbd_parser_module(const cbd_parser_module &) = delete;
  cbd_parser_module(cbd_parser_module &&) = delete;
  cbd_parser_module &operator=(cbd_parser_module &&) = delete;
  cbd_parser_module &operator=(const cbd_parser_module &) = delete;

  virtual ~cbd_parser_module() = default;

  uint32_t begin_parse_data_item();
  uint32_t parse_data_item(const uint8_t *data, size_t len);
  uint32_t end_parse_data_item();

  uint32_t get_enclave_hash(ypc::bytes &enclave_hash);

  // uint32_t get_encrypted_result_hash(ypc::bref &hash);

  uint32_t get_analyze_result(ypc::bytes &res);

  uint32_t init_data_source(const ypc::bytes &info);
  uint32_t init_model(const ypc::bytes &info);
  uint32_t get_parser_type();
};
