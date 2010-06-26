#include "KeyEventInputQueue.hpp"
#include "IOLockWrapper.hpp"
#include "ListHookedKeyboard.hpp"
#include "Config.hpp"
#include "FlagStatus.hpp"
#include "RemapClass.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  Queue* EventInputQueue::queue_ = NULL;
  IntervalChecker EventInputQueue::ic_;
  TimerWrapper EventInputQueue::timer_;

  void
  EventInputQueue::initialize(IOWorkLoop& workloop)
  {
    queue_ = new Queue();
    ic_.begin();
    timer_.initialize(&workloop, NULL, EventInputQueue::fire);
  }

  void
  EventInputQueue::terminate(void)
  {
    timer_.terminate();

    if (queue_) {
      for (;;) {
        EventInputQueue::Item* p = static_cast<EventInputQueue::Item*>(queue_->front());
        if (! p) break;

        queue_->pop();
        delete p;
      }
      delete queue_;
    }
  }

  EventInputQueue::Item::Item(const Params_KeyboardEventCallBack& p, uint32_t d) : dropped(false), delayMS(d)
  {
    params_KeyboardEventCallBack = Params_KeyboardEventCallBack::alloc(p);
  }

  EventInputQueue::Item::~Item(void)
  {
    if (params_KeyboardEventCallBack) {
      delete params_KeyboardEventCallBack;
      params_KeyboardEventCallBack = NULL;
    }
  }

  void
  EventInputQueue::enqueue_(const Params_KeyboardEventCallBack& p)
  {
    if (! queue_) return;

    // Because we handle the key repeat ourself, drop the key repeat.
    if (p.repeat) return;

    // ------------------------------------------------------------
    // calc delay
    uint32_t ms = ic_.getmillisec();
    uint32_t delay = config.get_simultaneouskeypresses_delay();
    if (delay > ms) delay = ms; // min(ms, delay)
    if (delay < MIN_DELAY) delay = MIN_DELAY; // max(MIN_DELAY, delay)
    ic_.begin();

    // --------------------
    Item* newp = new Item(p, delay);
    if (! newp) return;

    newp->dropped = false;
    newp->delayMS = delay;

    queue_->push(newp);
  }

  void
  EventInputQueue::setTimer(void)
  {
    if (! queue_) return;

    Item* front = static_cast<Item*>(queue_->front());
    if (front) {
      timer_.setTimeoutMS(front->delayMS, false);
    }
  }

  void
  EventInputQueue::push(const Params_KeyboardEventCallBack& p)
  {
    IOLockWrapper::ScopedLock lk(timer_.getlock());

    enqueue_(p);

    // remap keys
    size_t num = RemapClassManager::remap_simultaneouskeypresses();

    // if no SimultaneousKeyPresses is enabled, fire immediately.
    if (num == 0) {
      fire_nolock();
    } else {
      setTimer();
    }
  }

  void
  EventInputQueue::fire(OSObject* /*notuse_owner*/, IOTimerEventSource* /*notuse_sender*/)
  {
    IOLockWrapper::ScopedLock lk(timer_.getlock());
    fire_nolock();
  }

  void
  EventInputQueue::fire_nolock(void)
  {
    if (! queue_) return;

    Item* p = static_cast<Item*>(queue_->front());
    if (! p) return;
    queue_->pop();

    if (! p->dropped && p->params_KeyboardEventCallBack) {
      ListHookedKeyboard::hook_KeyboardEventCallback_queued(*(p->params_KeyboardEventCallBack));
    }

    delete p;

    setTimer();
  }

  void
  EventInputQueue::Remap::push_remapped(const Params_KeyboardEventCallBack& baseparams, bool isKeyDown)
  {
    EventType eventType = isKeyDown ? EventType::DOWN : EventType::UP;

    KeyCode key = virtualKeyCode_;
    if (option_ == SimultaneousKeyPresses::Option::RAW) {
      key = toKeyCode1_;
    }
    if (key == KeyCode::VK_NONE) return;

    // ----------------------------------------
    Params_KeyboardEventCallBack::auto_ptr ptr(Params_KeyboardEventCallBack::alloc(baseparams));
    if (! ptr) return;

    Params_KeyboardEventCallBack& params = *ptr;
    params.eventType = eventType;
    params.key = key;

    enqueue_(params);
  }

  void
  EventInputQueue::Remap::remap(void)
  {
    if (! queue_) return;

    // ------------------------------------------------------------
    if (active1_ || active2_) {
      // restore KeyUp Event (fromKeyCode1_).
      // drop KeyUp Event (fromKeyCode2_).

      for (Item* p = static_cast<Item*>(queue_->front()); p; p = static_cast<Item*>(p->getnext())) {
        if (p->dropped) continue;
        if (! p->params_KeyboardEventCallBack) continue;
        Params_KeyboardEventCallBack& params = *(p->params_KeyboardEventCallBack);

        if ((params.eventType).isKeyDownOrModifierDown(params.key, params.flags)) continue;

        if (active1_ && params.key == fromKeyCode1_) {
          push_remapped(params, false);

          p->dropped = true;
          active1_ = false;
        }
        if (active2_ && params.key == fromKeyCode2_) {
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

    Item* item1 = NULL;
    Item* item2 = NULL;
    Item* base = NULL; // later item of item1 or item2

    for (Item* p = static_cast<Item*>(queue_->back()); p; p = static_cast<Item*>(p->getprev())) {
      if (p->dropped) continue;
      if (! p->params_KeyboardEventCallBack) continue;
      Params_KeyboardEventCallBack& params = *(p->params_KeyboardEventCallBack);

      if (params.key == fromKeyCode1_ && ! item1) {
        item1 = p;
        if (! base) base = p;
      }
      if (params.key == fromKeyCode2_ && ! item2) {
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
  }

  bool
  EventInputQueue::Remap::handleVirtualKey(const Params_KeyboardEventCallBack& params)
  {
    if (virtualKeyCode_ != params.key) return false;

    RemapParams rp(params);
    if (toKeyCode2_ == KeyCode::VK_NONE) {
      keytokey_.remap(rp, virtualKeyCode_, fromFlags_,
                      toKeyCode1_, toFlags1_);

    } else {
      keytokey_.remap(rp, virtualKeyCode_, fromFlags_,
                      toKeyCode1_, toFlags1_,
                      toKeyCode2_, toFlags2_,
                      toKeyCode3_, toFlags3_,
                      toKeyCode4_, toFlags4_,
                      toKeyCode5_, toFlags5_);
    }

    return true;
  }
}
