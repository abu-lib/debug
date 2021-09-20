cmake_minimum_required(VERSION 3.16)
project(abu_debug VERSION 0.3.2)

include(cmake/abu_lib.cmake)

# abu::debug
add_library(abu_debug
  src/abu_debug.cpp
)

target_compile_features(abu_debug PUBLIC cxx_std_20)
target_include_directories(abu_debug PUBLIC
  "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>"
  "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)
add_library(abu::debug ALIAS abu_debug)

#abu::checked::debug
add_library(abu_debug_checked
  src/abu_debug.cpp
)

target_compile_definitions(abu_debug_checked PUBLIC ABU_DEBUG_ASSUMPTIONS=verify)
target_compile_features(abu_debug_checked PUBLIC cxx_std_20)
target_include_directories(abu_debug_checked PUBLIC
  "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>"
  "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)
add_library(abu::checked::debug ALIAS abu_debug_checked)


# Tests
if(${PROJECT_NAME}_build_tests)
  enable_testing()
  add_subdirectory(tests)
endif()

# Installation
if(ABU_INSTALL)
  install(
    DIRECTORY include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
  )

  install(TARGETS abu_debug abu_debug_checked
    EXPORT abu
      RUNTIME DESTINATION ${ABU_INSTALL_BINDIR}
      ARCHIVE DESTINATION ${ABU_INSTALL_LIBDIR}
      LIBRARY DESTINATION ${ABU_INSTALL_LIBDIR}
  )

endif()