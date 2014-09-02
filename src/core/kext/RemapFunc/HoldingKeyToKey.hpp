#ifndef HOLDINGKEYTOKEY_HPP
#define HOLDINGKEYTOKEY_HPP

#include "DependingPressingPeriodKeyToKey.hpp"
#include "RemapFuncBase.hpp"

namespace org_pqrs_Karabiner {
  namespace RemapFunc {
    class HoldingKeyToKey : RemapFuncBase {
    public:
      HoldingKeyToKey(void) :
        RemapFuncBase(BRIDGE_REMAPTYPE_HOLDINGKEYTOKEY),
        isUseSeparator_(false),
        indexType_(INDEX_IS_NORMAL),
        index_(0)
      {
        dppkeytokey_.setPeriodMS(DependingPressingPeriodKeyToKey::PeriodMS::Mode::HOLDING_KEY_TO_KEY);
      }

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
      void addToDependingPressingPeriodKeyToKey(AddDataType datatype, AddValue newval);

      bool isUseSeparator_;
      enum IndexType {
        INDEX_IS_NORMAL,
        INDEX_IS_HOLDING,
        INDEX_IS_KEYTOKEY_BEFORE_KEYDOWN,
        INDEX_IS_KEYTOKEY_AFTER_KEYUP,
      } indexType_;
      size_t index_;
      FromEvent fromEvent_;

      DependingPressingPeriodKeyToKey dppkeytokey_;
    };
  }
}

#endif
