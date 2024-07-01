set(T_SRCS heatmap_eparser.cpp base64.cpp)
add_enclave_library(heatmap_tsv_evaluate_parser SRCS ${T_SRCS}
  EDL ../parser/dianshu_parser.edl
  EDL_SEARCH_PATHS ${YPC_INCLUDE_DIR}/ypc/edl/util/:${YPC_INCLUDE_DIR}/ypc/edl/:${YPC_INCLUDE_DIR}/ypc/stbox/
)
target_include_directories(heatmap_tsv_evaluate_parser-edlobj PUBLIC
  "${YPC_INCLUDE_DIR}"
  )
target_include_directories(heatmap_tsv_evaluate_parser PUBLIC
  "${YPC_INCLUDE_DIR}"
  )
target_link_libraries(heatmap_tsv_evaluate_parser PRIVATE
  YPC::stbox_common_t
  YPC::stbox_channel_t
  YPC::analyzer_t YPC::stdeth_t)
enclave_sign(heatmap_tsv_evaluate_parser KEY enclave_private.pem
  CONFIG enclave.config.xml)
