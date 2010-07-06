#include "CallBackWrapper.hpp"
#include "CommonData.hpp"
#include "Config.hpp"
#include "FlagStatus.hpp"
#include "FromKeyChecker.hpp"
#include "KeyCode.hpp"
#include "ListHookedConsumer.hpp"
#include "ListHookedKeyboard.hpp"
#include "ListHookedPointing.hpp"
#include "RemapClass.hpp"

namespace org_pqrs_KeyRemap4MacBook {
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
    if (RemapClassManager::remap_dropkeyafterremap(*this)) return;

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
}
