set(srcs dianshu_parser_module.cpp
  )
add_untrusted_library(dianshu_parser_module SHARED
  SRCS ${srcs}
  EDL dianshu_parser.edl
  EDL_SEARCH_PATHS ${YPC_INCLUDE_DIR}/ypc/edl/util/:${YPC_INCLUDE_DIR}/ypc/edl/:${YPC_INCLUDE_DIR}/ypc/stbox/)

target_link_libraries(dianshu_parser_module PUBLIC YPC::stbox_common YPC::core_parser_module)
target_include_directories(dianshu_parser_module PUBLIC
  "$<BUILD_INTERFACE:${YPC_INCLUDE_DIR}>"
  "$<BUILD_INTERFACE:${FF_INCLUDE_DIR}>"
  "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")
target_include_directories(dianshu_parser_module INTERFACE
  "$<INSTALL_INTERFACE:${SGX_INCLUDE_DIR}>")
target_link_directories(dianshu_parser_module INTERFACE
  "$<INSTALL_INTERFACE:${SGX_LIBRARY_DIR}>")
install(TARGETS dianshu_parser_module
  DESTINATION "${lib_install_dir}")


find_package(OpenCV REQUIRED)
add_executable(dianshu_analyzer dianshu_analyzer.cpp
  sgx_bridge.cpp
  dianshu_parser.cpp
  )
target_link_libraries(dianshu_analyzer
  YPC::stbox_common
  YPC::core
  YPC::keymgr_module
  YPC::keymgr_utils
  dianshu_parser_module
  glog
  ${OpenCV_LIBS}
  )
install(TARGETS dianshu_analyzer
  DESTINATION "${bin_install_dir}")
