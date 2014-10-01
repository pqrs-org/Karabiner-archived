#ifndef FLIPSCROLLWHEEL_HPP
#define FLIPSCROLLWHEEL_HPP

#include "RemapFuncBase.hpp"

namespace org_pqrs_Karabiner {
namespace RemapFunc {
class FlipScrollWheel final : public RemapFuncBase {
public:
  FlipScrollWheel(void) : RemapFuncBase(BRIDGE_REMAPTYPE_FLIPSCROLLWHEEL),
                          flipHorizontalScroll_(false),
                          flipVerticalScroll_(false) {}

  bool remap(RemapParams& remapParams) override;

  void add(AddDataType datatype, AddValue newval) override;

private:
  bool flipHorizontalScroll_;
  bool flipVerticalScroll_;
};
}
}

#endif
