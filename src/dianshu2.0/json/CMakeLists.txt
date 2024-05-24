set(T_SRCS eparser.cpp)

add_ypc_applet(json_evaluate_parser
  CRYPTO stdeth
  SRCS ${T_SRCS})

enclave_sign(json_evaluate_parser KEY enclave_private.pem
  CONFIG enclave.config.xml)


