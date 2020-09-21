cmake_minimum_required(VERSION 3.0)

project(carbon-ir)

set(EXEC true CACHE BOOL "")

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-variable")

set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -s")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -s")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Os")

if (EXEC)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address")
endif()

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -pedantic -Wall -std=gnu++11") 

string(TOUPPER "${CMAKE_BUILD_TYPE}" uppercase_CMAKE_BUILD_TYPE)
if (uppercase_CMAKE_BUILD_TYPE STREQUAL "DEBUG")
    message("[Debugging mode enabled]")
    add_definitions(-DMODE_DEBUG)
endif ()

include_directories(include)
file(GLOB_RECURSE SOURCES "src/*.cpp")

if (EXEC)
    add_executable(carbon-ir ${SOURCES})
else ()
    add_library(carbon-ir STATIC ${SOURCES})
endif ()