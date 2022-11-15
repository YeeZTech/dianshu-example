
set(T_SRCS eparser.cpp)

add_enclave_library(gdp_prediction_v2_parser SRCS ${T_SRCS}
  EDL ${YPC_INSTALL_DIR}/include/ypc/edl/eparser.edl
  EDL_SEARCH_PATHS ${YPC_INSTALL_DIR}/include/ypc/edl/:${YPC_INSTALL_DIR}/include/ypc/stbox/
  )

target_link_libraries(gdp_prediction_v2_parser
  ypc_stdeth_t
  ypc_gmssl_t
  ypc_stdeth_t
  ypc_analyzer_t
  ypc_common_t
  sgx_eth_ecc_t
  sgx_gmssl_ecc_t
  stbox_gmssl_t
  stbox_secp256k1_t
  stbox_keccak_t
  stbox_crypto_t
  stbox_channel_t
  stbox_common_t
  sgx_trts
  sgx_tcxx
  sgx_tcrypto
  sgx_tservice
  sgx_tstdc
  )

enclave_sign(gdp_prediction_v2_parser KEY enclave_private.pem
  CONFIG enclave.config.xml)
install(FILES ${PROJECT_SOURCE_DIR}/lib/gdp_prediction_v2_parser.signed.so DESTINATION lib COMPONENT c_example OPTIONAL)
