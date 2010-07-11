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
          // pass-through (== no break)
          fromKey_.key = newval;
        case 1:
          keytokey_.add(newval);
          break;

        case 2:
          // pass-through (== no break)
          keytokey_fire_.add(KeyCode::VK_PSEUDO_KEY);
          keytokey_fire_.add(fromKey_.flags);
        default:
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
          // pass-through (== no break)
          fromKey_.flags = newval;
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

      if (remapParams.params.ex_iskeydown) {
        ++pressCount_;
      } else {
        if (pressCount_ >= 2) {
          pressCount_ = 0;

          keytokey_fire_.call_remap_with_VK_PSEUDO_KEY(EventType::DOWN);
          keytokey_fire_.call_remap_with_VK_PSEUDO_KEY(EventType::UP);
        }
      }

      return true;
    }
  }
}
