#ifndef SETKEYBOARDTYPE_HPP
#define SETKEYBOARDTYPE_HPP

#include "RemapFuncBase.hpp"

namespace org_pqrs_Karabiner {
namespace RemapFunc {
class SetKeyboardType final : public RemapFuncBase {
public:
  SetKeyboardType(AutogenId autogenId) : RemapFuncBase(BRIDGE_REMAPTYPE_SETKEYBOARDTYPE, autogenId),
                                         toKeyboardType_(KeyboardType::MACBOOK) {}

  bool remapSetKeyboardType(KeyboardType& keyboardType) override;

  // ----------------------------------------
  // [0] => toKeyboardType
  void add(AddDataType datatype, AddValue newval) override;

private:
  KeyboardType toKeyboardType_;
};
}
}

#endif
