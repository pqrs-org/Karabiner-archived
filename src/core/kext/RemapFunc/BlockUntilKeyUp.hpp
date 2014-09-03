#ifndef BLOCKUNTILKEYUP_HPP
#define BLOCKUNTILKEYUP_HPP

#include "RemapFuncBase.hpp"

namespace org_pqrs_Karabiner {
  namespace RemapFunc {
    class BlockUntilKeyUp : public RemapFuncBase {
    public:
      BlockUntilKeyUp(void) :
        RemapFuncBase(BRIDGE_REMAPTYPE_BLOCKUNTILKEYUP)
      {}

      void add(AddDataType datatype, AddValue newval);
      const FromEvent* getBlockUntilKeyUpFromEvent(void) const { return &fromEvent_; }

    private:
      FromEvent fromEvent_;
    };
  }
}

#endif
