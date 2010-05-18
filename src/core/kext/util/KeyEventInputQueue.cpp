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
  KeyEventInputQueue::Remap* KeyEventInputQueue::listRemap[MAXNUM_REMAPFUNC_SIMULTANEOUSKEYPRESSES + 1];
  int KeyEventInputQueue::listRemap_size = 0;

  namespace {
#include "../config/output/include.remapcode_simultaneouskeypresses_decl.cpp"
  }

  void
  KeyEventInputQueue::initialize(IOWorkLoop& workloop)
  {
    for (int i = 0; i < MAXNUM; ++i) {
      item_[i].active = false;
    }
    ic_.begin();

    timer_.initialize(&workloop, NULL, KeyEventInputQueue::fire);
    isTimerActive_ = false;

    for (int i = 0; i < MAXNUM_REMAPFUNC_SIMULTANEOUSKEYPRESSES; ++i) {
      listRemap[i] = NULL;
    }
    listRemap_size = 0;

#include "../config/output/include.remapcode_simultaneouskeypresses_initialize.cpp"
  }

  void
  KeyEventInputQueue::terminate(void)
  {
    timer_.terminate();
  }

  void
  KeyEventInputQueue::refresh_remapfunc(void)
  {
    IOLockWrapper::ScopedLock lk(timer_.getlock());

    listRemap_size = 0;
#include "../config/output/include.remapcode_refresh_remapfunc_simultaneouskeypresses.cpp"
  }

  bool
  KeyEventInputQueue::handleVirtualKey(const Params_KeyboardEventCallBack& params)
  {
    for (int i = 0; i < listRemap_size; ++i) {
      if (listRemap[i]) {
        if (listRemap[i]->handleVirtualKey(params)) return true;
      }
    }
    return false;
  }

  KeyEventInputQueue::Item*
  KeyEventInputQueue::enqueue_(OSObject* target,
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
    Item* p = last_;

    uint32_t ms = ic_.getmillisec();
    if (ms < 5) ms = 5;
    uint32_t delay = config.get_simultaneouskeypresses_delay();
    delay = (ms < delay) ? ms : delay; // min(ms, delay)

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
    last_->delayMS = delay;

    last_ = getnext(last_);

    if (last_ == current_) {
      IOLog("KeyRemap4MacBook --Warning-- KeyEventInputQueue::Item was filled up. Expand MAXNUM.\n");
    }

    ic_.begin();

    return p;
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

    Item* p = enqueue_(target, eventType, flags, key,
                       charCode, charSet, origCharCode, origCharSet,
                       keyboardType, repeat, ts, sender, refcon);

    // ------------------------------------------------------------
    // check queue
    for (int i = 0; i < listRemap_size; ++i) {
      if (listRemap[i]) {
        listRemap[i]->remap();
      }
    }

    // ------------------------------------------------------------
    if (p->delayMS == 0) {
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
                                                            current_->refcon);
    }
    current_->active = false;

    current_ = getnext(current_);

    if (current_->active && current_->delayMS > 0) {
      timer_.setTimeoutMS(current_->delayMS);
      isTimerActive_ = true;
    }
  }

  void
  KeyEventInputQueue::Remap::push(Item* base, KeyCode key, bool isKeyDown)
  {
    if (key == KeyCode::VK_NONE) return;
    if (! base) return;

    EventType eventType = isKeyDown ? EventType::DOWN : EventType::UP;

    KeyEventInputQueue::enqueue_(base->target,
                                 eventType,
                                 base->flags,
                                 key,
                                 base->charCode,
                                 base->charSet,
                                 base->origCharCode,
                                 base->origCharSet,
                                 base->keyboardType,
                                 base->repeat,
                                 base->ts,
                                 base->sender,
                                 base->refcon);
  }

  void
  KeyEventInputQueue::Remap::remap(void)
  {
    // We ignore repeated key event to prevent the KeyUp lost.
    //
    // An example of KeyUp lost with invalid repeated key handling.
    // (1) KeyDown fromKeyCode2_ without "Simultaneous Key Presses"
    // (2) Fire normal fromKeyCode2_
    // (3) KeyDown fromKeyCode1_ with repeated fromKeyCode2_ (Simultaneous Key Presses)
    // (4) Fire toKeyCode1
    // (5) KeyUp fromKeyCode2_
    // (6) drop KeyUp fromKeyCode2_ (***lost***)

    // ------------------------------------------------------------
    if (active1_ || active2_) {
      // restore KeyUp Event (fromKeyCode1_).
      // drop KeyUp Event (fromKeyCode2_).
      for (Item* p = current_; p != last_; p = getnext(p)) {
        if (! p->active) continue;
        if (p->repeat) continue;
        if (p->dropped) continue;
        if ((p->eventType).isKeyDownOrModifierDown(p->key, p->flags)) continue;

        if (active1_ && p->key == fromKeyCode1_) {
          push(p, virtualKeyCode_, false);

          p->dropped = true;
          active1_ = false;
        }
        if (active2_ && p->key == fromKeyCode2_) {
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

    for (Item* p = getprev(last_); p != getprev(current_); p = getprev(p)) {
      if (! p->active) continue;
      if (p->repeat) continue;
      if (p->dropped) continue;

      if (p->key == fromKeyCode1_ && ! item1) {
        item1 = p;
        if (! base) base = p;
      }
      if (p->key == fromKeyCode2_ && ! item2) {
        item2 = p;
        if (! base) base = p;
      }
    }

    // replace first fromKeyCode1. and drop first fromKeyCode2
    if (item1 && (item1->eventType).isKeyDownOrModifierDown(item1->key, item1->flags) &&
        item2 && (item2->eventType).isKeyDownOrModifierDown(item2->key, item2->flags)) {
      item1->dropped = true;
      active1_ = true;

      item2->dropped = true;
      active2_ = true;

      push(base, virtualKeyCode_, true);
    }
  }

  bool
  KeyEventInputQueue::Remap::handleVirtualKey(const Params_KeyboardEventCallBack& params)
  {
    if (virtualKeyCode_ != params.key) return false;

    RemapParams rp(params);
    if (toKeyCode2_ == KeyCode::VK_NONE) {
      keytokey_.remap(rp, virtualKeyCode_,
                      toKeyCode1_, toFlags1_);

    } else {
      keytokey_.remap(rp, virtualKeyCode_,
                      toKeyCode1_, toFlags1_,
                      toKeyCode2_, toFlags2_,
                      toKeyCode3_, toFlags3_,
                      toKeyCode4_, toFlags4_,
                      toKeyCode5_, toFlags5_);
    }

    return true;
  }
}
