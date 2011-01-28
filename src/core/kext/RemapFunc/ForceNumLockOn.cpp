#include "ForceNumLockOn.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace RemapFunc {
    ForceNumLockOn::ForceNumLockOn(void)
    {}

    ForceNumLockOn::~ForceNumLockOn(void)
    {}

    void
    ForceNumLockOn::add(unsigned int datatype, unsigned int newval)
    {
      switch (datatype) {
        default:
          IOLOG_ERROR("ForceNumLockOn::add invalid datatype:%d\n", datatype);
          break;
      }
    }

    bool
    ForceNumLockOn::remap(IOHIKeyboard* kbd)
    {
      // NumLock Hacks
      //
      // As for some keypads, NumLock is off when it was connected.
      // We need to call setAlphaLock(true) to activate a device.
      kbd->setNumLockFeedback(true);
      return false;
    }
  }
}
