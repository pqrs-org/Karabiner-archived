#ifndef HOLDINGKEYTOKEY_HPP
#define HOLDINGKEYTOKEY_HPP

#include "FromEvent.hpp"
#include "RemapFuncClasses.hpp"
#include "DependingPressingPeriodKeyToKey.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace RemapFunc {
    class HoldingKeyToKey {
    public:
      HoldingKeyToKey(void);
      ~HoldingKeyToKey(void);

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
      void add(AddDataType datatype, AddValue newval);

    private:
      size_t index_;
      bool index_is_holding_;
      FromEvent fromEvent_;

      DependingPressingPeriodKeyToKey dppkeytokey_;
    };
  }
}

#endif
