#include "Config.hpp"
#include "DoublePressModifier.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace RemapFunc {
    DoublePressModifier::DoublePressModifier(void) : index_(0), pressCount_(0)
    {
      ic_.begin();
    }

    DoublePressModifier::~DoublePressModifier(void)
    {}

    void
    DoublePressModifier::add(unsigned int datatype, unsigned int newval)
    {
      switch (datatype) {
        case BRIDGE_DATATYPE_KEYCODE:
        {
          switch (index_) {
            case 0:
              // pass-through (== no break)
              fromKey_.key = newval;
            case 1:
              keytokey_.add(KeyCode(newval));
              break;

            case 2:
              // pass-through (== no break)
              keytokey_fire_.add(KeyCode::VK_PSEUDO_KEY);
              keytokey_fire_.add(fromKey_.flags);
            default:
              keytokey_fire_.add(KeyCode(newval));
              break;
          }
          ++index_;

          break;
        }

        case BRIDGE_DATATYPE_FLAGS:
        {
          switch (index_) {
            case 0:
              IOLOG_ERROR("Invalid DoublePressModifier::add\n");
              break;

            case 1:
              // pass-through (== no break)
              fromKey_.flags = newval;
            case 2:
              keytokey_.add(Flags(newval));
              break;

            default:
              keytokey_fire_.add(Flags(newval));
              break;
          }
          break;
        }

        default:
          IOLOG_ERROR("DoublePressModifier::add invalid datatype:%d\n", datatype);
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
