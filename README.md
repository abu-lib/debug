# Abu Debug

[![CI](https://github.com/abu-lib/debug/actions/workflows/ci.yml/badge.svg)](https://github.com/abu-lib/debug/actions/workflows/ci.yml)

This is part of the [Abu](http://github.com/abu-lib/abu) meta-project.

This contains utilities to ensure program correctness.

## Differences with assert()

- Code is not stripped out of the executable unless explicitely stated.
- Disabled checks become compiler hints.

## Usage - general

**N.B.** For usage within other abu libaries, see below.

```cpp
// Set up a configuration
constexpr abu::dgb::config some_config = {
#ifdef NDEBUG
    .check_assumptions = false;
    .check_preconditions = true;
#else
    .check_assumptions = true;
    .check_preconditions = true;
#endif
};

void some_function(int x, int y) {
    // Validate that the conditions for a function being called are met
    abu::dbg::precondition<some_config>(some_value > 5 && some_value < 12);
    abu::dbg::precondition<some_config>(current_program_state == program_state::sane);
    if constexpr(some_config.check_preconditions) {
        abu::dbg::precondition<some_config>(external_test(y));
    }
    int z = x * y;

    // Check internal assumptions
    abu::dbg::assume(z > x);
}
```

## Usage - Abu libraries

`abu::<library>::details_::lib_config` is handled automatically by the build scripts

`abu_assume(condition)` and `abu_precondition(condition)` macros are 
systematically available.

- They assume that they are being called from within the `abu::<lib_name>` namespace.
- They assume that "abu/<lib_name>/details/abu_lib_config.h" has been included