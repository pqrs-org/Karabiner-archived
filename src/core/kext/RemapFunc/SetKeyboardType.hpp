#ifndef SETKEYBOARDTYPE_HPP
#define SETKEYBOARDTYPE_HPP

#include "RemapFuncClasses.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace RemapFunc {
    class SetKeyboardType {
    public:
      SetKeyboardType(void);
      ~SetKeyboardType(void);

      bool remap(KeyboardType& keyboardType);

      // ----------------------------------------
      // [0] => toKeyboardType
      void add(AddDataType datatype, AddValue newval);

    private:
      KeyboardType toKeyboardType_;
    };
  }
}

#endif
