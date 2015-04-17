#ifndef DROPKEYAFTERREMAP_HPP
#define DROPKEYAFTERREMAP_HPP

#include "RemapFuncBase.hpp"

namespace org_pqrs_Karabiner {
namespace RemapFunc {
class DropKeyAfterRemap final : public RemapFuncBase {
public:
  DropKeyAfterRemap(AutogenId autogenId) : RemapFuncBase(BRIDGE_REMAPTYPE_DROPKEYAFTERREMAP, autogenId) {}

  bool drop(const Params_KeyboardEventCallBack& params) override;

  // ----------------------------------------
  // [0] => fromKey_
  void add(AddDataType datatype, AddValue newval) override;

private:
  FromEvent fromEvent_;
  Vector_ModifierFlag fromModifierFlags_;
};
}
};

#endif
