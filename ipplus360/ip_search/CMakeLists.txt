add_subdirectory(enclave)

add_library(dshttp_request SHARED
  http_request.cpp)
install(TARGETS dshttp_request DESTINATION lib)
