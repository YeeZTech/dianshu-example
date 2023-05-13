set(T_SRCS eparser.cpp)

add_ypc_applet(clickhouse_query
  CRYPTO stdeth
  SRCS ${T_SRCS})

enclave_sign(clickhouse_query KEY enclave_private.pem
  CONFIG enclave.config.xml)
