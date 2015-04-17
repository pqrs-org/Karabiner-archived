#ifndef BLOCKUNTILKEYUP_HPP
#define BLOCKUNTILKEYUP_HPP

#include "RemapFuncBase.hpp"

namespace org_pqrs_Karabiner {
namespace RemapFunc {
class BlockUntilKeyUp final : public RemapFuncBase {
public:
  BlockUntilKeyUp(AutogenId autogenId) : RemapFuncBase(BRIDGE_REMAPTYPE_BLOCKUNTILKEYUP, autogenId) {}

  void add(AddDataType datatype, AddValue newval) override;
  const FromEvent* getBlockUntilKeyUpFromEvent(void) const override { return &fromEvent_; }

private:
  FromEvent fromEvent_;
};
}
}

#endif
