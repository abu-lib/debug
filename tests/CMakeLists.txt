add_executable(abu_debug_tests
    test_debug.cpp
)

target_link_libraries(abu_debug_tests PRIVATE abu::checked::debug)
abu_configure_test_target(abu_debug_tests)
add_test(abu_debug_tests abu_debug_tests)