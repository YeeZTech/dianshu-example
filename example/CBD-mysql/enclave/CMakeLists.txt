set(INFO_SRCS summary_org_info_parser.cpp)
#add_ypc_applet(summary_org_info_parser
  #CRYPTO stdeth
  #SRCS ${INFO_SRCS})
add_enclave_library(summary_org_info_parser SRCS ${INFO_SRCS}
  EDL ${PROJECT_SOURCE_DIR}/example/CBD-mysql/enclave/cbd_parser.edl
  EDL_SEARCH_PATHS ${YPC_INCLUDE_DIR}/ypc/edl/:${YPC_INCLUDE_DIR}/ypc/stbox/
)
target_include_directories(summary_org_info_parser-edlobj PUBLIC
  "${YPC_INCLUDE_DIR}"
  )

target_include_directories(summary_org_info_parser PUBLIC
  "${YPC_INCLUDE_DIR}"
  )
target_link_libraries(summary_org_info_parser PRIVATE
  YPC::stbox_common_t
  YPC::stbox_channel_t
  YPC::analyzer_t YPC::stdeth_t)

enclave_sign(summary_org_info_parser KEY enclave_private.pem
  CONFIG enclave.config.xml)
