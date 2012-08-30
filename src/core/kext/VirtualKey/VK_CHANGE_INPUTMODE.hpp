#ifndef VIRTUALKEY_VK_CHANGE_INPUTMODE_HPP
#define VIRTUALKEY_VK_CHANGE_INPUTMODE_HPP

namespace org_pqrs_KeyRemap4MacBook {
  namespace VirtualKey {
    class VK_CHANGE_INPUTMODE {
    public:
      static bool handle(const Params_KeyboardEventCallBack& params);
      static bool handleAfterEnqueued(const Params_KeyboardEventCallBack& params);
    };
  }
}

#endif
