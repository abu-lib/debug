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

```cpp
namespace abu::debug {
  struct ignore_tag_t {};
  struct assume_tag_t {};
  struct verify_tag_t {};

  static constexpr ignore_tag_t ignore;
  static constexpr assume_tag_t assume;
  static constexpr verify_tag_t verify;

  template<typename BehaviorTag>
  constexpr void check(
    BehaviorTag,
    bool condition,
    std::string_view msg = "",
    const source_location& location = source_location::current()) noexcept;

  [[noreturn]] constexpr void unreachable() noexcept;
}
```

Note: Checks are always performed in manifestly constexpr contexts, regardless of the chosen behavior.

Unlike `assert()`, there is no one-size-fit-all behavior selection logic for 
`check()`. Different modules often want to use different logic to determine 
which checks to perform. 

For example, in a library, we will generally want to perform aggressive internal
assertions in testing builds, and maintain precondition checks for user debug builds.

As such, `check()` is not really meant to be invoked directly from code, but instead
contextually wrapped. A typical setup looks like this.

```cpp
// my_code/debug.h

// MY_CODE_ASSUMPTIONS is set to validate as a compiler flag in the appropriate builds.
#ifndef MY_CODE_ASSUMPTIONS
#define MY_CODE_ASSUMPTIONS assume
#endif

#ifndef MY_CODE_PRECONDITIONS
  #ifdef NDEBUG
    #define MY_CODE_PRECONDITIONS assume
  #else
    #define MY_CODE_PRECONDITIONS validate
  #endif
#endif

#include "abu/debug.h"

namespace my_code {

inline constexpr void assume(
    bool condition, 
    std::string_view msg={}
    abu::debug::source_location loc = abu::debug::source_location::current()) noexcept {
  return abu::debug::check(abu::debug::MY_CODE_ASSUMPTIONS, condition, msg, loc);
} 

inline constexpr void precondition(
    bool condition, 
    std::string_view msg={},
    abu::debug::source_location loc = abu::debug::source_location::current()) noexcept {
  return abu::debug::check(abu::debug::MY_CODE_PRECONDITIONS, condition, msg, loc);
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
    precondition(v.index() == 1, "v must be an int here");

    v = 12.5f;
    assume(v.index() == 2);
}
}
```