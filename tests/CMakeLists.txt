add_executable(abu_debug_tests
    test_debug.cpp
)

target_link_libraries(abu_debug_tests PRIVATE GTest::gtest_main abu::debug)
add_test(abu_debug_tests abu_debug_tests)