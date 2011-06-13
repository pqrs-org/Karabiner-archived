#ifndef VIRTUALKEY_HPP
#define VIRTUALKEY_HPP

#include "CallbackWrapper.hpp"
#include "KeyCode.hpp"
#include "RemapClass.hpp"
#include "TimerWrapper.hpp"
#include "Vector.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace VirtualKey {
    void initialize(IOWorkLoop& workloop);
    void terminate(void);
    void reset(void);
    bool handle(const Params_KeyboardEventCallBack& params);

    bool isKeyLikeModifier(KeyCode keycode);
  };
}

#endif
