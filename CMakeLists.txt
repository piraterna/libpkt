cmake_minimum_required(VERSION 3.16)
project(libpkt VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

option(BUILD_EXAMPLES "Build example programs" OFF)

include_directories(${PROJECT_SOURCE_DIR}/include)

file(GLOB_RECURSE LIBPKT_SOURCES src/*.cpp include/libpkt/*.hpp)
add_library(libpkt STATIC ${LIBPKT_SOURCES})
target_include_directories(libpkt PUBLIC ${PROJECT_SOURCE_DIR}/include)

if(BUILD_EXAMPLES)
    add_executable(examples_simple examples/all.cpp)
    target_link_libraries(examples_simple PRIVATE libpkt)
    target_include_directories(examples_simple PRIVATE ${PROJECT_SOURCE_DIR}/include)
endif()
