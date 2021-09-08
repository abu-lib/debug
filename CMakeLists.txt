cmake_minimum_required(VERSION 3.16)
project(abu_debug VERSION 1.0.0)

get_directory_property(hasParent PARENT_DIRECTORY)

if(hasParent)
  set(abu_debug_on_if_main OFF)
else()
  set(abu_debug_on_if_main ON)
endif()

option(ABU_DEBUG_TESTS "Enable tests for abu::debug" ${abu_debug_on_if_main})

add_library(abu_debug
  src/abu_debug.cpp
)
add_library(abu::debug ALIAS abu_debug)

target_include_directories(abu_debug PUBLIC include)
target_compile_features(abu_debug PUBLIC cxx_std_20)

if(ABU_DEBUG_TESTS)
  add_subdirectory(tests)
endif()