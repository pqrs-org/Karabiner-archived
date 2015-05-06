#include <IOKit/IOLib.h>

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
    flags_.add(datatype, newval);
    break;
  }

  default:
    IOLOG_ERROR("DropKeyAfterRemap::add invalid datatype:%u\n", static_cast<unsigned int>(datatype));
    break;
  }
}

void DropKeyAfterRemap::cancelEventOutputQueueItems(EventOutputQueue::Item &item) {
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
      if (params->flags.isOn(flags_)) {
        dropped_ = true;
        item.cancel();
      }
    }
  }
}
}
}
