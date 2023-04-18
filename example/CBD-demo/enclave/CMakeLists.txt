set(INFO_SRCS company_info_parser.cpp)
add_ypc_applet(company_info_parser
  CRYPTO stdeth
  SRCS ${INFO_SRCS})
enclave_sign(company_info_parser KEY enclave_private.pem
  CONFIG enclave.config.xml)

set(STATISTICS_SRCS company_statistics_parser.cpp)
add_ypc_applet(company_statistics_parser
  CRYPTO stdeth
  SRCS ${STATISTICS_SRCS})
enclave_sign(company_statistics_parser KEY enclave_private.pem
  CONFIG enclave.config.xml)
