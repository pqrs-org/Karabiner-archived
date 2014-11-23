#include "Config.hpp"
#include "IOLogWrapper.hpp"
#include "KeyCode.hpp"
#include "PressingPhysicalKeys.hpp"

namespace org_pqrs_Karabiner {
int PressingPhysicalKeys::num_ = 0;

void
PressingPhysicalKeys::log(void) {
  IOLOG_DEVEL("PressingPhysicalKeys = %d\n", num_);
}

void
PressingPhysicalKeys::set(int diff) {
  num_ += diff;
  log();
}

void
PressingPhysicalKeys::reset(void) {
  IOLOG_DEVEL("PressingPhysicalKeys::reset\n");
  num_ = 0;
}

bool
PressingPhysicalKeys::iszero(void) {
  return num_ <= 0;
}
}
