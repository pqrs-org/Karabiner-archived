#include "NumHeldDownKeys.hpp"
#include "Config.hpp"
#include "RemapUtil.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace NumHeldDownKeys {
    int num = 0;

    static void
    log_(void)
    {
      if (config.debug_devel) {
        printf("KeyRemap4MacBook NumHeldDownKeys = %d\n", num);
      }
    }

    void
    set(const RemapParams &remapParams)
    {
      if (RemapUtil::isKeyDown(remapParams, (remapParams.params)->key)) {
        ++num;
      } else {
        --num;
      }
      log_();
    }

    void set(const RemapConsumerParams &remapParams)
    {
      if ((remapParams.params)->eventType == KeyEvent::DOWN) {
        ++num;
      } else if ((remapParams.params)->eventType == KeyEvent::UP) {
        --num;
      }
      log_();
    }

    void
    reset(void)
    {
      if (config.debug_devel) {
        printf("KeyRemap4MacBook NumHeldDownKeys::reset\n");
      }
      num = 0;
    }

    bool
    iszero(void)
    {
      return num <= 0;
    }
  }
}
