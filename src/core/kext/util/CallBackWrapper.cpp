#include "CallBackWrapper.hpp"
#include "CommonData.hpp"
#include "Config.hpp"
#include "FlagStatus.hpp"
#include "FromKeyChecker.hpp"
#include "ListHookedConsumer.hpp"
#include "ListHookedKeyboard.hpp"
#include "ListHookedPointing.hpp"
#include "KeyCode.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  Queue* EventOutputQueue::queue_ = NULL;
  IntervalChecker EventOutputQueue::ic_;
  TimerWrapper EventOutputQueue::timer_;

  void
  Params_KeyboardEventCallBack::log(const char* message) const
  {
    IOLOG_DEBUG("KeyboardEventCallback [%7s]: eventType %2d, flags 0x%08x, key %4d, kbdType %3d, repeat = %d\n",
                message, eventType.get(), flags.get(), key.get(), keyboardType.get(), repeat);
  }

  void
  Params_UpdateEventFlagsCallback::log(const char* message) const
  {
    IOLOG_DEBUG("UpdateEventFlagsCallback [%7s]: flags 0x%08x\n",
                message, flags.get());
  }

  void
  Params_KeyboardSpecialEventCallback::log(const char* message) const
  {
    IOLOG_DEBUG("KeyboardSpecialEventCallBack [%7s]: eventType %2d, flags 0x%08x, key %4d, flavor %4d, guid %lld, repeat = %d\n",
                message, eventType.get(), flags.get(), key.get(), flavor, guid, repeat);
  }

  void
  Params_RelativePointerEventCallback::log(const char* message) const
  {
    IOLOG_DEBUG_POINTING("RelativePointerEventCallBack [%7s]: buttons: 0x%08x, dx: %3d, dy: %3d\n",
                         message, buttons.get(), dx, dy);
  }

  void
  Params_ScrollWheelEventCallback::log(const char* message) const
  {
#if __x86_64__
    IOLOG_DEBUG_POINTING("ScrollWheelEventCallback [%7s]: deltaAxis(%d,%d,%d), fixedDelta(%d,%d,%d), pointDelta(%d,%d,%d), options: %d\n",
                         message,
                         deltaAxis1, deltaAxis2, deltaAxis3,
                         fixedDelta1, fixedDelta2, fixedDelta3,
                         pointDelta1, pointDelta2, pointDelta3,
                         options);
#else
    IOLOG_DEBUG_POINTING("ScrollWheelEventCallback [%7s]: deltaAxis(%d,%d,%d), fixedDelta(%ld,%ld,%ld), pointDelta(%ld,%ld,%ld), options: %ld\n",
                         message,
                         deltaAxis1, deltaAxis2, deltaAxis3,
                         fixedDelta1, fixedDelta2, fixedDelta3,
                         pointDelta1, pointDelta2, pointDelta3,
                         options);
#endif
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
        IOLOG_ERROR("CallBackWrapper::checkFlags invalid flags:0x%x\n", flags.get());
        return false;
      }
      return true;
    }
  }

  // ----------------------------------------------------------------------
  namespace {
    // for config.option_drop_slowexpose
    FromKeyChecker fromkeychecker_shiftL_f8;
    FromKeyChecker fromkeychecker_shiftR_f8;
    FromKeyChecker fromkeychecker_shiftL_f9;
    FromKeyChecker fromkeychecker_shiftR_f9;
    FromKeyChecker fromkeychecker_shiftL_f10;
    FromKeyChecker fromkeychecker_shiftR_f10;
    FromKeyChecker fromkeychecker_shiftL_f11;
    FromKeyChecker fromkeychecker_shiftR_f11;
    FromKeyChecker fromkeychecker_shiftL_f12;
    FromKeyChecker fromkeychecker_shiftR_f12;
    FromKeyChecker fromkeychecker_shiftL_expose;
    FromKeyChecker fromkeychecker_shiftR_expose;
    FromKeyChecker fromkeychecker_shiftL_dashboard;
    FromKeyChecker fromkeychecker_shiftR_dashboard;

    // config.option_jis_drop_eisuukana_with_modifiers
    FromKeyChecker fromkeychecker_commandL_jis_eisuu;
    FromKeyChecker fromkeychecker_commandR_jis_eisuu;
    FromKeyChecker fromkeychecker_commandL_jis_kana;
    FromKeyChecker fromkeychecker_commandR_jis_kana;
    FromKeyChecker fromkeychecker_controlL_jis_eisuu;
    FromKeyChecker fromkeychecker_controlR_jis_eisuu;
    FromKeyChecker fromkeychecker_controlL_jis_kana;
    FromKeyChecker fromkeychecker_controlR_jis_kana;
    FromKeyChecker fromkeychecker_optionL_jis_eisuu;
    FromKeyChecker fromkeychecker_optionR_jis_eisuu;
    FromKeyChecker fromkeychecker_optionL_jis_kana;
    FromKeyChecker fromkeychecker_optionR_jis_kana;
    FromKeyChecker fromkeychecker_shiftL_jis_eisuu;
    FromKeyChecker fromkeychecker_shiftR_jis_eisuu;
    FromKeyChecker fromkeychecker_shiftL_jis_kana;
    FromKeyChecker fromkeychecker_shiftR_jis_kana;
  }

  void
  Params_KeyboardEventCallBack::apply(void) const
  {
    if (key >= KeyCode::VK__BEGIN__) {
      // Invalid keycode
      IOLOG_ERROR("Params_KeyboardEventCallBack::apply invalid key:%d\n", key.get());
      return;
    }
    if (eventType == EventType::MODIFY && ! key.isModifier()) {
      // Invalid modifierkeycode
      IOLOG_ERROR("Params_KeyboardEventCallBack::apply invalid modifierkey:%d\n", key.get());
      return;
    }
    if (! checkFlags(flags)) return;

    // ------------------------------------------------------------
    if (config.option_drop_slowexpose) {
      // Skip if Shift+F8,F9,F10,F11,F12,EXPOSE_ALL, DASHBOARD
      if (fromkeychecker_shiftL_f8.isFromKey(*this,        KeyCode::F8,         ModifierFlag::SHIFT_L) ||
          fromkeychecker_shiftR_f8.isFromKey(*this,        KeyCode::F8,         ModifierFlag::SHIFT_R) ||
          fromkeychecker_shiftL_f9.isFromKey(*this,        KeyCode::F9,         ModifierFlag::SHIFT_L) ||
          fromkeychecker_shiftR_f9.isFromKey(*this,        KeyCode::F9,         ModifierFlag::SHIFT_R) ||
          fromkeychecker_shiftL_f10.isFromKey(*this,       KeyCode::F10,        ModifierFlag::SHIFT_L) ||
          fromkeychecker_shiftR_f10.isFromKey(*this,       KeyCode::F10,        ModifierFlag::SHIFT_R) ||
          fromkeychecker_shiftL_f11.isFromKey(*this,       KeyCode::F11,        ModifierFlag::SHIFT_L) ||
          fromkeychecker_shiftR_f11.isFromKey(*this,       KeyCode::F11,        ModifierFlag::SHIFT_R) ||
          fromkeychecker_shiftL_f12.isFromKey(*this,       KeyCode::F12,        ModifierFlag::SHIFT_L) ||
          fromkeychecker_shiftR_f12.isFromKey(*this,       KeyCode::F12,        ModifierFlag::SHIFT_R) ||
          fromkeychecker_shiftL_expose.isFromKey(*this,    KeyCode::EXPOSE_ALL, ModifierFlag::SHIFT_L) ||
          fromkeychecker_shiftR_expose.isFromKey(*this,    KeyCode::EXPOSE_ALL, ModifierFlag::SHIFT_R) ||
          fromkeychecker_shiftL_dashboard.isFromKey(*this, KeyCode::DASHBOARD,  ModifierFlag::SHIFT_L) ||
          fromkeychecker_shiftR_dashboard.isFromKey(*this, KeyCode::DASHBOARD,  ModifierFlag::SHIFT_R)) {
        return;
      }
    }

    if (config.option_jis_drop_eisuukana_with_modifiers) {
      // Skip if EISUU,KANA with any modifiers.
      if (fromkeychecker_commandL_jis_eisuu.isFromKey(*this, KeyCode::JIS_EISUU, ModifierFlag::COMMAND_L) ||
          fromkeychecker_commandR_jis_eisuu.isFromKey(*this, KeyCode::JIS_EISUU, ModifierFlag::COMMAND_R) ||
          fromkeychecker_commandL_jis_kana.isFromKey(*this,  KeyCode::JIS_KANA,  ModifierFlag::COMMAND_L) ||
          fromkeychecker_commandR_jis_kana.isFromKey(*this,  KeyCode::JIS_KANA,  ModifierFlag::COMMAND_R) ||
          fromkeychecker_controlL_jis_eisuu.isFromKey(*this, KeyCode::JIS_EISUU, ModifierFlag::CONTROL_L) ||
          fromkeychecker_controlR_jis_eisuu.isFromKey(*this, KeyCode::JIS_EISUU, ModifierFlag::CONTROL_R) ||
          fromkeychecker_controlL_jis_kana.isFromKey(*this,  KeyCode::JIS_KANA,  ModifierFlag::CONTROL_L) ||
          fromkeychecker_controlR_jis_kana.isFromKey(*this,  KeyCode::JIS_KANA,  ModifierFlag::CONTROL_R) ||
          fromkeychecker_optionL_jis_eisuu.isFromKey(*this,  KeyCode::JIS_EISUU, ModifierFlag::OPTION_L) ||
          fromkeychecker_optionR_jis_eisuu.isFromKey(*this,  KeyCode::JIS_EISUU, ModifierFlag::OPTION_R) ||
          fromkeychecker_optionL_jis_kana.isFromKey(*this,   KeyCode::JIS_KANA,  ModifierFlag::OPTION_L) ||
          fromkeychecker_optionR_jis_kana.isFromKey(*this,   KeyCode::JIS_KANA,  ModifierFlag::OPTION_R) ||
          fromkeychecker_shiftL_jis_eisuu.isFromKey(*this,   KeyCode::JIS_EISUU, ModifierFlag::SHIFT_L) ||
          fromkeychecker_shiftR_jis_eisuu.isFromKey(*this,   KeyCode::JIS_EISUU, ModifierFlag::SHIFT_R) ||
          fromkeychecker_shiftL_jis_kana.isFromKey(*this,    KeyCode::JIS_KANA,  ModifierFlag::SHIFT_L) ||
          fromkeychecker_shiftR_jis_kana.isFromKey(*this,    KeyCode::JIS_KANA,  ModifierFlag::SHIFT_R)) {
        return;
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
      IOLOG_ERROR("Params_KeyboardSpecialEventCallback::apply invalid key:%d\n", key.get());
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
    Item* newp = new Item;
    if (! newp) return;

    newp->type = Item::KEYBOARD;
    // newp->params will be deleted at ~Item().
    newp->params.params_KeyboardEventCallBack = Params_KeyboardEventCallBack::alloc(p);

    push(*newp);
  }

  void
  EventOutputQueue::push(const Params_UpdateEventFlagsCallback& p)
  {
    Item* newp = new Item;
    if (! newp) return;

    newp->type = Item::UPDATE_FLAGS;
    // newp->params will be deleted at ~Item().
    newp->params.params_UpdateEventFlagsCallback = Params_UpdateEventFlagsCallback::alloc(p);

    push(*newp);
  }

  void
  EventOutputQueue::push(const Params_KeyboardSpecialEventCallback& p)
  {
    Item* newp = new Item;
    if (! newp) return;

    newp->type = Item::KEYBOARD_SPECIAL;
    // newp->params will be deleted at ~Item().
    newp->params.params_KeyboardSpecialEventCallback = Params_KeyboardSpecialEventCallback::alloc(p);

    push(*newp);
  }

  void
  EventOutputQueue::push(const Params_RelativePointerEventCallback& p)
  {
    Item* newp = new Item;
    if (! newp) return;

    newp->type = Item::RELATIVE_POINTER;
    // newp->params will be deleted at ~Item().
    newp->params.params_RelativePointerEventCallback = Params_RelativePointerEventCallback::alloc(p);

    push(*newp);
  }

  void
  EventOutputQueue::push(const Params_ScrollWheelEventCallback& p)
  {
    Item* newp = new Item;
    if (! newp) return;

    newp->type = Item::SCROLL_POINTER;
    // newp->params will be deleted at ~Item().
    newp->params.params_ScrollWheelEventCallback = Params_ScrollWheelEventCallback::alloc(p);

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

    switch (p->type) {
      case Item::KEYBOARD:
        ((p->params).params_KeyboardEventCallBack)->apply();
        break;
      case Item::UPDATE_FLAGS:
        ((p->params).params_UpdateEventFlagsCallback)->apply();
        break;
      case Item::KEYBOARD_SPECIAL:
        ((p->params).params_KeyboardSpecialEventCallback)->apply();
        break;
      case Item::RELATIVE_POINTER:
        ((p->params).params_RelativePointerEventCallback)->apply();
        break;
      case Item::SCROLL_POINTER:
        ((p->params).params_ScrollWheelEventCallback)->apply();
        break;
    }

    delete p;

    // ----------------------------------------
    if (! queue_->empty()) {
      timer_.setTimeoutMS(DELAY);
    }
  }

  EventOutputQueue::Item::~Item(void)
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
