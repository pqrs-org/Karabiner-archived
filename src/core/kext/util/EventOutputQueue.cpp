#include "Config.hpp"
#include "EventOutputQueue.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  Queue* EventOutputQueue::queue_ = NULL;
  IntervalChecker EventOutputQueue::ic_;
  TimerWrapper EventOutputQueue::timer_;

  void
  EventOutputQueue::initialize(IOWorkLoop& workloop)
  {
    queue_ = new Queue();
    ic_.begin();
    timer_.initialize(&workloop, NULL, EventOutputQueue::fire);
  }

  void
  EventOutputQueue::terminate(void)
  {
    timer_.terminate();

    if (queue_) {
      for (;;) {
        EventOutputQueue::Item* p = static_cast<EventOutputQueue::Item*>(queue_->front());
        if (! p) break;

        queue_->pop();
        delete p;
      }
      delete queue_;
    }
  }

  // ----------------------------------------------------------------------
  void
  EventOutputQueue::push(Item& p)
  {
    IOLockWrapper::ScopedLock lk(timer_.getlock());

    if (! queue_) return;

    bool isempty = queue_->empty();
    queue_->push(&p);

    // ------------------------------------------------------------
    if (isempty && ic_.getmillisec() > DELAY) {
      fire_nolock();
    } else {
      IOLOG_DEVEL("Params_KeyboardEventCallBack::Queue enqueued ic_.getmillisec() = %d\n", ic_.getmillisec());
      timer_.setTimeoutMS(DELAY, false);
    }

    ic_.begin();
  }

  void
  EventOutputQueue::push(const Params_KeyboardEventCallBack& p)
  {
    Item* newp = new Item(p);
    if (! newp) return;
    push(*newp);
  }

  void
  EventOutputQueue::push(const Params_UpdateEventFlagsCallback& p)
  {
    Item* newp = new Item(p);
    if (! newp) return;
    push(*newp);
  }

  void
  EventOutputQueue::push(const Params_KeyboardSpecialEventCallback& p)
  {
    Item* newp = new Item(p);
    if (! newp) return;
    push(*newp);
  }

  void
  EventOutputQueue::push(const Params_RelativePointerEventCallback& p)
  {
    Item* newp = new Item(p);
    if (! newp) return;
    push(*newp);
  }

  void
  EventOutputQueue::push(const Params_ScrollWheelEventCallback& p)
  {
    Item* newp = new Item(p);
    if (! newp) return;
    push(*newp);
  }

  // ----------------------------------------------------------------------
  void
  EventOutputQueue::fire(OSObject* owner, IOTimerEventSource* sender)
  {
    IOLockWrapper::ScopedLock lk(timer_.getlock());
    fire_nolock();
  }

  void
  EventOutputQueue::fire_nolock(void)
  {
    if (! queue_) return;

    Item* p = static_cast<Item*>(queue_->front());
    if (! p) return;

    // ----------------------------------------
    queue_->pop();

    switch ((p->params).type) {
      case ParamsUnion::KEYBOARD:
        ((p->params).params.params_KeyboardEventCallBack)->apply();
        break;
      case ParamsUnion::UPDATE_FLAGS:
        ((p->params).params.params_UpdateEventFlagsCallback)->apply();
        break;
      case ParamsUnion::KEYBOARD_SPECIAL:
        ((p->params).params.params_KeyboardSpecialEventCallback)->apply();
        break;
      case ParamsUnion::RELATIVE_POINTER:
        ((p->params).params.params_RelativePointerEventCallback)->apply();
        break;
      case ParamsUnion::SCROLL_POINTER:
        ((p->params).params.params_ScrollWheelEventCallback)->apply();
        break;
    }

    delete p;

    // ----------------------------------------
    if (! queue_->empty()) {
      timer_.setTimeoutMS(DELAY);
    }
  }
}
