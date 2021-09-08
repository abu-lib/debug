if (NOT TARGET GTest::gtest_main)
  include(FetchContent)

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

target_link_libraries(abu_debug_tests PRIVATE GTest::gtest_main abu::debug)
add_test(abu_debug_tests abu_debug_tests)