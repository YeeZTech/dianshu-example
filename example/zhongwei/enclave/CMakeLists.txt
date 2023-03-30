set(T_SRCS eparser.cpp)

add_ypc_applet(person_tel_parser
  CRYPTO stdeth
  SRCS ${T_SRCS})

enclave_sign(person_tel_parser KEY enclave_private.pem
  CONFIG enclave.config.xml)
