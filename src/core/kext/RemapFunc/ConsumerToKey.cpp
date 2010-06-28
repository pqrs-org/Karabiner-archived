#include "ConsumerToKey.hpp"
#include "CommonData.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace RemapFunc {
    void
    ConsumerToKey::initialize(void)
    {
      keytokey_.initialize();
      consumertoconsumer_.initialize();
    }

    void
    ConsumerToKey::terminate(void)
    {
      keytokey_.terminate();
      consumertoconsumer_.terminate();
    }

    void
    ConsumerToKey::add(ConsumerKeyCode newval)
    {
      switch (index_) {
        case 0:
          fromKey_.key = newval;
          consumertoconsumer_.add(newval);
          consumertoconsumer_.add(ConsumerKeyCode::VK_NONE);
          break;
        default:
          IOLOG_ERROR("Invalid ConsumerToKey::add\n");
          break;
      }
      ++index_;
    }

    void
    ConsumerToKey::add(KeyCode newval)
    {
      switch (index_) {
        case 0:
          IOLOG_ERROR("Invalid ConsumerToKey::add\n");
          break;
        case 1:
          // pass-through (== no break)
          keytokey_.add(KeyCode::VK_PSEUDO_KEY);
          keytokey_.add(fromKey_.flags);
        default:
          keytokey_.add(newval);
          break;
      }
      ++index_;
    }

    void
    ConsumerToKey::add(Flags newval)
    {
      switch (index_) {
        case 0:
          break;
        case 1:
          fromKey_.flags = newval;
          break;
        default:
          keytokey_.add(newval);
          break;
      }
    }

    bool
    ConsumerToKey::remap(RemapConsumerParams& remapParams)
    {
      if (! fromkeychecker_.isFromKey(remapParams, fromKey_.key, fromKey_.flags)) return false;

      bool result = consumertoconsumer_.remap(remapParams);
      if (! result) return false;

      // ----------------------------------------
      Params_KeyboardEventCallBack::auto_ptr ptr(Params_KeyboardEventCallBack::alloc(remapParams.params.eventType,
                                                                                     FlagStatus::makeFlags(),
                                                                                     KeyCode::VK_PSEUDO_KEY,
                                                                                     CommonData::getcurrent_keyboardType(),
                                                                                     false));
      if (! ptr) return false;
      Params_KeyboardEventCallBack& params = *ptr;

      RemapParams rp(params);
      return keytokey_.remap(rp);
    }
  }
}
