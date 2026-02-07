cmake_minimum_required(VERSION 3.25)
project(asset_processing C)

set(CMAKE_C_STANDARD 11)

find_package(PkgConfig REQUIRED)

pkg_check_modules(CJSON REQUIRED libcjson)

add_executable(asset_processing main.c)

target_include_directories(asset_processing PRIVATE ${CJSON_INCLUDE_DIRS})
target_link_libraries(asset_processing PRIVATE ${CJSON_LIBRARIES} m)