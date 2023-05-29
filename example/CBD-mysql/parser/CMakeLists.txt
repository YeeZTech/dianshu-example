set(srcs cbd_parser_module.cpp
  )
add_untrusted_library(cbd_parser_module SHARED
  SRCS ${srcs}
  EDL ${PROJECT_SOURCE_DIR}/example/CBD-mysql/enclave/cbd_parser.edl
  EDL_SEARCH_PATHS ${YPC_INCLUDE_DIR}/ypc/edl/:${YPC_INCLUDE_DIR}/ypc/stbox/)

target_link_libraries(cbd_parser_module PUBLIC YPC::stbox_common)

target_include_directories(cbd_parser_module PUBLIC
  "$<BUILD_INTERFACE:${YPC_INCLUDE_DIR}>"
  "$<BUILD_INTERFACE:${FF_INCLUDE_DIR}>"
  "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")

target_include_directories(cbd_parser_module INTERFACE
  "$<INSTALL_INTERFACE:${SGX_INCLUDE_DIR}>")
target_link_directories(cbd_parser_module INTERFACE
  "$<INSTALL_INTERFACE:${SGX_LIBRARY_DIR}>")

add_executable(cbd_parser main.cpp
  cbd_parser.cpp
  )
target_link_libraries(cbd_parser
  YPC::core
  YPC::keymgr_module
  YPC::keymgr_utils
  YPC::terminus
  cbd_parser_module
  )
