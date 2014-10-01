#ifndef FORCENUMLOCKON_HPP
#define FORCENUMLOCKON_HPP

#include "RemapFuncBase.hpp"

namespace org_pqrs_Karabiner {
namespace RemapFunc {
class ForceNumLockOn final : public RemapFuncBase {
public:
  ForceNumLockOn(void) : RemapFuncBase(BRIDGE_REMAPTYPE_FORCENUMLOCKON),
                         index_(0) {}

  bool remapForceNumLockOn(ListHookedKeyboard::Item* item);

  // ----------------------------------------
  // [0] => DeviceVendor
  // [1] => DeviceProduct
  void add(AddDataType datatype, AddValue newval);

private:
  size_t index_;
  DeviceIdentifier deviceIdentifier_;
};
}
}

#endif
