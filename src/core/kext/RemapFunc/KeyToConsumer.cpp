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
      if (! fromkeychecker_.isFromKey(remapParams.params.ex_iskeydown, remapParams.params.key, FlagStatus::makeFlags(), fromKey_.key, fromKey_.flags)) return false;

      bool result = keytokey_.remap(remapParams);
      if (! result) return false;

      // ----------------------------------------
      return consumertoconsumer_.call_remap_with_VK_PSEUDO_KEY(remapParams.params.ex_iskeydown ? EventType::DOWN : EventType::UP);
    }
  }
}
