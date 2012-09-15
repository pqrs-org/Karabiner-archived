#ifndef FORCENUMLOCKON_HPP
#define FORCENUMLOCKON_HPP

#include "RemapFuncClasses.hpp"
#include "ListHookedKeyboard.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace RemapFunc {
    class ForceNumLockOn {
    public:
      ForceNumLockOn(void);
      ~ForceNumLockOn(void);

      bool remap(ListHookedKeyboard::Item* item);

      // ----------------------------------------
      // [0] => DeviceVendor
      // [1] => DeviceProduct
      void add(unsigned int datatype, unsigned int newval);

    private:
      size_t index_;
      DeviceIdentifier deviceIdentifier_;
    };
  }
}

#endif
