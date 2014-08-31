#ifndef BLOCKUNTILKEYUP_HPP
#define BLOCKUNTILKEYUP_HPP

#include "RemapFuncBase.hpp"

namespace org_pqrs_Karabiner {
  namespace RemapFunc {
    class BlockUntilKeyUp : RemapFuncBase {
    public:
      BlockUntilKeyUp(void) :
        RemapFuncBase(BRIDGE_REMAPTYPE_BLOCKUNTILKEYUP)
      {}

      void add(AddDataType datatype, AddValue newval);
      const FromEvent& getFromEvent(void) const { return fromEvent_; }

    private:
      FromEvent fromEvent_;
    };
  }
}

#endif
