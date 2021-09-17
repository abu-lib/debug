cmake_minimum_required(VERSION 3.16)
project(abu_debug VERSION 0.3.0)

set(ABU_BUILD_ALL_TESTS OFF CACHE BOOL "")

include(FetchContent)

get_directory_property(hasParent PARENT_DIRECTORY)
if(hasParent)
  set(ABU_DEBUG_BUILD_TESTS ${ABU_BUILD_ALL_TESTS} CACHE BOOL "")
else()
  set(ABU_DEBUG_BUILD_TESTS ON CACHE BOOL "")
endif()

# abu::debug
add_library(abu_debug
  src/abu_debug.cpp
)

target_compile_features(abu_debug PUBLIC cxx_std_20)
target_include_directories(abu_debug PUBLIC include)
add_library(abu::debug ALIAS abu_debug)
add_library(abu::checked::debug ALIAS abu_debug)

if(ABU_DEBUG_BUILD_TESTS)
  enable_testing()
  add_subdirectory(tests)
endif()
