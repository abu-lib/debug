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

#include "abu/debug.h"
#include "gtest/gtest.h"

using namespace abu;

constexpr debug::config validated = {
    .check_assumptions = true,
    .check_preconditions = false,
};

constexpr debug::config check_precond = {
    .check_assumptions = false,
    .check_preconditions = true,
};

constexpr void foo(int x) {
  debug::precondition<check_precond>(x < 10);
}

template <auto Func, auto... Args>
constexpr int wrapper() {
  (void)Func(Args...);
  return 0;
}

template <auto Func, auto... Args>
concept FailsConstexpr = requires {
  typename std::integral_constant<int, wrapper<Func, Args...>()>;
};

static_assert(FailsConstexpr<foo, 5>);
static_assert(!FailsConstexpr<foo, 12>);

TEST(debug, preconditions) {
  debug::precondition<validated>(true);
  debug::precondition<validated>(true, "With a message");

  debug::precondition<check_precond>(true);
  debug::precondition<check_precond>(true, "With a message");

  EXPECT_DEATH(debug::precondition<check_precond>(false), "Precondition");
  EXPECT_DEATH(debug::precondition<check_precond>(false, "With a message"),
               "With a message");
  // Invoking debug::precondition<validated>(false) is UB
}

TEST(debug, validation) {
  debug::assume<validated>(true);
  debug::assume<validated>(true, "With a message");

  debug::assume<check_precond>(true);
  debug::assume<check_precond>(true, "With a message");

  EXPECT_DEATH(debug::assume<validated>(false), "Assumption");
  EXPECT_DEATH(debug::precondition<check_precond>(false, "With a message"),
               "With a message");
  // Invoking debug::assume<validated>(false) is UB
}
