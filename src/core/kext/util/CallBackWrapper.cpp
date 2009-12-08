#include "CallBackWrapper.hpp"
#include "CommonData.hpp"
#include "Config.hpp"
#include "EventWatcher.hpp"
#include "ListHookedConsumer.hpp"
#include "ListHookedKeyboard.hpp"
#include "ListHookedPointing.hpp"
#include "KeyCode.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  void
  Params_KeyboardEventCallBack::log(const char* message) const
  {
    if (! config.debug) return;

    printf("KeyRemap4MacBook KeyboardEventCallback [%s]: eventType %d, flags 0x%x, key %d, kbdType %d\n",
           message, eventType.get(), flags.get(), key.get(), keyboardType.get());
  }

  void
  Params_KeyboardSpecialEventCallback::log(const char* message) const
  {
    if (! config.debug) return;

    printf("KeyRemap4MacBook KeyboardSpecialEventCallBack [%s]: eventType %d, flags 0x%x, key %d, flavor %d, guid %lld\n",
           message, eventType.get(), flags.get(), key.get(), flavor, guid);
  }

  void
  Params_RelativePointerEventCallback::log(const char* message) const
  {
    if (! config.debug_pointing) return;

    printf("KeyRemap4MacBook RelativePointerEventCallBack [%s]: buttons: %d, dx: %d, dy: %d\n",
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

  // ----------------------------------------------------------------------
  void
  Params_KeyboardEventCallBack::apply(void) const
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

    // ------------------------------------------------------------
    HookedKeyboard* hk = ListHookedKeyboard::instance().get();
    if (! hk) return;

    KeyboardEventCallback callback = hk->getOrig_keyboardEventAction();
    if (! callback) return;

    OSObject* target = hk->getOrig_keyboardEventTarget();
    if (! target) return;

    OSObject* sender = hk->get();
    if (! sender) return;

    const AbsoluteTime& ts = CommonData::getcurrent_ts();
    OSObject* refcon = NULL;

    log("sending");
    callback(target, eventType.get(), flags.get(), key.get(),
             charCode.get(), charSet.get(), origCharCode.get(), origCharSet.get(),
             keyboardType.get(), repeat, ts, sender, refcon);

    CommonData::setcurrent_keyboardType(keyboardType);

    if (eventType == EventType::DOWN) {
      EventWatcher::on();
    } else if (eventType == EventType::MODIFY) {
      if (flags.isOn(key.getModifierFlag())) {
        EventWatcher::on();
      }
    }
  }

  void
  Params_KeyboardSpecialEventCallback::apply(void) const
  {
    if (key >= ConsumerKeyCode::VK__BEGIN__) {
      // Invalid keycode
      IOLog("[KeyRemap4MacBook ERROR] Params_KeyboardSpecialEventCallback::apply invalid key:%d\n", key.get());
      return;
    }

    // ------------------------------------------------------------
    HookedConsumer* hc = ListHookedConsumer::instance().get();
    if (! hc) return;

    KeyboardSpecialEventCallback callback = hc->getOrig_keyboardSpecialEventAction();
    if (! callback) return;

    OSObject* target = hc->getOrig_keyboardSpecialEventTarget();
    if (! target) return;

    OSObject* sender = hc->get();
    if (! sender) return;

    const AbsoluteTime& ts = CommonData::getcurrent_ts();
    OSObject* refcon = NULL;

    log("sending");
    callback(target, eventType.get(), flags.get(), key.get(),
             flavor, guid, repeat, ts, sender, refcon);

    if (eventType == EventType::DOWN) {
      EventWatcher::on();
    }
  }

  void
  Params_RelativePointerEventCallback::apply(void) const
  {
    HookedPointing* hp = ListHookedPointing::instance().get();
    if (! hp) return;

    RelativePointerEventCallback callback = hp->getOrig_relativePointerEventAction();
    if (! callback) return;

    log("sending");
    callback(target, buttons.get(), dx, dy, ts, sender, refcon);

    if (! buttons.isNONE()) {
      EventWatcher::on();
    }
  }

  void
  Params_ScrollWheelEventCallback::apply(void) const
  {
    HookedPointing* hp = ListHookedPointing::instance().get();
    if (! hp) return;

    ScrollWheelEventCallback callback = hp->getOrig_scrollWheelEventAction();
    if (! callback) return;

    log("sending");
    callback(target,
             deltaAxis1, deltaAxis2, deltaAxis3,
             fixedDelta1, fixedDelta2, fixedDelta3,
             pointDelta1, pointDelta2, pointDelta3,
             options, ts, sender, refcon);

    // EventWatcher::on is not necessary.
  }
}
