add_library(company_reader SHARED company_reader.cpp)
target_link_libraries(company_reader pthread ff_net)

add_library(tax_reader SHARED tax_reader.cpp)
target_link_libraries(tax_reader pthread ff_net)

add_library(industry_reader SHARED industry_reader.cpp)
target_link_libraries(industry_reader pthread ff_net)

add_library(company_label_reader SHARED company_label_reader.cpp)
target_link_libraries(company_label_reader pthread ff_net)

add_library(county_reader SHARED county_reader.cpp)
target_link_libraries(county_reader pthread ff_net)
