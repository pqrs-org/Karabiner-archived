#include "EventInputQueue.hpp"
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
    if (delay > ms) delay = ms;  // min(ms, delay)
    if (delay < MIN_DELAY) delay = MIN_DELAY;  // max(MIN_DELAY, delay)
    ic_.begin();

    // --------------------
    Item* newp = new Item(p, delay);
    if (! newp) return;

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
}
