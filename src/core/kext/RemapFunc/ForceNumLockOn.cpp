#include <IOKit/IOLib.h>

#include "ForceNumLockOn.hpp"
#include "GlobalLock.hpp"
#include "IOLogWrapper.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace RemapFunc {
    ForceNumLockOn::ForceNumLockOn(void) : index_(0)
    {}

    ForceNumLockOn::~ForceNumLockOn(void)
    {}

    void
    ForceNumLockOn::add(unsigned int datatype, unsigned int newval)
    {
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

        default:
          IOLOG_ERROR("ForceNumLockOn::add invalid datatype:%d\n", datatype);
          break;
      }
    }

    bool
    ForceNumLockOn::remap(ListHookedKeyboard::Item* item)
    {
      if (! item) return false;
      if (index_ < 2) return false;

      if (! (item->getDeviceIdentifier()).isEqual(deviceIdentifier_)) return false;

      // NumLock Hacks
      //
      // As for some keypads, NumLock is off when it was connected.
      // We need to call setNumLock(true) to activate a device.
      IOHIKeyboard* kbd = OSDynamicCast(IOHIKeyboard, item->get());
      if (kbd) {
        GlobalLock::ScopedUnlock lk;
        if (! kbd->numLock()) {
          kbd->setNumLock(true);
        }
      }

      return false;
    }
  }
}
