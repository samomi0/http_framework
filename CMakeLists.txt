cmake_minimum_required(VERSION 3.20)
project(http_framework)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build." FORCE)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  message(STATUS "DEBUG build: Enabling debug flags for GDB/LLDB")

  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -O0")

  # set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g3")
  # set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g3")

  # set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ggdb")
  # set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -ggdb")
else()
  message(STATUS "Release build.")
endif()

find_package(Boost REQUIRED COMPONENTS system)
find_package(spdlog REQUIRED)
find_package(fmt REQUIRED)

option(BUILD_DEMO "BUILD_DEMO" on)

include_directories(${CMAKE_SOURCE_DIR}/include)

add_subdirectory(${CMAKE_SOURCE_DIR}/src)

if(BUILD_DEMO)
  message(STATUS "Enable BUILD_DEMO=on")
  
  add_subdirectory(${CMAKE_SOURCE_DIR}/examples)
endif(BUILD_DEMO)