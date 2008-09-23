#ifndef LISTFIREEXTRAKEY_HPP
#define LISTFIREEXTRAKEY_HPP

#include "keycode.hpp"
#include "CallBackWrapper.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace ListFireExtraKey {
    void reset(void);
    bool isEmpty(void);
    void add(unsigned int eventType, unsigned int flags, unsigned int key);
    void fire(KeyboardEventCallback callback, const Params_KeyboardEventCallBack &params);

    // utility
    void addKey(unsigned int flags, KeyCode::KeyCode keyCode);
  }
}

#endif
