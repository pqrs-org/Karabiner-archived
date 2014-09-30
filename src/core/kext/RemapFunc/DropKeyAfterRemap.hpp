#ifndef DROPKEYAFTERREMAP_HPP
#define DROPKEYAFTERREMAP_HPP

#include "RemapFuncBase.hpp"

namespace org_pqrs_Karabiner {
namespace RemapFunc {
class DropKeyAfterRemap : public RemapFuncBase {
public:
  DropKeyAfterRemap(void) : RemapFuncBase(BRIDGE_REMAPTYPE_DROPKEYAFTERREMAP) {}

  bool drop(const Params_KeyboardEventCallBack& params);

  // ----------------------------------------
  // [0] => fromKey_
  void add(AddDataType datatype, AddValue newval);

private:
  FromEvent fromEvent_;
  Vector_ModifierFlag fromModifierFlags_;
};
}
};

#endif
