#ifndef ABU_DEBUG_CONFIG_H_INCLUDED
#define ABU_DEBUG_CONFIG_H_INCLUDED

namespace abu::debug {
struct config {
  bool check_assumptions = false;
  bool check_preconditions = false;
};

}  // namespace abu::debug

#endif