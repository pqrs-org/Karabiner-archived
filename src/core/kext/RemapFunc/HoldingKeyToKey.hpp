#ifndef HOLDINGKEYTOKEY_HPP
#define HOLDINGKEYTOKEY_HPP

#include "RemapFuncBase.hpp"
#include "FromKeyChecker.hpp"
#include "KeyToKey.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace RemapFunc {
    class HoldingKeyToKey {
    public:
      static void static_initialize(IOWorkLoop& workloop);
      static void static_terminate(void);

      HoldingKeyToKey(void) : index_(0), index_is_holding_(false) {}
      void initialize(void);
      void terminate(void);
      bool remap(RemapParams& remapParams);

      // ----------------------------------------
      // [0]   => fromKey_
      // [1]   => toKeys_normal_[0]
      // [2]   => toKeys_normal_[1]
      // [3]   => ...
      // [n]   => KeyCode::VK_NONE
      // [n+1] => toKeys_holding_[0]
      // [n+2] => toKeys_holding_[1]
      // [n+3] => ...
      void add(KeyCode newval);
      void add(Flags newval);

    private:
      static void fireholding(OSObject* owner, IOTimerEventSource* sender);

      static TimerWrapper timer_;
      static HoldingKeyToKey* target_;
      static bool isfirenormal_;
      static bool isfireholding_;

      size_t index_;
      bool   index_is_holding_;
      PairKeyFlags fromKey_;
      KeyToKey keytokey_drop_;
      KeyToKey keytokey_normal_;
      KeyToKey keytokey_holding_;
    };
  }
}

#endif
