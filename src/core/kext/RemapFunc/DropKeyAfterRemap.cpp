#include "diagnostic_macros.hpp"

BEGIN_IOKIT_INCLUDE;
#include <IOKit/IOLib.h>
END_IOKIT_INCLUDE;

#include "DropKeyAfterRemap.hpp"
#include "IOLogWrapper.hpp"

namespace org_pqrs_Karabiner {
namespace RemapFunc {
void DropKeyAfterRemap::add(AddDataType datatype, AddValue newval) {
  switch (datatype) {
  case BRIDGE_DATATYPE_KEYCODE: {
    key_ = KeyCode(newval);
    break;
  }

  case BRIDGE_DATATYPE_MODIFIERFLAG:
  case BRIDGE_DATATYPE_MODIFIERFLAGS_END: {
    ModifierFlag modifierFlag(datatype, newval);
    flags_.add(modifierFlag);
    if (ModifierFlag::NONE == modifierFlag) {
      strictFlagsMatch_ = true;
    }
    break;
  }

  default:
    IOLOG_ERROR("DropKeyAfterRemap::add invalid datatype:%u\n", static_cast<unsigned int>(datatype));
    break;
  }
}

bool DropKeyAfterRemap::remap(RemapParams& remapParams) {
  helper_.registerSerialNumber();
  return true;
}

void DropKeyAfterRemap::cancelEventOutputQueueItems(EventOutputQueue::Item& item) {
  if (!helper_.isTarget(item)) {
    return;
  }

  auto params = item.getParamsBase().get_Params_KeyboardEventCallBack();
  if (params) {
    if (params->key != key_) {
      return;
    }

    if (!params->ex_iskeydown) {
      if (dropped_) {
        dropped_ = false;
        item.cancel();
      }
    } else {
      if (params->flags.isOn(flags_) &&
          (!strictFlagsMatch_ || params->flags == flags_)) {
        dropped_ = true;
        item.cancel();
      }
    }
  }
}
}
}
