cmake_minimum_required(VERSION 3.0)

project(ir)

set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-variable")

set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -s")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -s")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Os")

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address -pedantic -Wall") 

string(TOUPPER "${CMAKE_BUILD_TYPE}" uppercase_CMAKE_BUILD_TYPE)
if (uppercase_CMAKE_BUILD_TYPE STREQUAL "DEBUG")
    message("[Debugging mode enabled]")
    add_definitions(-DMODE_DEBUG)
endif ()

#add_definitions(-DSOURCE_DIR="\\"${CMAKE_CURRENT_SOURCE_DIR}\\"")
include_directories(include)
file(GLOB_RECURSE SOURCES "src/*.cpp")

add_executable(ir ${SOURCES})
