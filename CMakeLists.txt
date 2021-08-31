cmake_minimum_required(VERSION 3.20 FATAL_ERROR)

set(CMAKE_C_STANDARD 11)

project(
  cminsk
  LANGUAGES C
  VERSION 0.1.0
  DESCRIPTION "Minsk in C with garbage collection")

include(FindPkgConfig)
include(FetchContent)

pkg_search_module(gc bdw-gc>=8.0.0)
if(gc_FOUND)
  set(BDWGC ${gc_LIBRARIES})
else()
  message(STATUS "Using FetchContent to provide bdwgc")
  FetchContent_Declare(
    gc
    GIT_REPOSITORY "https://github.com/ivmai/bdwgc.git"
    GIT_TAG "v8.0.4")
  FetchContent_MakeAvailable(gc)
  set(BDWGC "gc-lib")
endif()

include_directories(include)
add_subdirectory(source)
