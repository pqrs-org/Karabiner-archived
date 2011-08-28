#ifndef VIRTUALKEY_VK_CONSUMERKEY_HPP
#define VIRTUALKEY_VK_CONSUMERKEY_HPP

#include "ConsumerToConsumer.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace VirtualKey {
    class VK_CONSUMERKEY {
    public:
      static bool handle(const Params_KeyboardEventCallBack& params);
    };
  }
}

#endif
