cmake_minimum_required(VERSION 3.16)
project(abu_debug VERSION 0.3.1)

include(cmake/abu_lib.cmake)

# abu::debug
add_library(abu_debug
  src/abu_debug.cpp
)

target_compile_features(abu_debug PUBLIC cxx_std_20)
target_include_directories(abu_debug PUBLIC include)
add_library(abu::debug ALIAS abu_debug)

add_library(abu::checked::debug ALIAS abu_debug)

if(${PROJECT_NAME}_BUILD_TESTS)
  enable_testing()
  add_subdirectory(tests)
endif()