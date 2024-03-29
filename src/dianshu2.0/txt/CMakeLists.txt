set(T_SRCS eparser.cpp)

add_ypc_applet(txt_evaluate_parser
  CRYPTO stdeth
  SRCS ${T_SRCS})

enclave_sign(txt_evaluate_parser KEY enclave_private.pem
  CONFIG enclave.config.xml)


add_library(txt_evaluate_reader SHARED txt_batch_reader.cpp)
