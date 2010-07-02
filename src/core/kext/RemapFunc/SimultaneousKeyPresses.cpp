#include "SimultaneousKeyPresses.hpp"
#include "EventInputQueue.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace RemapFunc {
    void
    SimultaneousKeyPresses::initialize(void)
    {
      keytokey_.initialize();
    }

    void
    SimultaneousKeyPresses::terminate(void)
    {
      keytokey_.terminate();
    }

    void
    SimultaneousKeyPresses::add(KeyCode newval)
    {
      switch (index_) {
        case 0:
          virtualkey_ = newval;
          break;

        case 1:
          fromKey1_ = newval;
          break;

        case 2:
          fromKey2_ = newval;
          break;

        case 3:
          // pass-through (== no break)
          keytokey_.add(virtualkey_);
          keytokey_.add(fromFlags_);
          toKey_raw_ = newval;
        default:
          keytokey_.add(newval);
          break;
      }
      ++index_;
    }

    void
    SimultaneousKeyPresses::add(Flags newval)
    {
      switch (index_) {
        case 0:
        case 1:
          IOLOG_ERROR("Invalid SimultaneousKeyPresses::add\n");
          break;

        case 2:
        case 3:
          fromFlags_ = newval;
          break;

        default:
          keytokey_.add(newval);
          break;
      }
    }

    void
    SimultaneousKeyPresses::add(SimultaneousKeyPresses::Option newval)
    {
      switch (newval) {
        case OPTION_RAW:
          isToRawKey_ = true;
          break;
      }
    }

    void
    SimultaneousKeyPresses::remap(void)
    {
      if (! EventInputQueue::queue_) return;

      // ------------------------------------------------------------
      if (active1_ || active2_) {
        // restore KeyUp Event (fromKey1_).
        // drop    KeyUp Event (fromKey2_).

        for (EventInputQueue::Item* p = static_cast<EventInputQueue::Item*>(EventInputQueue::queue_->front()); p; p = static_cast<EventInputQueue::Item*>(p->getnext())) {
          if (p->dropped) continue;
          if (! p->params_KeyboardEventCallBack) continue;
          Params_KeyboardEventCallBack& params = *(p->params_KeyboardEventCallBack);

          if ((params.eventType).isKeyDownOrModifierDown(params.key, params.flags)) continue;

          if (active1_ && params.key == fromKey1_) {
            push_remapped(params, false);

            p->dropped = true;
            active1_ = false;
          }
          if (active2_ && params.key == fromKey2_) {
            p->dropped = true;
            active2_ = false;
          }
        }
      }

      // ------------------------------------------------------------
      // find fromKeyCode*_
      //
      // We consider "A+B to C".
      // In this case, we change key in case1, but don't change key in case2.
      // Because A is released in case2.
      //
      // case1) "KeyDown A", "KeyDown B"             (=> change key)
      // case2) "KeyDown A", "KeyUp A", "KeyDown B". (=> don't change key)
      //
      // So, we check keys from tail.

      EventInputQueue::Item* item1 = NULL;
      EventInputQueue::Item* item2 = NULL;
      EventInputQueue::Item* base = NULL; // later item of item1 or item2

      for (EventInputQueue::Item* p = static_cast<EventInputQueue::Item*>(EventInputQueue::queue_->back()); p; p = static_cast<EventInputQueue::Item*>(p->getprev())) {
        if (p->dropped) continue;
        if (! p->params_KeyboardEventCallBack) continue;
        Params_KeyboardEventCallBack& params = *(p->params_KeyboardEventCallBack);

        if (params.key == fromKey1_ && ! item1) {
          item1 = p;
          if (! base) base = p;
        }
        if (params.key == fromKey2_ && ! item2) {
          item2 = p;
          if (! base) base = p;
        }
      }

      if (! item1 || ! item2 || ! base) return;

      Params_KeyboardEventCallBack& params1 = *(item1->params_KeyboardEventCallBack);
      Params_KeyboardEventCallBack& params2 = *(item2->params_KeyboardEventCallBack);
      Params_KeyboardEventCallBack& paramsbase = *(base->params_KeyboardEventCallBack);

      // replace first fromKeyCode1. and drop first fromKeyCode2
      if (params1.eventType.isKeyDownOrModifierDown(params1.key, params1.flags) &&
          params2.eventType.isKeyDownOrModifierDown(params2.key, params2.flags) &&
          FlagStatus::makeFlags().isOn(fromFlags_)) {
        item1->dropped = true;
        active1_ = true;

        item2->dropped = true;
        active2_ = true;

        push_remapped(paramsbase, true);
      }

      return;
    }

    void
    SimultaneousKeyPresses::push_remapped(const Params_KeyboardEventCallBack& baseparams, bool isKeyDown)
    {
      EventType eventType = isKeyDown ? EventType::DOWN : EventType::UP;

      KeyCode key = virtualkey_;
      if (isToRawKey_) {
        key = toKey_raw_;
      }
      if (key == KeyCode::VK_NONE) return;

      // ----------------------------------------
      Params_KeyboardEventCallBack::auto_ptr ptr(Params_KeyboardEventCallBack::alloc(baseparams));
      if (! ptr) return;

      Params_KeyboardEventCallBack& params = *ptr;
      params.eventType = eventType;
      params.key = key;

      EventInputQueue::enqueue_(params);
    }

    bool
    SimultaneousKeyPresses::handlevirtualkey(const Params_KeyboardEventCallBack& params)
    {
      RemapParams rp(params);
      return keytokey_.remap(rp);
    }
  }
}
