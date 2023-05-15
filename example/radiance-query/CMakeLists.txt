set(T_SRCS eparser.cpp)

add_ypc_applet(radiance_query_parser
  CRYPTO stdeth
  SRCS ${T_SRCS})

enclave_sign(radiance_query_parser KEY enclave_private.pem
  CONFIG enclave.config.xml)
