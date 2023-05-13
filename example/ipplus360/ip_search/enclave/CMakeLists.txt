set(districtT_SRCS eparser_district.cpp sgx_plugin.cpp)
add_ypc_applet(ip_search_district
  CRYPTO stdeth
  SRCS ${districtT_SRCS})
enclave_sign(ip_search_district KEY enclave_private.pem
  CONFIG enclave.config.xml)

set(streetT_SRCS eparser_street.cpp sgx_plugin.cpp)
add_ypc_applet(ip_search_street
  CRYPTO stdeth
  SRCS ${streetT_SRCS})
enclave_sign(ip_search_street KEY enclave_private.pem
  CONFIG enclave.config.xml)
