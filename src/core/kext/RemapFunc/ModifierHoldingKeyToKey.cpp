#include "Config.hpp"
#include "ModifierHoldingKeyToKey.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace RemapFunc {
    void
    ModifierHoldingKeyToKey::initialize(void)
    {
      keytokey_.initialize();
      ic_.begin();
    }

    void
    ModifierHoldingKeyToKey::terminate(void)
    {
      keytokey_.terminate();
    }

    void
    ModifierHoldingKeyToKey::add(KeyCode newval)
    {
      switch (index_) {
        case 0:
          // pass-through (== no break)
          fromKey_.key = newval;
        default:
          keytokey_.add(newval);
          break;
      }
      ++index_;
    }

    void
    ModifierHoldingKeyToKey::add(Flags newval)
    {
      switch (index_) {
        case 0:
          IOLOG_ERROR("Invalid ModifierHoldingKeyToKey::add\n");
          break;

        case 1:
          // pass-through (== no break)
          fromKey_.flags = newval;
        default:
          keytokey_.add(newval);
          break;
      }
    }

    bool
    ModifierHoldingKeyToKey::remap(RemapParams& remapParams)
    {
      if (remapParams.isremapped) goto nottargetkey;
      if (! fromkeychecker_.isFromKey(remapParams, fromKey_.key, fromKey_.flags)) goto nottargetkey;
      if (! ic_.checkThreshold(config.parameter_modifierholdingkeytokey_wait)) goto nottargetkey;

      return keytokey_.remap(remapParams);

    nottargetkey:
      ic_.begin();
      return false;
    }
  }
}

