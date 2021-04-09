cmake_minimum_required(VERSION 3.10)

project(sqfcurl VERSION 1.0)

file(GLOB SOURCES "${PROJECT_SOURCE_DIR}/src/*.cpp")

include(FindPkgConfig)
pkg_check_modules(CURLPP REQUIRED curlpp)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

find_package(Boost 1.70 REQUIRED COMPONENTS
                        thread
                        system)

include_directories(${Boost_INCLUDE_DIR})

add_executable(sqfcurl src/main.cpp ${SOURCES})

target_link_libraries(sqfcurl
                        ${Boost_LIBRARIES}
                        ${CURLPP_LDFLAGS}
                        )
