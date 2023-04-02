#版本号
cmake_minimum_required(VERSION 3.8)


add_executable(BP_test BP_test.cpp)
target_link_libraries(BP_test BPTREE)

add_executable(BITMAP_test bit_map_test.cpp)
target_link_libraries(BITMAP_test BITMAP)

add_executable(db_base db_base.cpp)
target_link_libraries(db_base db_example)

add_executable(PAGE_test page_test.cpp)
target_link_libraries(PAGE_test PAGE)





