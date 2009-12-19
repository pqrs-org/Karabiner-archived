#ifndef KEYBOARDREPEAT_HPP
#define KEYBOARDREPEAT_HPP

#include "base.hpp"
#include "bridge.hpp"
#include "KeyCode.hpp"
#include "Config.hpp"
#include "CallbackWrapper.hpp"
#include "TimerWrapper.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  class KeyboardRepeat {
  public:
    KeyboardRepeat(void) {}

    static void initialize(IOWorkLoop& workloop);
    static void terminate(void);

    static void setWorkSpaceData(const KeyRemap4MacBook_bridge::GetWorkspaceData::Reply& workspacedata) { workspacedata_ = workspacedata; }

    static void cancel(void);

    // --------------------------------------------------
    static void set(const EventType& eventType,
                    const Flags& flags,
                    const KeyCode& key,
                    const KeyboardType& keyboardType,
                    int wait);

    static void set(const Params_KeyboardEventCallBack& p,
                    int wait = config.get_repeat_initial_wait()) {
      set(p.eventType, p.flags, p.key, p.keyboardType, wait);
    }

    // --------------------------------------------------
    // primitive operations.
    static void primitive_add(const EventType& eventType,
                              const Flags& flags,
                              const KeyCode& key,
                              const KeyboardType& keyboardType);
    static void primitive_start(int wait = config.get_repeat_initial_wait());

  private:
    enum {
      MAXNUM = 16,
    };
    struct Item {
      Item(void) : params(EventType(0), Flags(0), KeyCode(0), KeyboardType(0), true), active(false) {}

      Params_KeyboardEventCallBack params;
      bool active;
    };

    // ------------------------------------------------------------
    static void fire(OSObject* owner, IOTimerEventSource* sender);

    static void cancel_nolock(void);

    static void primitive_add_nolock(const EventType& eventType,
                                     const Flags& flags,
                                     const KeyCode& key,
                                     const KeyboardType& keyboardType);
    static void primitive_start_nolock(int wait);

    static int getActiveItemNum(void);

    static TimerWrapper timer_;
    static Item item_[MAXNUM];
    static KeyRemap4MacBook_bridge::GetWorkspaceData::Reply workspacedata_;
  };
}

#endif
