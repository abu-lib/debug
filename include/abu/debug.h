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

#include <cstdlib>

#ifndef ABU_ENABLE_CHECKS
#if !defined(NDEBUG)
#define ABU_ENABLE_CHECKS 1
#else
#define ABU_ENABLE_CHECKS 0
#endif
#endif

namespace abu::dbg::details_ {
[[noreturn]] void handle_assert_failure(const char* msg,
                                        const char* file,
                                        int line) noexcept;
}  // namespace abu::dbg::details_

// ***** unreachable() *****
#if ABU_ENABLE_CHECKS
#define abu_unreachable()                    \
  abu::dbg::details_::handle_assert_failure( \
      "Executed unreachable code", __FILE__, __LINE__)
#else

#if defined(__GNUC__)
#define abu_unreachable __builtin_unreachable
#elif defined(_MSC_VER)
#define abu_unreachable() __assume(false)
#else
#define abu_unreachable() (void)
#endif

#endif

// ***** abu_assume() *****
#if ABU_ENABLE_CHECKS
#define abu_assume(condition) \
  if (!(condition))           \
  abu::dbg::details_::handle_assert_failure(#condition, __FILE__, __LINE__)
#else
#define abu_assume(condition) \
  if (!(condition)) abu_unreachable()
#endif  // ABU_ENABLE_CHECKS

#define abu_precondition(condition) abu_assume(condition)
#define abu_postcondition(condition) abu_assume(condition)

#endif