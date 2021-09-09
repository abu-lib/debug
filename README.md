# Abu Debug

[![CI](https://github.com/abu-lib/debug/actions/workflows/ci.yml/badge.svg)](https://github.com/abu-lib/debug/actions/workflows/ci.yml)

This is part of the [Abu](http://github.com/abu-lib/abu) meta-project.

This contains utilities to ensure program correctness.

## Differences with assert()

- Code is not stripped out of the executable unless explicitely stated.
- Disabled checks become compiler hints.
- More granular control over what gets checked.
- Checks are always performed in manifestly constexpr contexts.

## Usage
There are two types of checks: 
- **Assumptions**: expectations within the inner workings of the code.
- **Preconditions**: conditions that must be met for a public interface to be usable.

```cpp
namespace abu::debug {
  struct config {
    bool check_assumptions;
    bool check_preconditions;
  };

  template<config Cfg>
  constexpr void assume(bool condition, std::string_view msg={}) noexcept;

  template<config Cfg>
  constexpr void precondition(bool condition, std::string_view msg={}) noexcept;

  [[noreturn]] constexpr void unreachable() noexcept;
}
```

Here's a typical complete real-world setup:
```cpp
// my_code/debug.h
#ifndef MY_CODE_CHECKS_ASSUMPTIONS
  #ifdef NDEBUG
    #define MY_CODE_CHECKS_ASSUMPTIONS true
  #else
    #define MY_CODE_CHECKS_ASSUMPTIONS false
  #endif
#endif

#ifndef MY_CODE_CHECKS_PRECONDITIONS
  #define MY_CODE_CHECKS_PRECONDITIONS true
#endif

#include "abu/debug.h"

namespace my_code {
constexpr abu::debug::config debug_cfg = {
  .check_assumptions = MY_CODE_CHECKS_ASSUMPTIONS;
  .check_preconditions = MY_CODE_CHECKS_PRECONDITIONS;
};

inline constexpr void assume(bool condition, 
                             std::string_view msg={}) noexcept {
  return abu::debug::assume<debug_cfg>(condition, msg);
} 

inline constexpr void precondition(bool condition, 
                                   std::string_view msg={}) noexcept {
  return abu::debug::precondition<debug_cfg>(condition, msg);
} 
}
```
```cpp
// my_code/source.cpp
#include <string>
#include <variant>

#include "my_code/debug.h"

namespace my_code {
void int_to_float(std::variant<std::string, int, float>& v) {
    precondition(v.index() == 1);

    v = 12.5f;
    assume(v.index() == 2);
}
}
```

## FAQ

> How do I write tests for checks?

For runtime tests, a death test like GoogleTest's `EXPECT_DEATH()` is the way to go.

## Kludges

`abu::debug` has the following kludges:

- `<source_location>` Is not supported on clang as of version 12. A workaround is in place
- Optimal behavior for `unreachable()` is compiler-specific.
  - MSVC needs a `__force_inline` in order to behave well.