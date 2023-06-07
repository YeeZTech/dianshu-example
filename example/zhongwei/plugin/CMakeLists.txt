add_library(user_reader SHARED user_reader.cpp)
target_link_libraries(user_reader pthread ff_net)

add_library(station_reader SHARED station_reader.cpp)
target_link_libraries(station_reader pthread ff_net)

add_library(sjdx_log_csv_reader SHARED sjdx_reader.cpp)
target_link_libraries(sjdx_log_csv_reader pthread ff_net)

add_library(sjhd_log_csv_reader SHARED sjhd_reader.cpp)
target_link_libraries(sjhd_log_csv_reader pthread ff_net)

add_library(sjsw_log_csv_reader SHARED sjsw_reader.cpp)
target_link_libraries(sjsw_log_csv_reader pthread ff_net)

add_library(xlgj_csv_reader SHARED xlgj_reader.cpp)
target_link_libraries(xlgj_csv_reader pthread ff_net)
