#ifndef SCROLLWHEELTOSCROLLWHEEL_HPP
#define SCROLLWHEELTOSCROLLWHEEL_HPP

#include "RemapFuncBase.hpp"

namespace org_pqrs_Karabiner {
namespace RemapFunc {
class ScrollWheelToScrollWheel final : public RemapFuncBase {
public:
  ScrollWheelToScrollWheel(AutogenId autogenId) : RemapFuncBase(BRIDGE_REMAPTYPE_SCROLLWHEELTOSCROLLWHEEL, autogenId),
                                                  index_(0) {}

  bool remap(RemapParams& remapParams) override;

  void add(AddDataType datatype, AddValue newval) override;

private:
  size_t index_;
  Vector_ModifierFlag fromModifierFlags_;
  Vector_ModifierFlag toModifierFlags_;
};
}
}

#endif
