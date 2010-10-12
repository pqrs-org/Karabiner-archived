#ifndef KEYTOPOINTINGBUTTON_HPP
#define KEYTOPOINTINGBUTTON_HPP

#include "bridge.h"
#include "RemapFuncBase.hpp"
#include "FromKeyChecker.hpp"
#include "KeyToKey.hpp"
#include "PointingButtonToPointingButton.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace RemapFunc {
    class KeyToPointingButton {
    public:
      KeyToPointingButton(void);
      ~KeyToPointingButton(void);

      bool remap(RemapParams& remapParams);

      // ----------------------------------------
      // [0] => fromKey_
      // [1] => toButtons_[0]
      // [2] => toButtons_[1]
      // [3] => ...
      void add(unsigned int datatype, unsigned int newval);

      // ----------------------------------------
      // utility functions
      void add(KeyCode newval)        { add(BRIDGE_DATATYPE_KEYCODE, newval.get()); }
      void add(Flags newval)          { add(BRIDGE_DATATYPE_FLAGS,   newval.get()); }
      void add(PointingButton newval) { add(BRIDGE_DATATYPE_POINTINGBUTTON, newval.get()); }

    private:
      size_t index_;
      FromKeyChecker fromkeychecker_;
      PairKeyFlags fromKey_;
      Vector_PairPointingButtonFlags* toButtons_;
    };
  }
}

#endif
