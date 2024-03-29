set(T_SRCS eparser.cpp)

add_ypc_applet(csv_evaluate_parser
  CRYPTO stdeth
  SRCS ${T_SRCS})

enclave_sign(csv_evaluate_parser KEY enclave_private.pem
  CONFIG enclave.config.xml)


add_library(csv_evaluate_reader SHARED csv_line_reader.cpp)
