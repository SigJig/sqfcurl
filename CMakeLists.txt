cmake_minimum_required(VERSION 3.10)
project(sqfcurl VERSION 1.0)

file(GLOB SOURCES "${PROJECT_SOURCE_DIR}/src/*.cpp")

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)


include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

set(Boost_INSTALL_DIR ${CONAN_BOOST_ROOT})
# find_package(Boost 1.70 COMPONENTS program_options REQUIRED)
#include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include")

#add_executable(sqfcurl ${SOURCES})
add_library(sqfcurl SHARED ${SOURCES})
target_link_libraries(sqfcurl
                        ${CONAN_LIBS}
                        )
