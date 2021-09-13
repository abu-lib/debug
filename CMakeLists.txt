cmake_minimum_required(VERSION 3.16)
project(abu_debug VERSION 0.2.3)

include(.abu/build-utils/abu_lib.cmake)

# abu::debug
add_library(abu_debug
  src/abu_debug.cpp
)
target_compile_features(abu_debug PUBLIC cxx_std_20)
target_include_directories(abu_debug PUBLIC include)
add_library(abu::debug ALIAS abu_debug)
add_library(abu::checked::debug ALIAS abu_debug)

abu_announce(debug)

if(ABU_DEBUG_BUILD_TESTS)
  enable_testing()
  add_subdirectory(tests)
endif()
