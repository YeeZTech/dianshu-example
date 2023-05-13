set(T_SRCS eparser.cpp)

add_ypc_applet(gdp_prediction_parser
  CRYPTO stdeth
  SRCS ${T_SRCS})

enclave_sign(gdp_prediction_parser KEY enclave_private.pem
  CONFIG enclave.config.xml)
