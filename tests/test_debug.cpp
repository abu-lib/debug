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

TEST(common, basic_api_test) {
  abu_assume(true);
  abu_precondition(true);
  abu_postcondition(true);
}

TEST(common, code_in_assumption_is_always_executed_once) {
  int count = 0;
  auto foo = [&]() {
    ++count;
    return true;
  };

  abu_assume(foo());
  EXPECT_EQ(count, 1);

  abu_precondition(foo());
  EXPECT_EQ(count, 2);

  abu_postcondition(foo());
  EXPECT_EQ(count, 3);
}

namespace {
bool unreachable_if_under_5(int v) {
  if (v > 4) {
    return true;
  }
  // This is not supposed to cause any warning despite being an unreturning
  // branch.
  abu_unreachable();
}
}  // namespace

TEST(common, reacheable_branches) {
  EXPECT_TRUE(unreachable_if_under_5(5));
}

TEST(common, can_assume_func_calls_with_commas) {
  auto foo = [](int, bool b) { return b; };
  abu_assume(foo(12, 1 > 0));
}

#ifndef NDEBUG
TEST(debug, failed_assumptions_are_fatal) {
  int x = 1;
  EXPECT_DEATH(abu_assume(x < 0), "");
}

TEST(debug, reaching_unreachable_code_is_fatal) {
  EXPECT_DEATH(unreachable_if_under_5(0), "");
}
#endif
