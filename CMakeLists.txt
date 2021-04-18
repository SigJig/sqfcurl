cmake_minimum_required(VERSION 3.10)
project(sqfcurl VERSION 1.0)

set(EXECUTABLE OFF)

file(GLOB SOURCES "${PROJECT_SOURCE_DIR}/src/*.cpp")

if (EXECUTABLE)
    list(REMOVE_ITEM SOURCES "${PROJECT_SOURCE_DIR}/src/entry.cpp")
else()
    list(REMOVE_ITEM SOURCES "${PROJECT_SOURCE_DIR}/src/main.cpp")
endif()

set(CMAKE_CXX_STANDARD 17)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

set(Boost_INSTALL_DIR ${CONAN_BOOST_ROOT})

if (EXECUTABLE)
    add_executable(sqfcurl ${SOURCES})
else()
    add_library(sqfcurl SHARED ${SOURCES})
endif()

target_compile_definitions(sqfcurl
                            PRIVATE EXTENSION_NAME="sqfcurl"
                            PRIVATE EXTENSION_CB_FNC="extension_cb"
                            PUBLIC _SILENCE_CXX17_ALLOCATOR_VOID_DEPRECATION_WARNING
                            PUBLIC _WIN32_WINNT=0x0601
                            PUBLIC  _SILENCE_CXX17_C_HEADER_DEPRECATION_WARNING
                            )

target_link_libraries(sqfcurl
                    ${CONAN_LIBS})

# find_package(Boost 1.70 COMPONENTS program_options REQUIRED)
target_include_directories(sqfcurl PUBLIC
                            "${CMAKE_CURRENT_SOURCE_DIR}/include")
