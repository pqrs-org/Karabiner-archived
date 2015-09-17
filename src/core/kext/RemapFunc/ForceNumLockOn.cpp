#include "diagnostic_macros.hpp"

BEGIN_IOKIT_INCLUDE;
#include <IOKit/IOLib.h>
END_IOKIT_INCLUDE;

#include "ForceNumLockOn.hpp"
#include "GlobalLock.hpp"
#include "IOLogWrapper.hpp"

namespace org_pqrs_Karabiner {
namespace RemapFunc {
void ForceNumLockOn::add(AddDataType datatype, AddValue newval) {
  switch (datatype) {
  case BRIDGE_DATATYPE_DEVICEVENDOR:
    switch (index_) {
    case 0:
      deviceIdentifier_.setVendor(DeviceVendor(newval));
      ++index_;
      break;
    default:
      IOLOG_ERROR("Invalid ForceNumLockOn::add\n");
      break;
    }
    break;

  case BRIDGE_DATATYPE_DEVICEPRODUCT:
    switch (index_) {
    case 1:
      deviceIdentifier_.setProduct(DeviceProduct(newval));
      ++index_;
      break;
    default:
      IOLOG_ERROR("Invalid ForceNumLockOn::add\n");
      break;
    }
    break;

  case BRIDGE_DATATYPE_DEVICELOCATION:
    switch (index_) {
    case 2:
      deviceIdentifier_.setLocation(DeviceLocation(newval));
      ++index_;
      break;
    default:
      IOLOG_ERROR("Invalid ForceNumLockOn::add\n");
      break;
    }
    break;

  case BRIDGE_DATATYPE_OPTION: {
    Option option(newval);
    if (Option::FORCENUMLOCKON_FORCE_OFF == option) {
      forceOffMode_ = true;
    }
    break;
  }

  default:
    IOLOG_ERROR("ForceNumLockOn::add invalid datatype:%u\n", static_cast<unsigned int>(datatype));
    break;
  }
}

bool ForceNumLockOn::remapForceNumLockOn(ListHookedKeyboard::Item* item) {
  if (!item) return false;
  if (index_ < 2) return false;

  if (!(item->getDeviceIdentifier()).isEqual(deviceIdentifier_)) return false;

  // NumLock Hacks
  //
  // As for some keypads, NumLock is off when it was connected.
  // We need to call setNumLock(true) to activate a device.
  IOHIKeyboard* kbd = OSDynamicCast(IOHIKeyboard, item->get());
  if (kbd) {
    GlobalLock::ScopedUnlock lk;
    if (!forceOffMode_) {
      if (!kbd->numLock()) {
        kbd->setNumLock(true);
      }
    } else {
      if (kbd->numLock()) {
        kbd->setNumLock(false);
      }
    }
  }

  return false;
}
}
}
