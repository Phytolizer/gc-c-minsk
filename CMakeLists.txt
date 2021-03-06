cmake_minimum_required(VERSION 3.20 FATAL_ERROR)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)

project(
  cminsk
  LANGUAGES C CXX
  VERSION 0.1.0
  DESCRIPTION "Minsk in C with garbage collection"
)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g3")
endif()

set(CMAKE_C_FLAGS
    "${CMAKE_C_FLAGS} -Wall -Wextra -Werror=implicit-function-declaration -Werror=incompatible-pointer-types -Werror=return-type -Wno-sign-compare"
)

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
    GIT_TAG "v8.0.4"
  )
  FetchContent_MakeAvailable(gc)
  set(BDWGC "gc-lib")
endif()

FetchContent_Declare(
  doctest
  GIT_REPOSITORY "https://github.com/onqtam/doctest.git"
  GIT_TAG "2.4.6"
)
FetchContent_MakeAvailable(doctest)

include_directories(include)
add_subdirectory(source)
