set(T_SRCS eparser.cpp)

add_ypc_applet(pagerank_query_parser
  CRYPTO stdeth
  SRCS ${T_SRCS})

enclave_sign(pagerank_query_parser KEY enclave_private.pem
  CONFIG enclave.config.xml)
