#ifndef KEYTOKEY_HPP
#define KEYTOKEY_HPP

#include "bridge.h"
#include "RemapFuncClasses.hpp"
#include "FromKeyChecker.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace RemapFunc {
    class KeyToKey {
    public:
      KeyToKey(void);
      ~KeyToKey(void);

      bool remap(RemapParams& remapParams);

      // ----------------------------------------
      // [0] => fromKey_
      // [1] => toKeys_[0]
      // [2] => toKeys_[1]
      // [3] => ...
      void add(unsigned int datatype, unsigned int newval);

      // ----------------------------------------
      // utility functions
      void add(KeyCode newval) { add(BRIDGE_DATATYPE_KEYCODE, newval.get()); }
      void add(Flags newval)   { add(BRIDGE_DATATYPE_FLAGS,   newval.get()); }
      void add(Option newval)  { add(BRIDGE_DATATYPE_OPTION,  newval.get()); }

      bool call_remap_with_VK_PSEUDO_KEY(EventType eventType);

      void disabled_callback(void);

    private:
      void retractInput(void);
      void restoreInput(void);
      int getDelayUntilRepeat(void);
      int getKeyRepeat(void);

      size_t index_;
      FromKeyChecker fromkeychecker_;
      PairKeyFlags fromKey_;
      Vector_PairKeyFlags toKeys_;
      Vector_PairKeyFlags beforeKeys_;
      Vector_PairKeyFlags afterKeys_;
      // Pointer to toKeys_ or beforeKeys_ or afterKeys_.
      Vector_PairKeyFlags* currentVectorPointer_;
      int keyboardRepeatID_;
      bool isRepeatEnabled_;

      int delayUntilRepeat_;
      int keyRepeat_;
    };
  }
}

#endif
