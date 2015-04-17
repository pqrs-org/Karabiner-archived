#ifndef KEYOVERLAIDMODIFIER_HPP
#define KEYOVERLAIDMODIFIER_HPP

#include "DependingPressingPeriodKeyToKey.hpp"
#include "RemapFuncBase.hpp"

namespace org_pqrs_Karabiner {
namespace RemapFunc {
// for SandS like behavior remappings (remap_space2shift, remap_enter2optionL_commandSpace, ...)
class KeyOverlaidModifier final : public RemapFuncBase {
public:
  KeyOverlaidModifier(AutogenId autogenId) : RemapFuncBase(BRIDGE_REMAPTYPE_KEYOVERLAIDMODIFIER, autogenId),
                                             isUseSeparator_(false),
                                             indexType_(INDEX_IS_HOLDING),
                                             index_(0),
                                             dppkeytokey_(this, autogenId) {
    dppkeytokey_.setPeriodMS(DependingPressingPeriodKeyToKey::PeriodMS::Mode::KEY_OVERLAID_MODIFIER);
  }

  void prepare(RemapParams& remapParams) override { dppkeytokey_.prepare(remapParams); }
  bool remap(RemapParams& remapParams) override { return dppkeytokey_.remap(remapParams); }

  // ----------------------------------------
  // [0]   => fromKey_
  // [1]   => toKey_
  // [2]   => toKeys_fire_[0]
  // [3]   => toKeys_fire_[1]
  // [4]   => toKeys_fire_[2]
  // [5]   => ...
  void add(AddDataType datatype, AddValue newval) override;

  // utility
  void add(KeyCode newval) { add(AddDataType(BRIDGE_DATATYPE_KEYCODE), AddValue(newval.get())); }
  void add(Option newval) { add(AddDataType(BRIDGE_DATATYPE_OPTION), AddValue(newval.get())); }

private:
  void addToDependingPressingPeriodKeyToKey(AddDataType datatype, AddValue newval);

  bool isUseSeparator_;
  enum IndexType {
    INDEX_IS_HOLDING,
    INDEX_IS_NORMAL,
    INDEX_IS_REPEAT_TOKEYS,
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
