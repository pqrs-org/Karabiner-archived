#ifndef DROPSCROLLWHEEL_HPP
#define DROPSCROLLWHEEL_HPP

#include "RemapFuncBase.hpp"

namespace org_pqrs_Karabiner {
namespace RemapFunc {
class DropScrollWheel final : public RemapFuncBase {
public:
  DropScrollWheel(AutogenId autogenId) : RemapFuncBase(BRIDGE_REMAPTYPE_DROPSCROLLWHEEL, autogenId),
                                         dropHorizontalScroll_(false),
                                         dropMomentumScroll_(false) {}

  bool remap(RemapParams& remapParams) override;

  void add(AddDataType datatype, AddValue newval) override;

private:
  bool dropHorizontalScroll_;
  bool dropMomentumScroll_;
};
}
}

#endif
