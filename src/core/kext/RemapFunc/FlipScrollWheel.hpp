#ifndef FLIPSCROLLWHEEL_HPP
#define FLIPSCROLLWHEEL_HPP

#include "RemapFuncBase.hpp"

namespace org_pqrs_Karabiner {
namespace RemapFunc {
class FlipScrollWheel final : public RemapFuncBase {
public:
  FlipScrollWheel(AutogenId autogenId) : RemapFuncBase(BRIDGE_REMAPTYPE_FLIPSCROLLWHEEL, autogenId),
                                         flipHorizontalScroll_(false),
                                         flipVerticalScroll_(false),
                                         rotate_(false) {}

  bool remap(RemapParams& remapParams) override;

  void add(AddDataType datatype, AddValue newval) override;

private:
  bool flipHorizontalScroll_;
  bool flipVerticalScroll_;
  bool rotate_;
};
}
}

#endif
