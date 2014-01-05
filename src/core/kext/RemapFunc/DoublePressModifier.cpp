#include <IOKit/IOLib.h>

#include "Config.hpp"
#include "DoublePressModifier.hpp"
#include "IOLogWrapper.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace RemapFunc {
    DoublePressModifier::DoublePressModifier(void) :
      isUseSeparator_(false),
      index_is_double_pressed_(false),
      index_(0),
      pressCount_(0)
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
              fromKey_.key = KeyCode(newval);
              keytokey_.add(KeyCode(newval));
              keytokey_fire_.add(KeyCode::VK_PSEUDO_KEY);
              break;

            default:
              if (! isUseSeparator_ && index_ >= 2) {
                index_is_double_pressed_ = true;
              }

              if (! index_is_double_pressed_) {
                keytokey_.add(KeyCode(newval));
              } else {
                keytokey_fire_.add(KeyCode(newval));
              }
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
              fromKey_.flags = Flags(newval);
              keytokey_.add(Flags(newval));
              keytokey_fire_.add(fromKey_.flags);
              break;

            default:
              if (! index_is_double_pressed_) {
                keytokey_.add(Flags(newval));
              } else {
                keytokey_fire_.add(Flags(newval));
              }
              break;
          }
          break;
        }

        case BRIDGE_DATATYPE_OPTION:
        {
          Option option(newval);
          if (Option::USE_SEPARATOR == option) {
            isUseSeparator_ = true;
          } else if (Option::SEPARATOR == option) {
            if (index_ >= 2) {
              index_is_double_pressed_ = true;
            }
          } else {
            IOLOG_ERROR("DoublePressModifier::add unknown option:%d\n", newval);
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
      Params_KeyboardEventCallBack* params = remapParams.paramsUnion.get_Params_KeyboardEventCallBack();
      if (! params) return false;

      bool result = keytokey_.remap(remapParams);
      if (! result) {
        pressCount_ = 0;
        return false;
      }

      // ----------------------------------------
      if (ic_.getmillisec() > static_cast<uint32_t>(Config::get_essential_config(BRIDGE_ESSENTIAL_CONFIG_INDEX_parameter_doublepressmodifier_threshold))) {
        pressCount_ = 0;
      }
      ic_.begin();

      if (params->ex_iskeydown) {
        ++pressCount_;
      } else {
        if (pressCount_ >= 2) {
          pressCount_ = 0;

          // clear temporary flags.
          FlagStatus::set();
          keytokey_fire_.call_remap_with_VK_PSEUDO_KEY(EventType::DOWN);
          keytokey_fire_.call_remap_with_VK_PSEUDO_KEY(EventType::UP);
        }
      }

      return true;
    }
  }
}
