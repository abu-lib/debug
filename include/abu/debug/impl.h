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

#ifndef ABU_DEBUG_IMPL_H_INCLUDED
#define ABU_DEBUG_IMPL_H_INCLUDED

#if __has_include(<source_location>)
#include <source_location>
#endif

#include <string_view>
#include <type_traits>

namespace abu::debug {
namespace details_ {

[[noreturn]] void handle_failed_check(std::string_view msg,
                                      std::uint_least32_t line,
                                      std::uint_least32_t column,
                                      const char* file_name,
                                      const char* function_name) noexcept;

// These simply make compile-time errors look nicer.
inline void assumption_failed_error() {}
inline void precondition_failed_error() {}

}  // namespace details_

// ********** unreachable() **********
#ifdef _MSC_VER
// MSVC needs the __forceinline in order to properly optimize this.
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

// ********** assume() **********
template <config Cfg>
constexpr void assume(bool condition,
                      std::string_view msg,
                      const source_location location) noexcept {
  if (std::is_constant_evaluated()) {
    if (!condition) {
      details_::assumption_failed_error();
    }
  } else {
    if constexpr (Cfg.check_assumptions) {
      if (!condition) {
        details_::handle_failed_check(msg,
                                      location.line(),
                                      location.column(),
                                      location.file_name(),
                                      location.function_name());
      }
    } else {
      if (!condition) {
        unreachable();
      }
    }
  }
}

// ********** precondition() **********
template <config Cfg>
constexpr void precondition(bool condition,
                            std::string_view msg,
                            const source_location location) noexcept {
  if (std::is_constant_evaluated()) {
    if (!condition) {
      details_::precondition_failed_error();
    }
  } else {
    if constexpr (Cfg.check_preconditions) {
      if (!condition) {
        details_::handle_failed_check(msg,
                                      location.line(),
                                      location.column(),
                                      location.file_name(),
                                      location.function_name());
      }
    } else {
      if (!condition) {
        unreachable();
      }
    }
  }
}
}  // namespace abu::debug

#endif