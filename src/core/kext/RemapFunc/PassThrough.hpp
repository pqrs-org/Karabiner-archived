#ifndef PASSTHROUGH_HPP
#define PASSTHROUGH_HPP

#include "RemapFuncBase.hpp"

namespace org_pqrs_Karabiner {
namespace RemapFunc {
class PassThrough : public RemapFuncBase {
public:
  PassThrough(void) : RemapFuncBase(BRIDGE_REMAPTYPE_PASSTHROUGH) {}

  void add(AddDataType datatype, AddValue newval) {}
};
}
}

#endif
