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

#include <source_location>

#include "abu/debug/config.h"
#include "abu/debug/details/abu_lib_config.h"

namespace abu::debug {
namespace details_ {
[[noreturn]] void handle_failed_check(const char*,
                                      const std::source_location&) noexcept;
}

// *****
constexpr void unreacheable() {
#if defined(__GNUC__)
#define abu_unreachable __builtin_unreachable
#elif defined(_MSC_VER)
#define abu_unreachable() __assume(false)
#endif
}

// *****
template <config Cfg>
constexpr void assume(bool condition,
                      const std::source_location location =
                          std::source_location::current()) noexcept {
  if constexpr (Cfg.check_assumptions) {
    if (!condition) {
      details_::handle_failed_check(
          "Assumption failed. This is a bug in the library.", location);
    }
  } else {
    if (!condition) {
      unreacheable();
    }
  }
}

// *****
template <config Cfg>
constexpr void precondition(bool condition,
                            const std::source_location location =
                                std::source_location::current()) noexcept {
  if constexpr (Cfg.check_preconditions) {
    if (!condition) {
      details_::handle_failed_check(
          "Precondition failed. Library not being used as expected.", location);
    }
  } else {
    if (!condition) {
      unreacheable();
    }
  }
}

#define abu_assume(condition) ::abu::debug::assume<details_::dbg_cfg>(condition)

#define abu_precondition(condition) \
  ::abu::debug::precondition<details_::dbg_cfg>(condition)
}  // namespace abu::debug

#endif