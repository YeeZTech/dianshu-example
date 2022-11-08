#set(TESTING_CONFIG_OPT FILEPATH ${CMAKE_CURRENT_LIST_DIR}/config.cmake)
#include("cmake/HunterGate.cmake")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
#HunterGate(
  #URL "https://github.com/cpp-pm/hunter/archive/v0.23.231.tar.gz"
  #SHA1 "3f9910d70a5be7005a2f23448aff4f7de5ce6bed"
#)

project(DianshuExample)
cmake_minimum_required(VERSION 3.9.3)
add_definitions(-std=c++14)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
find_package(SGX REQUIRED)

enable_testing()

if(SGX_MODE STREQUAL "Debug")
	message(STATUS "This is a Debug version!")
	add_definitions(-g)
  add_definitions(-O0)
else()
  message(STATUS "This is a " ${SGX_MODE} " version!")
	add_definitions(-O2)
	add_definitions(-DNDEBUG)
endif()

if(NOT DEFINED YPC_INSTALL_DIR)
  set(YPC_INSTALL_DIR /usr/local)
endif()

include_directories(${SGX_INCLUDE_DIR})
include_directories(/usr/local/include)
link_directories(/usr/local/lib)
include_directories(${YPC_INSTALL_DIR}/include)
link_directories(${YPC_INSTALL_DIR}/lib/)
link_directories(${SGX_LIBRARY_DIR})

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib/)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin/)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib/)

add_subdirectory(example)
