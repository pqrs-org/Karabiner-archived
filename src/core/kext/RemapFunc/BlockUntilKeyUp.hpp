#ifndef BLOCKUNTILKEYUP_HPP
#define BLOCKUNTILKEYUP_HPP

#include "FromEvent.hpp"
#include "ParamsUnion.hpp"
#include "Types.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace RemapFunc {
    class BlockUntilKeyUp {
    public:
      void add(AddDataType datatype, AddValue newval);
      bool isTargetEvent(const ParamsUnion& paramsUnion);
      const FromEvent& getFromEvent(void) const { return fromEvent_; }

    private:
      FromEvent fromEvent_;
      Vector_ModifierFlag fromModifierFlags_;
    };
  }
}

#endif
