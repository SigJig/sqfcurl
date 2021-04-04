cmake_minimum_required(VERSION 3.10)

project(sqfcurl VERSION 1.0)
add_executable(sqfcurl src/main.cpp)

include(FindPkgConfig)
pkg_check_modules(CURLPP REQUIRED curlpp)

target_link_libraries(sqfcurl ${CURLPP_LDFLAGS})
