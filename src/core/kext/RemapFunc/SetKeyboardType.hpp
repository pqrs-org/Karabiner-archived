#ifndef SETKEYBOARDTYPE_HPP
#define SETKEYBOARDTYPE_HPP

#include "RemapFuncBase.hpp"

namespace org_pqrs_Karabiner {
  namespace RemapFunc {
    class SetKeyboardType : RemapFuncBase {
    public:
      SetKeyboardType(void) :
        RemapFuncBase(BRIDGE_REMAPTYPE_SETKEYBOARDTYPE),
        toKeyboardType_(KeyboardType::MACBOOK)
      {}

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
