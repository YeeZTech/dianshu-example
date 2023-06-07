set(T_SRCS person_tel_parser.cpp)
add_ypc_applet(person_tel_parser
  CRYPTO stdeth
  SRCS ${T_SRCS})
enclave_sign(person_tel_parser KEY enclave_private.pem
  CONFIG enclave.config.xml)

set(multi_userT_SRCS multi_user_parser.cpp)
add_ypc_applet(multi_user_parser
  CRYPTO stdeth
  SRCS ${multi_userT_SRCS})
enclave_sign(multi_user_parser KEY enclave_private.pem
  CONFIG enclave.config.xml)

set(carT_SRCS car_parser.cpp)
add_ypc_applet(car_parser
  CRYPTO stdeth
  SRCS ${carT_SRCS})
enclave_sign(car_parser KEY enclave_private.pem
  CONFIG enclave.config.xml)
