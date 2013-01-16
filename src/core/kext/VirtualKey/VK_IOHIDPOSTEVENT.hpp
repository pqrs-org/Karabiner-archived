#ifndef VIRTUALKEY_VK_IOHIDPOSTEVENT_HPP
#define VIRTUALKEY_VK_IOHIDPOSTEVENT_HPP

#include "RemapClass.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace VirtualKey {
    class VK_IOHIDPOSTEVENT {
    public:
      static bool handle(const Params_KeyboardEventCallBack& params);
      static bool handleAfterEnqueued(const Params_KeyboardEventCallBack& params);

      static void post(const Params_KeyboardSpecialEventCallback& params);
    };
  }
}

#endif
