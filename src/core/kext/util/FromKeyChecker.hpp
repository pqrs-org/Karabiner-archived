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

    bool isFromKey(const Params_KeyboardEventCallBack& params, KeyCode fromKeyCode, Flags fromFlags) {
      if (params.key != fromKeyCode) return false;
      bool isKeyDown = params.eventType.isKeyDownOrModifierDown(params.key, params.flags);
      return isFromKey(isKeyDown, fromFlags);
    }
    bool isFromKey(const Params_KeyboardSpecialEventCallback& params, ConsumerKeyCode fromKeyCode, Flags fromFlags) {
      if (params.key != fromKeyCode) return false;
      bool isKeyDown = (params.eventType == EventType::DOWN);
      return isFromKey(isKeyDown, fromFlags);
    }
    bool isFromPointingButton(const Params_RelativePointerEventCallback& params, PointingButton fromButton, Flags fromFlags) {
      if (params.ex_justPressed.isOn(fromButton)) {
        return isFromKey(true,  fromFlags);
      } else if (params.ex_justReleased.isOn(fromButton)) {
        return isFromKey(false, fromFlags);
      }
      return false;
    }
    bool isFromButtons(Buttons fromButtons, Flags fromFlags) {
      if (fromButtons != 0) {
        if (ButtonStatus::makeButtons().isOn(fromButtons)) {
          return isFromKey(true,  fromFlags);
        } else {
          return isFromKey(false, fromFlags);
        }
      } else {
        return FlagStatus::makeFlags().isOn(fromFlags);
      }
    }
    bool isactive(void) const { return active_; }

  private:
    bool isFromKey(bool isKeyDown, Flags fromFlags) {
      if (isKeyDown) {
        if (! FlagStatus::makeFlags().isOn(fromFlags)) return false;
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
