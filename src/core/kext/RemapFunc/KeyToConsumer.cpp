#include "KeyToConsumer.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace RemapFunc {
    void
    KeyToConsumer::initialize(void)
    {
      keytokey_.initialize();
      consumertoconsumer_.initialize();
    }

    void
    KeyToConsumer::terminate(void)
    {
      keytokey_.terminate();
      consumertoconsumer_.terminate();
    }

    void
    KeyToConsumer::add(KeyCode newval)
    {
      switch (index_) {
        case 0:
          fromKey_.key = newval;
          keytokey_.add(newval);
          keytokey_.add(KeyCode::VK_NONE);
          break;
        default:
          IOLOG_ERROR("Invalid KeyToConsumer::add\n");
          break;
      }
      ++index_;
    }

    void
    KeyToConsumer::add(ConsumerKeyCode newval)
    {
      switch (index_) {
        case 0:
          IOLOG_ERROR("Invalid KeyToConsumer::add\n");
          break;

        case 1:
          // pass-through (== no break)
          consumertoconsumer_.add(ConsumerKeyCode::VK_PSEUDO_KEY);
          consumertoconsumer_.add(fromKey_.flags);
        default:
          consumertoconsumer_.add(newval);
          break;
      }
      ++index_;
    }

    void
    KeyToConsumer::add(Flags newval)
    {
      switch (index_) {
        case 0:
          IOLOG_ERROR("Invalid KeyToConsumer::add\n");
          break;
        case 1:
          fromKey_.flags = newval;
          break;
        default:
          consumertoconsumer_.add(newval);
          break;
      }
    }

    bool
    KeyToConsumer::remap(RemapParams& remapParams)
    {
      if (! fromkeychecker_.isFromKey(remapParams.params, fromKey_.key, fromKey_.flags)) return false;

      bool isKeyDown = remapParams.isKeyDownOrModifierDown();
      bool result = keytokey_.remap(remapParams);
      if (! result) return false;

      // ----------------------------------------
      Params_KeyboardSpecialEventCallback::auto_ptr ptr(Params_KeyboardSpecialEventCallback::alloc(isKeyDown ? EventType::DOWN : EventType::UP,
                                                                                                   FlagStatus::makeFlags(),
                                                                                                   ConsumerKeyCode::VK_PSEUDO_KEY,
                                                                                                   false));
      if (! ptr) return false;
      Params_KeyboardSpecialEventCallback& params = *ptr;

      RemapConsumerParams rp(params);
      return consumertoconsumer_.remap(rp);
    }
  }
}
