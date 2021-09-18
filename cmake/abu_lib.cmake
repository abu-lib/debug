message(STATUS "Configuring ${PROJECT_NAME} @ ${PROJECT_VERSION}")

set(${PROJECT_NAME}_VERSION ${PROJECT_VERSION} CACHE INTERNAL "")

set(ABU_BUILD_ALL_TESTS OFF CACHE BOOL "Build tests for all abu libs")
SET(ABU_COVERAGE OFF CACHE BOOL "Creates coverage report")
SET(ABU_BENCHMARKS OFF CACHE BOOL "Benchmark abu libraries")
SET(ABU_REPO_PREFIX https://github.com/abu-lib/ CACHE STRING "")

get_directory_property(hasParent PARENT_DIRECTORY)
if(hasParent)
  set(${PROJECT_NAME}_BUILD_TESTS ${ABU_BUILD_ALL_TESTS} CACHE BOOL "")
else()
  set(${PROJECT_NAME}_BUILD_TESTS ON CACHE BOOL "")
endif()

include(FetchContent)
function(abu_fetch LIB VER)
  if(NOT TARGET abu_${LIB})
    FetchContent_Declare(abu_${LIB} 
      GIT_REPOSITORY ${ABU_REPO_PREFIX}${LIB}.git 
      GIT_TAG ${VER}
    )
    FetchContent_MakeAvailable(abu_${LIB})
  endif()
  if(${abu_${LIB}_VERSION} VERSION_LESS ${VER})
    message(SEND_ERROR "abu_${LIB} ${abu_${LIB}_VERSION} is too old, ${PROJECT_NAME} needs > ${VER}")
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

      FetchContent_MakeAvailable(googletest)
  endif()

  if(ABU_COVERAGE)
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