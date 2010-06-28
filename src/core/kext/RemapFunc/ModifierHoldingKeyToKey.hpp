#ifndef MODIFIERHOLDINGKEYTOKEY_HPP
#define MODIFIERHOLDINGKEYTOKEY_HPP

#include "RemapFuncBase.hpp"
#include "FromKeyChecker.hpp"
#include "IntervalChecker.hpp"
#include "KeyToKey.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace RemapFunc {
    // Modifier Holding + Key -> Key
    class ModifierHoldingKeyToKey {
    public:
      ModifierHoldingKeyToKey(void) : index_(0) {}
      void initialize(void);
      void terminate(void);
      bool remap(RemapParams& remapParams);

      // ----------------------------------------
      // [0] => fromKey_
      // [1] => toKeys_[0]
      // [2] => toKeys_[1]
      // [3] => ...
      void add(KeyCode newval);
      void add(Flags newval);

    private:
      size_t index_;
      FromKeyChecker fromkeychecker_;
      PairKeyFlags fromKey_;
      IntervalChecker ic_;

      KeyToKey keytokey_;
    };
  }
}

#endif
