cmake_minimum_required(VERSION 3.20 FATAL_ERROR)

project(
  sds
  LANGUAGES C
  VERSION 2.1.0
  DESCRIPTION "Simple Dynamic Strings"
  HOMEPAGE_URL "https://github.com/Phytolizer/sds")

add_library(sds sds.c)
target_include_directories(sds PUBLIC "${PROJECT_SOURCE_DIR}")
