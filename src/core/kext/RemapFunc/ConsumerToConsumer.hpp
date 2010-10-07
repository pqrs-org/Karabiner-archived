#ifndef CONSUMERTOCONSUMER_HPP
#define CONSUMERTOCONSUMER_HPP

#include "RemapFuncBase.hpp"
#include "FromKeyChecker.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace RemapFunc {
    class ConsumerToConsumer {
    public:
      ConsumerToConsumer(void) : index_(0), toKeys_(NULL) {}
      bool remap(RemapConsumerParams& remapParams);
      void initialize(void);
      void terminate(void);

      // ----------------------------------------
      // [0] => fromKey_
      // [1] => toKeys_[0]
      // [2] => toKeys_[1]
      // [3] => ...
      void add(unsigned int datatype, unsigned int newval);

      // ----------------------------------------
      // utility functions
      void add(ConsumerKeyCode newval) { add(BRIDGE_DATATYPE_CONSUMERKEYCODE, newval.get()); }
      void add(Flags newval)           { add(BRIDGE_DATATYPE_FLAGS,           newval.get()); }

      bool call_remap_with_VK_PSEUDO_KEY(EventType eventType);

    private:
      size_t index_;
      FromKeyChecker fromkeychecker_;
      PairConsumerKeyFlags fromKey_;
      Vector_PairConsumerKeyFlags* toKeys_;
    };
  }
}

#endif
