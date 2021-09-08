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

TEST(debug, preconditions) {
  debug::precondition<validated>(true);
  debug::precondition<check_precond>(true);

  EXPECT_DEATH(debug::precondition<check_precond>(false), "");
  // Invoking debug::precondition<validated>(false) is UB
}

TEST(debug, validation) {
  debug::assume<validated>(true);
  debug::assume<check_precond>(true);

  EXPECT_DEATH(debug::assume<validated>(false), "");
  // Invoking debug::assume<validated>(false) is UB
}


namespace abu::debug {
TEST(debug, shorthand_api) {
  abu_assume(true);
  abu_precondition(true);
}
}