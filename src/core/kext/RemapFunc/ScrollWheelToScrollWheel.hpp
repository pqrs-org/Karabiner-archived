#ifndef SCROLLWHEELTOSCROLLWHEEL_HPP
#define SCROLLWHEELTOSCROLLWHEEL_HPP

#include "RemapFuncBase.hpp"

namespace org_pqrs_Karabiner {
namespace RemapFunc {
class ScrollWheelToScrollWheel final : public RemapFuncBase {
public:
  ScrollWheelToScrollWheel(void) : RemapFuncBase(BRIDGE_REMAPTYPE_SCROLLWHEELTOSCROLLWHEEL),
                                   index_(0) {}

  bool remap(RemapParams& remapParams);

  void add(AddDataType datatype, AddValue newval);

private:
  size_t index_;
  Vector_ModifierFlag fromModifierFlags_;
  Vector_ModifierFlag toModifierFlags_;
};
}
}

#endif
