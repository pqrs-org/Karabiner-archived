#ifndef SETKEYBOARDTYPE_HPP
#define SETKEYBOARDTYPE_HPP

#include "RemapFuncBase.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace RemapFunc {
    class SetKeyboardType {
      SetKeyboardType(void);
      ~SetKeyboardType(void);

      bool remap(KeyboardType& keyboardType);

      // ----------------------------------------
      // [0] => toKeyboardType
      void add(unsigned int datatype, unsigned int newval);

    private:
      KeyboardType toKeyboardType_;
    };
  }
}

#endif
