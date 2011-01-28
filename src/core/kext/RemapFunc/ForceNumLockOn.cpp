#include "ForceNumLockOn.hpp"

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
              vendorID_ = newval;
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
              productID_ = newval;
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
      if (index_ != 2) return false;

      if (! item->isEqualVendorProduct(vendorID_, productID_)) return false;

      // NumLock Hacks
      //
      // As for some keypads, NumLock is off when it was connected.
      // We need to call setAlphaLock(true) to activate a device.
      IOHIKeyboard* kbd = OSDynamicCast(IOHIKeyboard, item->get());
      if (kbd && ! kbd->numLock()) {
        kbd->setNumLockFeedback(true);
      }

      return false;
    }
  }
}
