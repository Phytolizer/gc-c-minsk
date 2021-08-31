cmake_minimum_required(VERSION 3.20 FATAL_ERROR)

set(CMAKE_C_STANDARD 11)

project(
  cminsk
  LANGUAGES C
  VERSION 0.1.0
  DESCRIPTION "Minsk in C with garbage collection")

include_directories(include)
add_subdirectory(source)
