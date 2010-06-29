#ifndef KEYTOKEY_HPP
#define KEYTOKEY_HPP

#include "RemapFuncBase.hpp"
#include "FromKeyChecker.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace RemapFunc {
    class KeyToKey {
    public:
      KeyToKey(void) : index_(0), toKeys_(NULL) {}
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

      // ----------------------------------------
      // utility functions
      bool call_remap_with_VK_PSEUDO_KEY(EventType eventType);

    private:
      size_t index_;
      FromKeyChecker fromkeychecker_;
      PairKeyFlags fromKey_;
      Vector_PairKeyFlags* toKeys_;
    };
  }
}

#endif
