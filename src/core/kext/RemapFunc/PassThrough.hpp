#ifndef PASSTHROUGH_HPP
#define PASSTHROUGH_HPP

#include "RemapFuncBase.hpp"

namespace org_pqrs_Karabiner {
namespace RemapFunc {
class PassThrough final : public RemapFuncBase {
public:
  PassThrough(AutogenId autogenId) : RemapFuncBase(BRIDGE_REMAPTYPE_PASSTHROUGH, autogenId) {}

  void add(AddDataType datatype, AddValue newval) override {}
};
}
}

#endif
