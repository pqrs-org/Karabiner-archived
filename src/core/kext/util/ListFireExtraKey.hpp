#ifndef LISTFIREEXTRAKEY_HPP
#define LISTFIREEXTRAKEY_HPP

#include "keycode.hpp"
#include "bridge.hpp"
#include "CallBackWrapper.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace ListFireExtraKey {
    void reset(void);
    bool isEmpty(void);
    void add(unsigned int eventType, unsigned int flags, unsigned int key);
    void fire(KeyboardEventCallback callback, const Params_KeyboardEventCallBack& params, const KeyRemap4MacBook_bridge::GetWorkspaceData::Reply& workspacedata);

    // utility
    void addKey(unsigned int flags, KeyCode::KeyCode keyCode);
  }
}

#endif
