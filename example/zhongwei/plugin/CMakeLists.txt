add_library(user_csv_reader SHARED reader.cpp)
target_link_libraries(user_csv_reader pthread ff_net)

add_library(station_csv_reader SHARED reader.cpp)
target_link_libraries(station_csv_reader pthread ff_net)
