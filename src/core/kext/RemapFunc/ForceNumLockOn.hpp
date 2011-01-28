#ifndef FORCENUMLOCKON_HPP
#define FORCENUMLOCKON_HPP

#include "RemapFuncBase.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace RemapFunc {
    class ForceNumLockOn {
    public:
      ForceNumLockOn(void);
      ~ForceNumLockOn(void);

      bool remap(IOHIKeyboard* kbd);

      // ----------------------------------------
      void add(unsigned int datatype, unsigned int newval);
    };
  }
}

#endif
