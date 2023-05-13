set(T_SRCS eparser.cpp)

add_ypc_applet(gdp_query_v2_parser
  CRYPTO stdeth
  SRCS ${T_SRCS})

enclave_sign(gdp_query_v2_parser KEY enclave_private.pem
  CONFIG enclave.config.xml)
