#include "CommonData.hpp"
#include "Config.hpp"
#include "Core.hpp"
#include "EventInputQueue.hpp"
#include "EventWatcher.hpp"
#include "FlagStatus.hpp"
#include "IOLockWrapper.hpp"
#include "ListHookedConsumer.hpp"
#include "ListHookedKeyboard.hpp"
#include "ListHookedPointing.hpp"
#include "NumHeldDownKeys.hpp"
#include "RemapClass.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  List* EventInputQueue::queue_ = NULL;
  IntervalChecker EventInputQueue::ic_;
  TimerWrapper EventInputQueue::timer_;

  void
  EventInputQueue::initialize(IOWorkLoop& workloop)
  {
    queue_ = new List();
    ic_.begin();
    timer_.initialize(&workloop, NULL, EventInputQueue::fire);
  }

  void
  EventInputQueue::terminate(void)
  {
    timer_.terminate();

    if (queue_) {
      delete queue_;
    }
  }

  uint32_t
  EventInputQueue::calcdelay(void)
  {
    // if no SimultaneousKeyPresses is enabled, fire immediately.
    if (! RemapClassManager::isEventInputQueueDelayEnabled()) {
      return IMMEDIATELY_DELAY;
    }

    uint32_t ms = ic_.getmillisec();
    uint32_t delay = config.get_simultaneouskeypresses_delay();
    if (delay > ms) delay = ms;  // min(ms, delay)
    if (delay < MIN_DELAY) delay = MIN_DELAY;  // max(MIN_DELAY, delay)
    ic_.begin();
    return delay;
  }

  void
  EventInputQueue::enqueue_(const Params_KeyboardEventCallBack& p)
  {
    if (! queue_) return;

    // Because we handle the key repeat ourself, drop the key repeat.
    if (p.repeat) return;

    // --------------------
    uint32_t delay = calcdelay();
    queue_->push_back(new Item(p, delay));
  }

  void
  EventInputQueue::enqueue_(const Params_KeyboardSpecialEventCallback& p)
  {
    if (! queue_) return;

    // Because we handle the key repeat ourself, drop the key repeat.
    if (p.repeat) return;

    // --------------------
    uint32_t delay = calcdelay();
    queue_->push_back(new Item(p, delay));
  }

  void
  EventInputQueue::enqueue_(const Params_RelativePointerEventCallback& p)
  {
    if (! queue_) return;

    // --------------------
    uint32_t delay = calcdelay();
    queue_->push_back(new Item(p, delay));
  }

  void
  EventInputQueue::enqueue_(const Params_ScrollWheelEventCallback& p)
  {
    if (! queue_) return;

    // --------------------
    uint32_t delay = calcdelay();
    queue_->push_back(new Item(p, delay));
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
    RemapClassManager::remap_simultaneouskeypresses();

    setTimer();
  }

  void
  EventInputQueue::push(const Params_KeyboardSpecialEventCallback& p)
  {
    IOLockWrapper::ScopedLock lk(timer_.getlock());

    enqueue_(p);

    // remap keys
    RemapClassManager::remap_simultaneouskeypresses();

    setTimer();
  }

  void
  EventInputQueue::push(const Params_RelativePointerEventCallback& p)
  {
    IOLockWrapper::ScopedLock lk(timer_.getlock());

    enqueue_(p);

    // remap keys
    RemapClassManager::remap_simultaneouskeypresses();

    setTimer();
  }

  void
  EventInputQueue::push(const Params_ScrollWheelEventCallback& p)
  {
    IOLockWrapper::ScopedLock lk(timer_.getlock());

    enqueue_(p);

    // remap keys
    RemapClassManager::remap_simultaneouskeypresses();

    setTimer();
  }

  void
  EventInputQueue::fire(OSObject* /*notuse_owner*/, IOTimerEventSource* /*notuse_sender*/)
  {
    IOLockWrapper::ScopedLock lk(timer_.getlock());

    if (! queue_) return;

    Item* p = static_cast<Item*>(queue_->front());
    if (! p) return;

    if (! p->dropped) {
      switch (p->params.type) {
        case ParamsUnion::KEYBOARD:
        {
          Params_KeyboardEventCallBack* params = (p->params).params.params_KeyboardEventCallBack;
          if (params) {
            IOLockWrapper::ScopedLock lk2(CommonData::eventLock);

            if ((params->eventType).isKeyDownOrModifierDown(params->key, params->flags)) {
              EventWatcher::on();
            }

            // ------------------------------------------------------------
            // We must call NumHeldDownKeys after inputqueue.
            // For example, when we type Command_L+S.
            //
            // (1) Command_L down (queued)
            // (2) KeyCode::S down (Command_L+S)
            // (1') dequeue Command_L down
            // (3) Command_L up
            // (4) KeyCode::S up
            // (2') dequeue KeyCode::S down
            //
            // if NumHeldDownKeys called when (4), Command_L state is reset.
            // Then (2') send KeyCode::S without Modifiers.
            NumHeldDownKeys::set(params->eventType, params->key, params->flags);

            Core::remap_KeyboardEventCallback(*params);
          }
          break;
        }
        case ParamsUnion::KEYBOARD_SPECIAL:
          if ((p->params).params.params_KeyboardSpecialEventCallback) {
            ListHookedConsumer::hook_KeyboardSpecialEventCallback_queued(*((p->params).params.params_KeyboardSpecialEventCallback));
          }
          break;

        case ParamsUnion::RELATIVE_POINTER:
          if ((p->params).params.params_RelativePointerEventCallback) {
            ListHookedPointing::hook_RelativePointerEventCallback_queued(*((p->params).params.params_RelativePointerEventCallback));
          }
          break;

        case ParamsUnion::SCROLL_POINTER:
          if ((p->params).params.params_ScrollWheelEventCallback) {
            ListHookedPointing::hook_ScrollWheelEventCallback_queued(*((p->params).params.params_ScrollWheelEventCallback));
          }
          break;

        default:
          IOLOG_ERROR("%s unkown type\n", __PRETTY_FUNCTION__);
          break;
      }
    }

    queue_->pop_front();

    setTimer();
  }
}
