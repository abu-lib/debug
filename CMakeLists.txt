cmake_minimum_required(VERSION 3.16)
project(abu_debug VERSION 1.0.0)

include(.abu/build-utils/abu_lib.cmake)

add_library(abu_debug
  src/abu_debug.cpp
)
add_library(abu::debug ALIAS abu_debug)
target_include_directories(abu_debug PUBLIC include)
target_compile_features(abu_debug PUBLIC cxx_std_17)

abu_announce(debug)

if(abu_debug_TEST)
  enable_testing()
  add_subdirectory(tests)
endif()