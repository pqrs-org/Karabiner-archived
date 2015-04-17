#ifndef IGNOREMULTIPLESAMEKEYPRESS_HPP
#define IGNOREMULTIPLESAMEKEYPRESS_HPP

#include "RemapFuncBase.hpp"

namespace org_pqrs_Karabiner {
namespace RemapFunc {
// ex. Ignore JIS_KANA x 2. (validate only the first once)
class IgnoreMultipleSameKeyPress final : public RemapFuncBase {
public:
  IgnoreMultipleSameKeyPress(AutogenId autogenId) : RemapFuncBase(BRIDGE_REMAPTYPE_IGNOREMULTIPLESAMEKEYPRESS, autogenId),
                                                    needToIgnore_(false) {}

  bool remap(RemapParams& remapParams) override;

  // ----------------------------------------
  // [0] => fromKey_
  void add(AddDataType datatype, AddValue newval) override;

private:
  FromEvent fromEvent_;
  Vector_ModifierFlag fromModifierFlags_;
  bool needToIgnore_;
};
}
}

#endif
