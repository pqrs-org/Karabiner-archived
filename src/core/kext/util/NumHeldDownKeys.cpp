#include "NumHeldDownKeys.hpp"
#include "Config.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  int NumHeldDownKeys::num_ = 0;

  void
  NumHeldDownKeys::log(void)
  {
    if (config.debug_devel) {
      printf("KeyRemap4MacBook NumHeldDownKeys = %d\n", num_);
    }
  }

  void
  NumHeldDownKeys::set(const RemapParams& remapParams)
  {
    if (remapParams.isKeyDownOrModifierDown()) {
      ++num_;
    } else {
      --num_;
    }
    log();
  }

  void
  NumHeldDownKeys::set(const RemapConsumerParams& remapParams)
  {
    if (remapParams.params.eventType == EventType::DOWN) {
      ++num_;
    } else if (remapParams.params.eventType == EventType::UP) {
      --num_;
    }
    log();
  }

  void
  NumHeldDownKeys::set(int diff) {
    num_ += diff;
    log();
  }

  void
  NumHeldDownKeys::reset(void)
  {
    if (config.debug_devel) {
      printf("KeyRemap4MacBook NumHeldDownKeys::reset\n");
    }
    num_ = 0;
  }

  bool
  NumHeldDownKeys::iszero(void)
  {
    return num_ <= 0;
  }
}
