#include "KeyEventInputQueue.hpp"
#include "IOLockWrapper.hpp"
#include "ListHookedKeyboard.hpp"
#include "Config.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  KeyEventInputQueue::Item KeyEventInputQueue::item_[KeyEventInputQueue::MAXNUM];
  IntervalChecker KeyEventInputQueue::ic_;
  KeyEventInputQueue::Item* KeyEventInputQueue::current_ = KeyEventInputQueue::item_;
  KeyEventInputQueue::Item* KeyEventInputQueue::last_ = KeyEventInputQueue::item_;
  TimerWrapper KeyEventInputQueue::timer_;
  bool KeyEventInputQueue::isTimerActive_;

#include "../config/output/include.remapcode_simultaneouskeypresses_decl.cpp"

  void
  KeyEventInputQueue::initialize(IOWorkLoop& workloop)
  {
    for (int i = 0; i < MAXNUM; ++i) {
      item_[i].active = false;
    }
    ic_.begin();

    timer_.initialize(&workloop, NULL, KeyEventInputQueue::fire);
    isTimerActive_ = false;
  }

  void
  KeyEventInputQueue::terminate(void)
  {
    timer_.terminate();
  }

  void
  KeyEventInputQueue::add(OSObject* target,
                          EventType eventType,
                          Flags flags,
                          KeyCode key,
                          CharCode charCode,
                          CharSet charSet,
                          OrigCharCode origCharCode,
                          OrigCharSet origCharSet,
                          KeyboardType keyboardType,
                          bool repeat,
                          AbsoluteTime ts,
                          OSObject* sender,
                          void* refcon)
  {
    IOLockWrapper::ScopedLock lk(timer_.getlock());

    uint32_t ms = ic_.getmillisec();
    if (ms < 5) ms = 5;
    uint32_t delay = config.get_simultaneouskeypresses_delay();
    delay = (ms < delay) ? ms : delay;

    last_->target = target;
    last_->eventType = eventType;
    last_->flags = flags;
    last_->key = key;
    last_->charCode = charCode;
    last_->charSet = charSet;
    last_->origCharCode = origCharCode;
    last_->origCharSet = origCharSet;
    last_->keyboardType = keyboardType;
    last_->repeat = repeat;
    last_->ts = ts;
    last_->sender = sender;
    last_->refcon = refcon;

    last_->active = true;
    last_->dropped = false;
    last_->isremapped = false;
    last_->delayMS = delay;

    last_ = getnext(last_);

    if (last_ == current_) {
      IOLog("KeyRemap4MacBook --Warning-- KeyEventInputQueue::Item was filled up. Expand MAXNUM.\n");
    }

    // ------------------------------------------------------------
    // check queue
#include "../config/output/include.remapcode_simultaneouskeypresses.cpp"

    // ------------------------------------------------------------
    ic_.begin();

    if (delay == 0) {
      fire_nolock();
    } else {
      if (! isTimerActive_) {
        timer_.setTimeoutMS(current_->delayMS);
        isTimerActive_ = true;
      }
    }
  }

  void
  KeyEventInputQueue::fire(OSObject* notuse_owner, IOTimerEventSource* notuse_sender)
  {
    IOLockWrapper::ScopedLock lk(timer_.getlock());
    fire_nolock();
  }

  void
  KeyEventInputQueue::fire_nolock(void)
  {
    isTimerActive_ = false;

    if (! current_->active) return;

    if (! current_->dropped) {
      ListHookedKeyboard::hook_KeyboardEventCallback_queued(current_->target,
                                                            current_->eventType,
                                                            current_->flags,
                                                            current_->key,
                                                            current_->charCode,
                                                            current_->charSet,
                                                            current_->origCharCode,
                                                            current_->origCharSet,
                                                            current_->keyboardType,
                                                            current_->repeat,
                                                            current_->ts,
                                                            current_->sender,
                                                            current_->refcon,
                                                            current_->isremapped);
    }
    current_->active = false;

    current_ = getnext(current_);

    if (current_->active && current_->delayMS > 0) {
      timer_.setTimeoutMS(current_->delayMS);
      isTimerActive_ = true;
    }
  }

  void
  KeyEventInputQueue::Remap::remap(KeyCode fromKeyCode1, KeyCode fromKeyCode2, KeyCode toKeyCode)
  {
    // We ignore repeated key event to prevent the KeyUp lost.
    //
    // An example of KeyUp lost with invalid repeated key handling.
    // (1) KeyDown fromKeyCode2 without "Simultaneous Key Presses"
    // (2) Fire normal fromKeyCode2
    // (3) KeyDown fromKeyCode1 with repeated fromKeyCode2 (Simultaneous Key Presses)
    // (4) Fire toKeyCode
    // (5) KeyUp fromKeyCode2
    // (6) drop KeyUp fromKeyCode2 (***lost***)

    // ------------------------------------------------------------
    if (active1_ || active2_) {
      // restore KeyUp Event (fromKeyCode1).
      // drop KeyUp Event (fromKeyCode2).
      for (Item* p = current_; p != last_; p = getnext(p)) {
        if (! p->active) continue;
        if (p->repeat) continue;
        if (p->dropped) continue;
        if (p->isremapped) continue;
        if ((p->eventType).isKeyDownOrModifierDown(p->key, p->flags)) continue;

        if (active1_ && p->key == fromKeyCode1) {
          p->key = toKeyCode;
          p->isremapped = true;
          active1_ = false;
        }
        if (active2_ && p->key == fromKeyCode2) {
          p->dropped = true;
          active2_ = false;
        }
      }
    }

    // ------------------------------------------------------------
    // find fromKeyCode*
    Item* item1 = NULL;
    Item* item2 = NULL;

    for (Item* p = current_; p != last_; p = getnext(p)) {
      if (! p->active) continue;
      if (p->repeat) continue;
      if (p->dropped) continue;
      if (p->isremapped) continue;
      if (! (p->eventType).isKeyDownOrModifierDown(p->key, p->flags)) continue;

      if (p->key == fromKeyCode1) item1 = p;
      if (p->key == fromKeyCode2) item2 = p;
    }

    // replace first fromKeyCode1. and drop first fromKeyCode2
    if (item1 && item2) {
      item1->key = toKeyCode;
      item1->isremapped = true;
      active1_ = true;

      item2->dropped = true;
      active2_ = true;
    }
  }
}
