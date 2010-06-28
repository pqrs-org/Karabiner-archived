#include "Config.hpp"
#include "DoublePressModifier.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace RemapFunc {
    void
    DoublePressModifier::initialize(void)
    {
      keytokey_.initialize();
      keytokey_fire_.initialize();
      ic_.begin();
    }

    void
    DoublePressModifier::terminate(void)
    {
      keytokey_.terminate();
      keytokey_fire_.terminate();
    }

    void
    DoublePressModifier::add(KeyCode newval)
    {
      switch (index_) {
        case 0:
          fromKey_.key = newval;
          // pass-through
        case 1:
          keytokey_.add(newval);
          break;

        case 2:
          keytokey_fire_.add(KeyCode::VK_PSEUDO_KEY);
          keytokey_fire_.add(fromKey_.flags);
          // pass-through
        case 3:
        keytokey_fire_.add(newval);
          break;
      }
      ++index_;
    }

    void
    DoublePressModifier::add(Flags newval)
    {
      switch (index_) {
        case 0:
          IOLOG_ERROR("Invalid DoublePressModifier::add\n");
          break;

        case 1:
          fromKey_.flags = newval;
          // pass-through
        case 2:
          keytokey_.add(newval);
          break;

        default:
          keytokey_fire_.add(newval);
          break;
      }
    }

    bool
    DoublePressModifier::remap(RemapParams& remapParams)
    {
      bool isKeyDown = remapParams.isKeyDownOrModifierDown();

      bool result = keytokey_.remap(remapParams);
      if (! result) {
        pressCount_ = 0;
        return false;
      }

      // ----------------------------------------
      if (ic_.getmillisec() > static_cast<uint32_t>(config.parameter_doublepressmodifier_threshold)) {
        pressCount_ = 0;
      }
      ic_.begin();

      if (isKeyDown) {
        ++pressCount_;
      } else {
        if (pressCount_ >= 2) {
          pressCount_ = 0;

          Params_KeyboardEventCallBack::auto_ptr ptr(Params_KeyboardEventCallBack::alloc(EventType::DOWN,
                                                                                         FlagStatus::makeFlags(),
                                                                                         KeyCode::VK_PSEUDO_KEY,
                                                                                         remapParams.params.keyboardType,
                                                                                         false));
          if (! ptr) return false;
          Params_KeyboardEventCallBack& params = *ptr;

          RemapParams rp_down(params);
          keytokey_fire_.remap(rp_down);

          params.eventType = EventType::UP;
          RemapParams rp_up(params);
          keytokey_fire_.remap(rp_up);
        }
      }

      return true;
    }
  }
}
