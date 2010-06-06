#include "CallBackWrapper.hpp"
#include "CommonData.hpp"
#include "Config.hpp"
#include "FlagStatus.hpp"
#include "ListHookedConsumer.hpp"
#include "ListHookedKeyboard.hpp"
#include "ListHookedPointing.hpp"
#include "KeyFloodChecker.hpp"
#include "KeyCode.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  Queue* CallBackWrapperQueue::queue_;
  IntervalChecker CallBackWrapperQueue::ic_;
  TimerWrapper CallBackWrapperQueue::timer_;
  bool CallBackWrapperQueue::isTimerActive_;

  void
  Params_KeyboardEventCallBack::log(const char* message) const
  {
    if (! config.debug) return;

    printf("KeyRemap4MacBook KeyboardEventCallback [%7s]: eventType %d, flags 0x%x, key %d, kbdType %d\n",
           message, eventType.get(), flags.get(), key.get(), keyboardType.get());
  }

  void
  Params_UpdateEventFlagsCallback::log(const char* message) const
  {
    if (! config.debug) return;

    printf("KeyRemap4MacBook UpdateEventFlagsCallback [%7s]: flags 0x%x\n",
           message, flags.get());
  }

  void
  Params_KeyboardSpecialEventCallback::log(const char* message) const
  {
    if (! config.debug) return;

    printf("KeyRemap4MacBook KeyboardSpecialEventCallBack [%7s]: eventType %d, flags 0x%x, key %d, flavor %d, guid %lld\n",
           message, eventType.get(), flags.get(), key.get(), flavor, guid);
  }

  void
  Params_RelativePointerEventCallback::log(const char* message) const
  {
    if (! config.debug_pointing) return;

    printf("KeyRemap4MacBook RelativePointerEventCallBack [%7s]: buttons: %d, dx: %d, dy: %d\n",
           message, buttons.get(), dx, dy);
  }

  void
  Params_ScrollWheelEventCallback::log(const char* message) const
  {
    if (! config.debug_pointing) return;

#if __x86_64__
    const char* format = "KeyRemap4MacBook ScrollWheelEventCallback [%s]: deltaAxis(%d, %d, %d), fixedDelta(%ld, %ld, %ld), pointDelta(%d,%d,%d), options: %d\n";
#else
    const char* format = "KeyRemap4MacBook ScrollWheelEventCallback [%s]: deltaAxis(%d, %d, %d), fixedDelta(%d, %d, %d), pointDelta(%d,%d,%d), options: %d\n";
#endif
    printf(format,
           message,
           deltaAxis1, deltaAxis2, deltaAxis3,
           fixedDelta1, fixedDelta2, fixedDelta3,
           pointDelta1, pointDelta2, pointDelta3,
           options);
  }

  namespace {
    bool
    checkFlags(Flags flags) {
      if (flags.isOn(ModifierFlag::NONE) ||
          flags.isOn(ModifierFlag::EXTRA1) ||
          flags.isOn(ModifierFlag::EXTRA2) ||
          flags.isOn(ModifierFlag::EXTRA3) ||
          flags.isOn(ModifierFlag::EXTRA4) ||
          flags.isOn(ModifierFlag::EXTRA5)) {
        IOLog("[KeyRemap4MacBook ERROR] CallBackWrapper::checkFlags invalid flags:0x%x\n", flags.get());
        return false;
      }
      return true;
    }
  }

  // ----------------------------------------------------------------------
  void
  Params_KeyboardEventCallBack::apply(void) const
  {
    CallBackWrapperQueue::push(*this);
  }

  void
  Params_KeyboardEventCallBack::do_apply(void) const
  {
    if (key >= KeyCode::VK__BEGIN__) {
      // Invalid keycode
      IOLog("[KeyRemap4MacBook ERROR] Params_KeyboardEventCallBack::apply invalid key:%d\n", key.get());
      return;
    }
    if (eventType == EventType::MODIFY && ! key.isModifier()) {
      // Invalid modifierkeycode
      IOLog("[KeyRemap4MacBook ERROR] Params_KeyboardEventCallBack::apply invalid modifierkey:%d\n", key.get());
      return;
    }
    if (! checkFlags(flags)) return;

    // ------------------------------------------------------------
    if (config.option_drop_slowexpose) {
      // Skip if Shift+F8,F9,F10,F11,F12,EXPOSE_ALL.
      if (key == KeyCode::F8 || key == KeyCode::F9 ||
          key == KeyCode::F10 || key == KeyCode::F11 ||
          key == KeyCode::F12 || key == KeyCode::EXPOSE_ALL) {
        if (flags.isOn(ModifierFlag::SHIFT_L) ||
            flags.isOn(ModifierFlag::SHIFT_R)) {
          return;
        }
      }
    }

    if (config.option_jis_drop_eisuukana_with_modifiers) {
      // Skip if EISUU,KANA with any modifiers.
      if (key == KeyCode::JIS_EISUU || key == KeyCode::JIS_KANA) {
        if (flags != 0) {
          return;
        }
      }
    }

    if (config.option_jis_drop_frequently_eisuukana) {
      if (key == KeyCode::JIS_EISUU || key == KeyCode::JIS_KANA) {
        if (eventType.isKeyDownOrModifierDown(key, flags)) {
          static KeyFloodChecker keyfloodchecker;
          enum {
            INTERVAL_MILLISEC = 600,
            ALLOWED_COUNT_IN_INTERVAL = 5,
          };
          if (! keyfloodchecker.check(INTERVAL_MILLISEC, ALLOWED_COUNT_IN_INTERVAL)) {
            return;
          }
          keyfloodchecker.add();
        }
      }
    }

    // ------------------------------------------------------------
    HookedKeyboard* hk = ListHookedKeyboard::instance().get();
    if (! hk) return;

    KeyboardEventCallback callback = hk->getOrig_keyboardEventAction();
    if (! callback) return;

    OSObject* target = hk->getOrig_keyboardEventTarget();
    if (! target) return;

    OSObject* sender = OSDynamicCast(OSObject, hk->get());
    if (! sender) return;

    const AbsoluteTime& ts = CommonData::getcurrent_ts();
    OSObject* refcon = NULL;

    log("sending");
    callback(target, eventType.get(), flags.get(), key.get(),
             charCode.get(), charSet.get(), origCharCode.get(), origCharSet.get(),
             keyboardType.get(), repeat, ts, sender, refcon);

    CommonData::setcurrent_keyboardType(keyboardType);

    // --------------------
    if (eventType == EventType::DOWN) {
      FlagStatus::sticky_clear();
    }

    // --------------------
    // handle CapsLock LED
    IOHIKeyboard* kbd = hk->get();
    if (kbd) {
      int led = kbd->getLEDStatus();
      if (config.general_capslock_led_hack) {
        if (led == 0) {
          kbd->setAlphaLockFeedback(true);
        }
      } else {
        if (flags.isOn(ModifierFlag::CAPSLOCK)) {
          if (led == 0) {
            kbd->setAlphaLockFeedback(true);
          }
        } else {
          if (led != 0) {
            kbd->setAlphaLockFeedback(false);
          }
        }
      }
    }
  }

  void
  Params_UpdateEventFlagsCallback::apply(void) const
  {
    if (! checkFlags(flags)) return;

    // ------------------------------------------------------------
    HookedKeyboard* hk = ListHookedKeyboard::instance().get();
    if (! hk) return;

    UpdateEventFlagsCallback callback = hk->getOrig_updateEventFlagsAction();
    if (! callback) return;

    OSObject* target = hk->getOrig_updateEventFlagsTarget();
    if (! target) return;

    OSObject* sender = OSDynamicCast(OSObject, hk->get());
    if (! sender) return;

    OSObject* refcon = NULL;

    log("sending");
    callback(target, flags.get(), sender, refcon);
  }

  void
  Params_KeyboardSpecialEventCallback::apply(void) const
  {
    if (key >= ConsumerKeyCode::VK__BEGIN__) {
      // Invalid keycode
      IOLog("[KeyRemap4MacBook ERROR] Params_KeyboardSpecialEventCallback::apply invalid key:%d\n", key.get());
      return;
    }
    if (! checkFlags(flags)) return;

    // ------------------------------------------------------------
    HookedConsumer* hc = ListHookedConsumer::instance().get();
    if (! hc) return;

    KeyboardSpecialEventCallback callback = hc->getOrig_keyboardSpecialEventAction();
    if (! callback) return;

    OSObject* target = hc->getOrig_keyboardSpecialEventTarget();
    if (! target) return;

    OSObject* sender = OSDynamicCast(OSObject, hc->get());
    if (! sender) return;

    const AbsoluteTime& ts = CommonData::getcurrent_ts();
    OSObject* refcon = NULL;

    log("sending");
    callback(target, eventType.get(), flags.get(), key.get(),
             flavor, guid, repeat, ts, sender, refcon);

    // --------------------
    if (eventType == EventType::DOWN) {
      FlagStatus::sticky_clear();
    }
  }

  void
  Params_RelativePointerEventCallback::apply(void) const
  {
    HookedPointing* hp = ListHookedPointing::instance().get();
    if (! hp) return;

    RelativePointerEventCallback callback = hp->getOrig_relativePointerEventAction();
    if (! callback) return;

    OSObject* target = hp->getOrig_relativePointerEventTarget();
    if (! target) return;

    OSObject* sender = OSDynamicCast(OSObject, hp->get());
    if (! sender) return;

    const AbsoluteTime& ts = CommonData::getcurrent_ts();
    OSObject* refcon = NULL;

    log("sending");
    callback(target, buttons.get(), dx, dy, ts, sender, refcon);

    // --------------------
    if (buttons != Buttons(0)) {
      FlagStatus::sticky_clear();
    }
  }

  void
  Params_ScrollWheelEventCallback::apply(void) const
  {
    HookedPointing* hp = ListHookedPointing::instance().get();
    if (! hp) return;

    ScrollWheelEventCallback callback = hp->getOrig_scrollWheelEventAction();
    if (! callback) return;

    OSObject* target = hp->getOrig_scrollWheelEventTarget();
    if (! target) return;

    OSObject* sender = OSDynamicCast(OSObject, hp->get());
    if (! sender) return;

    const AbsoluteTime& ts = CommonData::getcurrent_ts();
    OSObject* refcon = NULL;

    log("sending");
    callback(target,
             deltaAxis1, deltaAxis2, deltaAxis3,
             fixedDelta1, fixedDelta2, fixedDelta3,
             pointDelta1, pointDelta2, pointDelta3,
             options, ts, sender, refcon);

    // --------------------
    FlagStatus::sticky_clear();
  }


  // ======================================================================
  void
  CallBackWrapperQueue::initialize(IOWorkLoop& workloop)
  {
    queue_ = new Queue();
    ic_.begin();
    timer_.initialize(&workloop, NULL, CallBackWrapperQueue::fire);
    isTimerActive_ = false;
  }

  void
  CallBackWrapperQueue::terminate(void)
  {
    timer_.terminate();

    for (;;) {
      CallBackWrapperQueue::Item* p = static_cast<CallBackWrapperQueue::Item*>(queue_->front());
      if (! p) break;

      queue_->pop();
      delete p;
    }
  }

  void
  CallBackWrapperQueue::push(Item& p)
  {
    IOLockWrapper::ScopedLock lk(timer_.getlock());

    bool isempty = queue_->empty();
    queue_->push(&p);

    // ------------------------------------------------------------
    if (isempty && ic_.getmillisec() > DELAY) {
      fire_nolock();
    } else {
      if (! isTimerActive_) {
        if (config.debug_devel) {
          IOLog("KeyRemap4MacBook --Info-- Params_KeyboardEventCallBack::Queue enqueued ic_.getmillisec() = %d\n", ic_.getmillisec());
        }
        timer_.setTimeoutMS(DELAY);
        isTimerActive_ = true;
      }
    }

    ic_.begin();
  }

  void
  CallBackWrapperQueue::push(const Params_KeyboardEventCallBack& p)
  {
    Item* newp = new Item;
    if (! newp) return;

    newp->type = Item::KEYBOARD;
    newp->params.params_KeyboardEventCallBack = Params_KeyboardEventCallBack::alloc(p.eventType, p.flags, p.key,
                                                                                    p.charCode, p.charSet, p.origCharCode, p.origCharSet,
                                                                                    p.keyboardType, p.repeat);
    push(*newp);
  }

  void
  CallBackWrapperQueue::fire(OSObject* owner, IOTimerEventSource* sender)
  {
    IOLockWrapper::ScopedLock lk(timer_.getlock());
    fire_nolock();
  }

  void
  CallBackWrapperQueue::fire_nolock(void)
  {
    isTimerActive_ = false;

    Item* p = static_cast<Item*>(queue_->front());
    if (! p) return;

    queue_->pop();
    ((p->params).params_KeyboardEventCallBack)->do_apply();
    delete p;

    if (! queue_->empty()) {
      timer_.setTimeoutMS(DELAY);
      isTimerActive_ = true;
    }
  }

  CallBackWrapperQueue::Item::~Item(void)
  {
#define DELETE_PARAMS(PARAMS) { \
    if (PARAMS) {               \
      delete PARAMS;            \
    }                           \
}

    switch (type) {
      case KEYBOARD:
        DELETE_PARAMS(params.params_KeyboardEventCallBack);
        break;
      case UPDATE_FLAGS:
        DELETE_PARAMS(params.params_UpdateEventFlagsCallback);
        break;
      case KEYBOARD_SPECIAL:
        DELETE_PARAMS(params.params_KeyboardSpecialEventCallback);
        break;
      case RELATIVE_POINTER:
        DELETE_PARAMS(params.params_RelativePointerEventCallback);
        break;
      case SCROLL_POINTER:
        DELETE_PARAMS(params.params_ScrollWheelEventCallback);
        break;
    }

#undef DELETE_PARAMS
  }
}
