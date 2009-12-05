#ifndef KEYBOARDREPEAT_HPP
#define KEYBOARDREPEAT_HPP

#include "base.hpp"
#include "bridge.hpp"
#include "keycode.hpp"
#include "Config.hpp"
#include "CallbackWrapper.hpp"
#include "TimerWrapper.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  class KeyboardRepeat {
  public:
    KeyboardRepeat(void) {}

    static void initialize(IOWorkLoop& workloop);
    static void terminate(void);

    static void setTS(const AbsoluteTime& ts) { params_.ts = ts; }
    static void setWorkSpaceData(const KeyRemap4MacBook_bridge::GetWorkspaceData::Reply& workspacedata) { workspacedata_ = workspacedata; }

    static void set(const EventType& eventType = EventType::DOWN,
                    const Flags& flags = 0,
                    const KeyCode& key = KeyCode::VK_NONE,
                    const KeyboardType& keyboardType = KeyboardType::MACBOOK,
                    int wait = config.get_repeat_initial_wait());

    static void set(const Params_KeyboardEventCallBack& p,
                    int wait = config.get_repeat_initial_wait()) {
      set(p.eventType, p.flags, p.key, p.keyboardType, wait);
    }

    static void cancel(void) {
      set();
    }

  private:
    static void fire(OSObject* owner, IOTimerEventSource* sender);

    static TimerWrapper timer_;
    static Params_KeyboardEventCallBack params_;
    static KeyRemap4MacBook_bridge::GetWorkspaceData::Reply workspacedata_;
  };
}

#endif
