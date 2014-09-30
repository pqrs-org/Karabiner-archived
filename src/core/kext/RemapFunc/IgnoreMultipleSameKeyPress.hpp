#ifndef IGNOREMULTIPLESAMEKEYPRESS_HPP
#define IGNOREMULTIPLESAMEKEYPRESS_HPP

#include "RemapFuncBase.hpp"

namespace org_pqrs_Karabiner {
namespace RemapFunc {
// ex. Ignore JIS_KANA x 2. (validate only the first once)
class IgnoreMultipleSameKeyPress : public RemapFuncBase {
public:
  IgnoreMultipleSameKeyPress(void) : RemapFuncBase(BRIDGE_REMAPTYPE_IGNOREMULTIPLESAMEKEYPRESS),
                                     needToIgnore_(false) {}

  bool remap(RemapParams& remapParams);

  // ----------------------------------------
  // [0] => fromKey_
  void add(AddDataType datatype, AddValue newval);

private:
  FromEvent fromEvent_;
  Vector_ModifierFlag fromModifierFlags_;
  bool needToIgnore_;
};
}
}

#endif
