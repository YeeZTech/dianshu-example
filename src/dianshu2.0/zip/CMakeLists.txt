set(T_SRCS eparser.cpp base64.cpp)
add_enclave_library(zip_evaluate_parser SRCS ${T_SRCS}
  EDL ../parser/dianshu_parser.edl
  EDL_SEARCH_PATHS ${YPC_INCLUDE_DIR}/ypc/edl/util/:${YPC_INCLUDE_DIR}/ypc/edl/:${YPC_INCLUDE_DIR}/ypc/stbox/
)
target_include_directories(zip_evaluate_parser-edlobj PUBLIC
  "${YPC_INCLUDE_DIR}"
  )
target_include_directories(zip_evaluate_parser PUBLIC
  "${YPC_INCLUDE_DIR}"
  )
target_link_libraries(zip_evaluate_parser PRIVATE
  YPC::stbox_common_t
  YPC::stbox_channel_t
  YPC::analyzer_t YPC::stdeth_t)
enclave_sign(zip_evaluate_parser KEY enclave_private.pem
  CONFIG enclave.config.xml)

