set(MAX_SRCS max_parser.cpp)
add_ypc_applet(max_parser
  CRYPTO stdeth
  SRCS ${MAX_SRCS})
enclave_sign(max_parser KEY enclave_private.pem
  CONFIG enclave.config.xml)

set(MEAN_SRCS mean_parser.cpp)
add_ypc_applet(mean_parser
  CRYPTO stdeth
  SRCS ${MEAN_SRCS})
enclave_sign(mean_parser KEY enclave_private.pem
  CONFIG enclave.config.xml)

set(VARIANCE_SRCS variance_parser.cpp)
add_ypc_applet(variance_parser
  CRYPTO stdeth
  SRCS ${VARIANCE_SRCS})
enclave_sign(variance_parser KEY enclave_private.pem
  CONFIG enclave.config.xml)

set(PSI_SRCS psi_parser.cpp)
add_ypc_applet(psi_parser
  CRYPTO stdeth
  SRCS ${PSI_SRCS})
enclave_sign(psi_parser KEY enclave_private.pem
  CONFIG enclave.config.xml)

