message(STATUS "Configuring ${PROJECT_NAME} @ ${PROJECT_VERSION}")

set(${PROJECT_NAME}_version ${PROJECT_VERSION} CACHE INTERNAL "")

SET(ABU_REPO_PREFIX https://github.com/abu-lib/ CACHE STRING "")

if(NOT DEFINED ${PROJECT_NAME}_master_project)
    if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
        message(STATUS "${PROJECT_NAME} built as master project")
        set(${PROJECT_NAME}_master_project ON)
    else()
        set(${PROJECT_NAME}_master_project OFF)
    endif()
endif()

get_directory_property(hasParent PARENT_DIRECTORY)

set(${PROJECT_NAME}_build_tests ${${PROJECT_NAME}_master_project} CACHE BOOL "")
set(${PROJECT_NAME}_install ${${PROJECT_NAME}_master_project} CACHE BOOL "")
set(${PROJECT_NAME}_install_dependencies ON CACHE BOOL "(Only if ${PROJECT_NAME}_install): Also install other required abu libraries ")
set(${PROJECT_NAME}_coverage OFF CACHE BOOL "")
set(${PROJECT_NAME}_build_benchmarks OFF CACHE BOOL "")

include(FetchContent)

function(abu_fetch LIB VER)
  if(${PROJECT_NAME}_install AND ${PROJECT_NAME}_install_dependencies)
    set(abu_${LIB}_install ON CACHE BOOL "")
    set(abu_${LIB}_install_dependencies ON CACHE BOOL "")
  endif()

  if(NOT TARGET abu_${LIB})
    FetchContent_Declare(abu_${LIB} 
      GIT_REPOSITORY ${ABU_REPO_PREFIX}${LIB}.git 
      GIT_TAG ${VER}
    )
    FetchContent_MakeAvailable(abu_${LIB})
  endif()
  if(${abu_${LIB}_version} VERSION_LESS ${VER})
    message(SEND_ERROR "abu_${LIB} ${abu_${LIB}_version} is too old, ${PROJECT_NAME} needs > ${VER}")
  endif()
endfunction()

function(abu_configure_test_target TGT)
  if (NOT TARGET GTest::gtest_main)
    find_package(GTest 1.11 QUIET)
  endif()

  if (NOT TARGET GTest::gtest_main)
      message(STATUS "fetching GTest from github repo")
      FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        release-1.11.0
      )

      option(INSTALL_GTEST "Install gtest" OFF)
      set(INSTALL_GTEST OFF)
      FetchContent_MakeAvailable(googletest)
  endif()

  if(${PROJECT_NAME}_coverage)
      target_compile_options(${TGT} PRIVATE -fprofile-arcs -ftest-coverage)
      target_link_libraries(${TGT} PRIVATE gcov)
  endif()

  target_link_libraries(${TGT} PRIVATE GTest::gtest_main)
  add_test(${TGT} ${TGT})
endfunction()

function(abu_configure_benchmark_target TGT)
  if (NOT TARGET benchmark::benchmark)
    find_package(benchmark 1.6 QUIET)
  endif()

  if (NOT TARGET benchmark::benchmark)
      message(STATUS "Fetching benchmark library from github repo")

    FetchContent_Declare(
        googlebench
        GIT_REPOSITORY https://github.com/google/benchmark.git
        GIT_TAG        v1.6.0
      )
    set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googlebench)
  endif()

  target_link_libraries(${TGT} PRIVATE benchmark::benchmark)
endfunction()