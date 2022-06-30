add_subdirectory(enclave)

add_library(dianshu_http_request SHARED
  http_request.cpp)
install(TARGETS dshttp_request DESTINATION lib)
