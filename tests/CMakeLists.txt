
find_package(GTest 1.11)
if (NOT TARGET GTest::gtest_main)
    FetchContent_Declare(
      googletest
      GIT_REPOSITORY https://github.com/google/googletest.git
      GIT_TAG        release-1.11.0
    )

    FetchContent_MakeAvailable(googletest)
endif()

add_executable(abu_debug_tests
    test_debug.cpp
)

if(ABU_COVERAGE)
    target_compile_options(abu_debug_tests PRIVATE -fprofile-arcs -ftest-coverage)
    target_link_libraries(abu_debug_tests PRIVATE gcov)
endif()

target_link_libraries(abu_debug_tests PRIVATE abu::checked::debug GTest::gtest_main)
add_test(abu_debug_tests abu_debug_tests)