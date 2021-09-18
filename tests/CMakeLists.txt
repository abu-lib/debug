add_executable(abu_debug_tests
    test_debug.cpp
)

abu_configure_test_target(abu_debug_tests)
target_link_libraries(abu_debug_tests PRIVATE abu::checked::debug)
