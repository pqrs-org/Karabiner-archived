#include <IOKit/IOLib.h>

#include "IOLogWrapper.hpp"
#include "IgnoreMultipleSameKeyPress.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace RemapFunc {
    IgnoreMultipleSameKeyPress::IgnoreMultipleSameKeyPress(void)
    {
      lastkeycode_ = KeyCode::VK_NONE;
    }

    IgnoreMultipleSameKeyPress::~IgnoreMultipleSameKeyPress(void)
    {}

    void
    IgnoreMultipleSameKeyPress::add(unsigned int datatype, unsigned int newval)
    {
      switch (datatype) {
        case BRIDGE_DATATYPE_KEYCODE:
          fromKey_.key = KeyCode(newval);
          break;

        case BRIDGE_DATATYPE_MODIFIERFLAG:
        case BRIDGE_DATATYPE_MODIFIERFLAGS_END:
          fromKey_.flags.add(datatype, newval);
          break;

        default:
          IOLOG_ERROR("IgnoreMultipleSameKeyPress::add invalid datatype:%d\n", datatype);
          break;
      }
    }

    bool
    IgnoreMultipleSameKeyPress::remap(RemapParams& remapParams)
    {
      Params_KeyboardEventCallBack* params = remapParams.paramsUnion.get_Params_KeyboardEventCallBack();
      if (! params) return false;

      if (remapParams.isremapped || ! FlagStatus::makeFlags().isOn(fromKey_.flags)) {
        lastkeycode_ = KeyCode::VK_NONE;
        return false;
      }

      if (fromKey_.key == params->key &&
          fromKey_.key == lastkeycode_) {
        // disable event.
        remapParams.isremapped = true;
        return true;
      }

      // set lastkeycode_ if KeyUp.
      if (! params->ex_iskeydown) {
        lastkeycode_ = params->key;
      }
      return false;
    }
  }
}
