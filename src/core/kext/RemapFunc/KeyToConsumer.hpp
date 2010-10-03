#ifndef KEYTOCONSUMER_HPP
#define KEYTOCONSUMER_HPP

#include "RemapFuncBase.hpp"
#include "KeyToKey.hpp"
#include "ConsumerToConsumer.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace RemapFunc {
    class KeyToConsumer {
    public:
      KeyToConsumer(void) : index_(0) {}
      bool remap(RemapParams& remapParams);
      void initialize(void);
      void terminate(void);

      // ----------------------------------------
      // [0] => fromKey
      // [1] => toKeys[0]
      // [2] => toKeys[1]
      // [3] => ...
      void add(unsigned int datatype, unsigned int newval);

    private:
      size_t index_;
      FromKeyChecker fromkeychecker_;
      PairKeyFlags fromKey_;

      KeyToKey keytokey_;
      ConsumerToConsumer consumertoconsumer_;
    };
  }
}

#endif
