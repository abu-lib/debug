// Copyright 2021 Francois Chabot

// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at

//     http://www.apache.org/licenses/LICENSE-2.0

// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef ABU_DEBUG_H_INCLUDED
#define ABU_DEBUG_H_INCLUDED

#if __has_include(<source_location>)
#include <source_location>
#endif

#include <string_view>

namespace abu::debug {

#ifdef __cpp_lib_source_location
using source_location = std::source_location;
#else
struct source_location {
  constexpr std::uint_least32_t line() const noexcept {
    return 0;
  }
  constexpr std::uint_least32_t column() const noexcept {
    return 0;
  };
  constexpr const char* file_name() const noexcept {
    return "";
  }
  constexpr const char* function_name() const noexcept {
    return "";
  }

  static constexpr source_location current() noexcept {
    return {};
  }
};
#endif

// Indicates that code cannot be reached.
#ifdef _MSC_VER
[[noreturn]] inline __forceinline void unreachable() noexcept {
  __assume(false);
}
#elif defined(__GNUC__)
[[noreturn]] inline void unreachable() noexcept {
  __builtin_unreachable();
}
#else
[[noreturn]] inline void unreachable() noexcept {}
#endif

namespace details_ {

[[noreturn]] void handle_failed_check(std::string_view msg,
                                      std::uint_least32_t line,
                                      std::uint_least32_t column,
                                      const char* file_name,
                                      const char* function_name) noexcept;

}  // namespace details_

struct ignore_tag_t {};
struct assume_tag_t {};
struct verify_tag_t {};

static constexpr ignore_tag_t ignore;
static constexpr assume_tag_t assume;
static constexpr verify_tag_t verify;

namespace details_ {
inline void constexpr_check_failure(){};
}

constexpr void check(
    ignore_tag_t,
    bool condition,
    std::string_view msg = "",
    const source_location& location = source_location::current()) noexcept {
  if (std::is_constant_evaluated()) {
    if (!condition) {
      details_::constexpr_check_failure();
    }
  }
}

constexpr void check(
    assume_tag_t,
    bool condition,
    std::string_view msg = "",
    const source_location& location = source_location::current()) noexcept {
  if (std::is_constant_evaluated()) {
    if (!condition) {
      details_::constexpr_check_failure();
    }
  }

  if (!condition) {
    unreachable();
  }
}

constexpr void check(
    verify_tag_t,
    bool condition,
    std::string_view msg = "",
    const source_location& location = source_location::current()) noexcept {
  if (std::is_constant_evaluated()) {
    if (!condition) {
      details_::constexpr_check_failure();
    }
  }

  if (!condition) {
    details_::handle_failed_check(msg,
                                  location.line(),
                                  location.column(),
                                  location.file_name(),
                                  location.function_name());
  }
}
}  // namespace abu::debug

#endif