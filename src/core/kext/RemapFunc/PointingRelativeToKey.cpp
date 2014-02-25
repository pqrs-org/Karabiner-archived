#include <IOKit/IOLib.h>

#include "Config.hpp"
#include "IOLogWrapper.hpp"
#include "PointingRelativeToKey.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace RemapFunc {
    PointingRelativeToKey::PointingRelativeToKey(void) :
      isFirstEvent(true),
      current_keytokey_(NULL)
    {
      keyrepeat_ic_.begin();
    }

    PointingRelativeToKey::~PointingRelativeToKey(void)
    {}

    void
    PointingRelativeToKey::add(unsigned int datatype, unsigned int newval)
    {
      switch (datatype) {
        case BRIDGE_DATATYPE_POINTINGRELATIVE:
        {
          PointingRelative val(newval);
          if (val == PointingRelative::UP)    { current_keytokey_ = &(keytokey_[KEYTOKEY_UP]);    }
          if (val == PointingRelative::DOWN)  { current_keytokey_ = &(keytokey_[KEYTOKEY_DOWN]);  }
          if (val == PointingRelative::LEFT)  { current_keytokey_ = &(keytokey_[KEYTOKEY_LEFT]);  }
          if (val == PointingRelative::RIGHT) { current_keytokey_ = &(keytokey_[KEYTOKEY_RIGHT]); }
          break;
        }

        case BRIDGE_DATATYPE_KEYCODE:
        case BRIDGE_DATATYPE_CONSUMERKEYCODE:
        case BRIDGE_DATATYPE_POINTINGBUTTON:
        case BRIDGE_DATATYPE_OPTION:
        {
          if (! current_keytokey_) {
            IOLOG_ERROR("Invalid PointingRelativeToKey::add\n");
          } else {
            if (current_keytokey_->toKeysSize() == 0) {
              current_keytokey_->add(KeyCode::VK_PSEUDO_KEY);
              current_keytokey_->add(fromFlags_);
            }
            current_keytokey_->add(datatype, newval);
          }

          break;
        }

        case BRIDGE_DATATYPE_FLAGS:
        {
          if (! current_keytokey_) {
            fromFlags_ = Flags(newval);
          } else {
            current_keytokey_->add(datatype, newval);
          }
          break;
        }

        default:
          IOLOG_ERROR("PointingRelativeToKey::add invalid datatype:%d\n", datatype);
          break;
      }
    }

    bool
    PointingRelativeToKey::remap(RemapParams& remapParams)
    {
      Params_RelativePointerEventCallback* params = remapParams.paramsUnion.get_Params_RelativePointerEventCallback();
      if (! params) goto skip;

      // ------------------------------------------------------------
      if (remapParams.isremapped) goto skip;
      if (! FlagStatus::makeFlags().isOn(fromFlags_)) goto skip;
      // skip button press/release events.
      if (params->ex_button != PointingButton::NONE) goto skip;

      remapParams.isremapped = true;

      // ------------------------------------------------------------
      {
        uint32_t detection_interval = static_cast<uint32_t>(Config::get_essential_config(BRIDGE_ESSENTIAL_CONFIG_INDEX_parameter_pointingrelativetokey_detection_interval));
        if (isFirstEvent || keyrepeat_ic_.getmillisec() > detection_interval) {
          size_t keytokey_index = KEYTOKEY__END__;
          PointingRelative pr = PointingRelative::getPointingRelativeFromDelta(params->dx, params->dy);
          if (pr == PointingRelative::UP)    { keytokey_index = KEYTOKEY_UP;    }
          if (pr == PointingRelative::DOWN)  { keytokey_index = KEYTOKEY_DOWN;  }
          if (pr == PointingRelative::LEFT)  { keytokey_index = KEYTOKEY_LEFT;  }
          if (pr == PointingRelative::RIGHT) { keytokey_index = KEYTOKEY_RIGHT; }

          if (keytokey_index != KEYTOKEY__END__) {
            keytokey_[keytokey_index].call_remap_with_VK_PSEUDO_KEY(EventType::DOWN);
            keytokey_[keytokey_index].call_remap_with_VK_PSEUDO_KEY(EventType::UP);
            // clear temporary flags.
            FlagStatus::set();
          }

          isFirstEvent = false;
          keyrepeat_ic_.begin();
        }
      }

      return true;

    skip:
      isFirstEvent = true;
      return false;
    }
  }
}
