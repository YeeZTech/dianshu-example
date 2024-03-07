set(T_SRCS eparser.cpp)

add_ypc_applet(download_parser
  CRYPTO stdeth
  SRCS ${T_SRCS})

enclave_sign(download_parser KEY enclave_private.pem
  CONFIG enclave.config.xml)
