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

        case BRIDGE_DATATYPE_FLAGS:
          fromKey_.flags = Flags(newval);
          break;

        default:
          IOLOG_ERROR("IgnoreMultipleSameKeyPress::add invalid datatype:%d\n", datatype);
          break;
      }
    }

    bool
    IgnoreMultipleSameKeyPress::remap(RemapParams& remapParams)
    {
      if (remapParams.isremapped || ! FlagStatus::makeFlags().isOn(fromKey_.flags)) {
        lastkeycode_ = KeyCode::VK_NONE;
        return false;
      }

      if (fromKey_.key == remapParams.params.key &&
          fromKey_.key == lastkeycode_) {
        // disable event.
        remapParams.isremapped = true;
        return true;
      }

      // set lastkeycode_ if KeyUp.
      if (! remapParams.params.ex_iskeydown) {
        lastkeycode_ = remapParams.params.key;
      }
      return false;
    }
  }
}
