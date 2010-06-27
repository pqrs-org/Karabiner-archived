#include "CallBackWrapper.hpp"
#include "EventOutput.hpp"
#include "RemapClass.hpp"
#include "PressDownKeys.hpp"
#include "VirtualKey.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace EventOutput {
    // ======================================================================
    Flags FireModifiers::lastFlags_(0);

    void
    FireModifiers::fire(Flags toFlags, KeyboardType keyboardType)
    {
      // At first I handle KeyUp and handle KeyDown next.
      // We need to end KeyDown at Command+Space to Option_L+Shift_L.
      //
      // When Option_L+Shift_L has a meaning (switch input language at Windows),
      // it does not works well when the last is KeyUp of Command.

      // ------------------------------------------------------------
      // Handle KEYPAD first

      // KeyUp
      if (lastFlags_.isOn(ModifierFlag::KEYPAD) && ! toFlags.isOn(ModifierFlag::KEYPAD)) {
        lastFlags_.remove(ModifierFlag::KEYPAD);

        Params_UpdateEventFlagsCallback::auto_ptr ptr(Params_UpdateEventFlagsCallback::alloc(lastFlags_));
        if (ptr) {
          EventOutputQueue::push(*ptr);
        }
      }
      if (! lastFlags_.isOn(ModifierFlag::KEYPAD) && toFlags.isOn(ModifierFlag::KEYPAD)) {
        lastFlags_.add(ModifierFlag::KEYPAD);

        Params_UpdateEventFlagsCallback::auto_ptr ptr(Params_UpdateEventFlagsCallback::alloc(lastFlags_));
        if (ptr) {
          EventOutputQueue::push(*ptr);
        }
      }

      // ------------------------------------------------------------
      // KeyUp
      for (int i = 0;; ++i) {
        ModifierFlag flag = FlagStatus::getFlag(i);
        if (flag == ModifierFlag::NONE) break;
        if (flag == ModifierFlag::EXTRA1) continue;
        if (flag == ModifierFlag::EXTRA2) continue;
        if (flag == ModifierFlag::EXTRA3) continue;
        if (flag == ModifierFlag::EXTRA4) continue;
        if (flag == ModifierFlag::EXTRA5) continue;

        if (! lastFlags_.isOn(flag)) continue;
        if (toFlags.isOn(flag)) continue;

        lastFlags_.remove(flag);

        Params_KeyboardEventCallBack::auto_ptr ptr(Params_KeyboardEventCallBack::alloc(EventType::MODIFY, lastFlags_, flag.getKeyCode(), keyboardType, false));
        if (! ptr) continue;
        EventOutputQueue::push(*ptr);
      }

      // KeyDown
      for (int i = 0;; ++i) {
        ModifierFlag flag = FlagStatus::getFlag(i);
        if (flag == ModifierFlag::NONE) break;
        if (flag == ModifierFlag::EXTRA1) continue;
        if (flag == ModifierFlag::EXTRA2) continue;
        if (flag == ModifierFlag::EXTRA3) continue;
        if (flag == ModifierFlag::EXTRA4) continue;
        if (flag == ModifierFlag::EXTRA5) continue;

        if (! toFlags.isOn(flag)) continue;
        if (lastFlags_.isOn(flag)) continue;

        lastFlags_.add(flag);

        Params_KeyboardEventCallBack::auto_ptr ptr(Params_KeyboardEventCallBack::alloc(EventType::MODIFY, lastFlags_, flag.getKeyCode(), keyboardType, false));
        if (! ptr) continue;
        EventOutputQueue::push(*ptr);
      }
    }

    // ======================================================================
    void
    FireKey::fire(const Params_KeyboardEventCallBack& params)
    {
      // ----------------------------------------
      // handle virtual keys
      Params_KeyboardEventCallBack::auto_ptr ptr(Params_KeyboardEventCallBack::alloc(params));
      if (! ptr) return;
      Params_KeyboardEventCallBack& p = *ptr;

      if (Handle_VK_LOCK::handle(p)) return;
      if (Handle_VK_STICKY::handle(p)) return;
      if (Handle_VK_LAZY::handle(p)) return;
      if (Handle_VK_CHANGE_INPUTMODE::handle(p)) return;
      if (Handle_VK_CONFIG::handle(p)) return;
      if (Handle_VK_JIS_TOGGLE_EISUU_KANA::handle(p)) return;
      if (handle_VK_JIS_EISUU_x2(p)) return;
      if (handle_VK_JIS_KANA_x2(p)) return;
      if (handle_VK_JIS_BACKSLASH(p)) return;
      if (handle_VK_JIS_YEN(p)) return;
      if (Handle_VK_JIS_TEMPORARY::handle(p)) return;
      if (RemapClassManager::handlevirtualkey(p)) return;
      if (p.key == KeyCode::VK_MODIFIER_EXTRA1 ||
          p.key == KeyCode::VK_MODIFIER_EXTRA2 ||
          p.key == KeyCode::VK_MODIFIER_EXTRA3 ||
          p.key == KeyCode::VK_MODIFIER_EXTRA4 ||
          p.key == KeyCode::VK_MODIFIER_EXTRA5) return;

      // ------------------------------------------------------------
      p.key.reverseNormalizeKey(p.flags, p.eventType, p.keyboardType);
      p.flags.stripEXTRA();

      // skip no-outputable keycodes.
      // Note: check before FireModifiers to avoid meaningless modifier event.
      if (p.key == KeyCode::VK_NONE ||
          p.key == KeyCode::VK_PSEUDO_KEY) {
        return;
      }

      FireModifiers::fire(p.flags, p.keyboardType);

      if (p.eventType == EventType::DOWN || p.eventType == EventType::UP) {
        EventOutputQueue::push(p);

        if (p.eventType == EventType::DOWN) {
          PressDownKeys::add(p.key, p.keyboardType);
        } else {
          PressDownKeys::remove(p.key, p.keyboardType);
        }
      }
    }

    void
    FireKey::fire_downup(Flags flags, KeyCode key, KeyboardType keyboardType)
    {
      Params_KeyboardEventCallBack::auto_ptr ptr(Params_KeyboardEventCallBack::alloc(EventType::MODIFY, flags, key, keyboardType, false));
      if (! ptr) return;
      Params_KeyboardEventCallBack& params = *ptr;

      Flags f = key.getModifierFlag();

      if (f != ModifierFlag::NONE) {
        // We operate FlagStatus for the case "key == KeyCode::CAPSLOCK".
        FlagStatus::increase(f);
        params.flags.add(f);
        FireKey::fire(params);

        FlagStatus::decrease(f);
        params.flags = flags;
        FireKey::fire(params);

      } else {
        params.eventType = EventType::DOWN;
        FireKey::fire(params);
        params.eventType = EventType::UP;
        FireKey::fire(params);
      }
    }

    // ======================================================================
    void
    FireConsumer::fire(const Params_KeyboardSpecialEventCallback& params)
    {
      Params_KeyboardSpecialEventCallback::auto_ptr ptr(Params_KeyboardSpecialEventCallback::alloc(params));
      if (! ptr) return;

      Params_KeyboardSpecialEventCallback& p = *ptr;

      p.flags.stripEXTRA();

      // skip no-outputable keycodes.
      // Note: check before FireModifiers to avoid meaningless modifier event.
      if (p.key == ConsumerKeyCode::VK_NONE ||
          p.key == ConsumerKeyCode::VK_PSEUDO_KEY) {
        return;
      }

      FireModifiers::fire();

      EventOutputQueue::push(p);
    }

    // ======================================================================
    Buttons FireRelativePointer::lastButtons_(0);

    void
    FireRelativePointer::fire(Buttons toButtons, int dx, int dy)
    {
      // We do not fire event if no need.
      if (dx == 0 && dy == 0 && toButtons == lastButtons_) return;

      Params_RelativePointerEventCallback::auto_ptr ptr(Params_RelativePointerEventCallback::alloc(toButtons, dx, dy));
      if (! ptr) return;
      Params_RelativePointerEventCallback& params = *ptr;

      lastButtons_ = toButtons;
      FireModifiers::fire();
      EventOutputQueue::push(params);
    }

    // ======================================================================
    void
    FireScrollWheel::fire(const Params_ScrollWheelEventCallback& params)
    {
      FireModifiers::fire();
      EventOutputQueue::push(params);
    }
  }
}
