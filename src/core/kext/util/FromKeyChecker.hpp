#ifndef FROMKEYCHECKER_HPP
#define FROMKEYCHECKER_HPP

#include "ButtonStatus.hpp"
#include "CallBackWrapper.hpp"
#include "FlagStatus.hpp"
#include "KeyCode.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  class FromKeyChecker {
  public:
    FromKeyChecker(void) : active_(false) {}

    bool isFromKey(EventType eventType, KeyCode key, Flags flags, KeyCode fromKeyCode, Flags fromFlags) {
      if (key != fromKeyCode) return false;
      bool isKeyDown = eventType.isKeyDownOrModifierDown(key, flags);
      return isFromKey(isKeyDown, flags, fromFlags);
    }
    bool isFromKey(EventType eventType, ConsumerKeyCode key, Flags flags, ConsumerKeyCode fromKeyCode, Flags fromFlags) {
      if (key != fromKeyCode) return false;
      bool isKeyDown = (eventType == EventType::DOWN);
      return isFromKey(isKeyDown, flags, fromFlags);
    }
    bool isFromPointingButton(const Params_RelativePointerEventCallback& params, PointingButton fromButton, Flags fromFlags) {
      if (params.ex_justPressed.isOn(fromButton)) {
        return isFromKey(true,  FlagStatus::makeFlags(), fromFlags);
      } else if (params.ex_justReleased.isOn(fromButton)) {
        return isFromKey(false, FlagStatus::makeFlags(), fromFlags);
      }
      return false;
    }
    bool isFromButtons(Buttons fromButtons, Flags fromFlags) {
      if (fromButtons != 0) {
        if (ButtonStatus::makeButtons().isOn(fromButtons)) {
          return isFromKey(true,  FlagStatus::makeFlags(), fromFlags);
        } else {
          return isFromKey(false, FlagStatus::makeFlags(), fromFlags);
        }
      } else {
        return FlagStatus::makeFlags().isOn(fromFlags);
      }
    }
    bool isactive(void) const { return active_; }

  private:
    bool isFromKey(bool isKeyDown, Flags flags, Flags fromFlags) {
      if (isKeyDown) {
        if (! flags.isOn(fromFlags)) return false;
        active_ = true;

      } else {
        // When active_ is true, we converted the key at KeyDown.
        // So we also convert the key at KeyUp.
        //
        // We don't check the flags in KeyUp.
        // When we decide by flags, a problem occurs in the following situation.
        //
        // ex. "Shift+Delete to Forward Delete"
        // (1) KeyDown "Delete" => "Delete"
        // (2) KeyDown "Shift"  => "Shift"
        // (3) KeyUp   "Delete" => "Forward Delete" *** Bad ***
        // (4) KeyUp   "Shift"  => "Shift"

        if (! active_) return false;
        active_ = false;
      }

      return true;
    }

    bool active_;
  };
}

#endif
