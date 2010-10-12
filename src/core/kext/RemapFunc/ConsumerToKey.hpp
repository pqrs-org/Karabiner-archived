#ifndef CONSUMERTOKEY_HPP
#define CONSUMERTOKEY_HPP

#include "RemapFuncBase.hpp"
#include "KeyToKey.hpp"
#include "ConsumerToConsumer.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace RemapFunc {
    class ConsumerToKey {
    public:
      ConsumerToKey(void);
      ~ConsumerToKey(void);

      bool remap(RemapConsumerParams& remapParams);

      // [0] => fromKey
      // [1] => toKeys[0]
      // [2] => toKeys[1]
      // [3] => ...
      void add(unsigned int datatype, unsigned int newval);

    private:
      size_t index_;
      FromKeyChecker fromkeychecker_;
      PairConsumerKeyFlags fromKey_;

      KeyToKey keytokey_;
      ConsumerToConsumer consumertoconsumer_;
    };
  }
}

#endif
