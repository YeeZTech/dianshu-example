add_library(user_reader SHARED user_reader.cpp)
target_link_libraries(user_reader pthread ff_net)

add_library(station_reader SHARED station_reader.cpp)
target_link_libraries(station_reader pthread ff_net)
