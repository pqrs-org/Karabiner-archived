#ifndef FORCENUMLOCKON_HPP
#define FORCENUMLOCKON_HPP

#include "RemapFuncBase.hpp"

namespace org_pqrs_Karabiner {
namespace RemapFunc {
class ForceNumLockOn final : public RemapFuncBase {
public:
  ForceNumLockOn(AutogenId autogenId) : RemapFuncBase(BRIDGE_REMAPTYPE_FORCENUMLOCKON, autogenId),
                                        index_(0), forceOffMode_(false) {}

  bool remapForceNumLockOn(ListHookedKeyboard::Item* item) override;

  // ----------------------------------------
  // [0] => DeviceVendor
  // [1] => DeviceProduct
  void add(AddDataType datatype, AddValue newval) override;

private:
  size_t index_;
  DeviceIdentifier deviceIdentifier_;
  bool forceOffMode_;
};
}
}

#endif
