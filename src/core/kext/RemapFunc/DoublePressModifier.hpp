#ifndef DOUBLEPRESSMODIFIER_HPP
#define DOUBLEPRESSMODIFIER_HPP

#include "RemapFuncBase.hpp"
#include "IntervalChecker.hpp"
#include "KeyToKey.hpp"

namespace org_pqrs_Karabiner {
namespace RemapFunc {
// A modifier has DoublePressed key action.
class DoublePressModifier final : public RemapFuncBase {
public:
  DoublePressModifier(AutogenId autogenId) : RemapFuncBase(BRIDGE_REMAPTYPE_DOUBLEPRESSMODIFIER, autogenId),
                                             isUseSeparator_(false),
                                             index_is_double_pressed_(false),
                                             index_(0),
                                             pressCount_(0),
                                             keytokey_(autogenId),
                                             keytokey_fire_(autogenId) {
    ic_.begin();
  }

  bool remap(RemapParams& remapParams) override;

  // ----------------------------------------
  // [0] => fromKey_
  // [1] => toKey_
  // [2] => fireKeys_[0]
  // [3] => fireKeys_[1]
  // [4] => fireKeys_[2]
  // ...
  void add(AddDataType datatype, AddValue newval) override;

private:
  bool isUseSeparator_;
  bool index_is_double_pressed_;
  size_t index_;
  int pressCount_;
  IntervalChecker ic_;

  KeyToKey keytokey_;
  KeyToKey keytokey_fire_;
};
}
}

#endif
