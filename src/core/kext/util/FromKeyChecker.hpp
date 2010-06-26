#ifndef FROMKEYCHECKER_HPP
#define FROMKEYCHECKER_HPP

#include "remap.hpp"
#include "FlagStatus.hpp"
#include "KeyCode.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  class FromKeyChecker {
  public:
    FromKeyChecker(void) : active_(false) {}

    bool isFromKey(const RemapParams& remapParams, KeyCode fromKeyCode, Flags fromFlags) {
      if (remapParams.params.key != fromKeyCode) return false;
      bool isKeyDown = remapParams.isKeyDownOrModifierDown();
      return isFromKey(isKeyDown, fromFlags);
    }
    bool isFromKey(const RemapConsumerParams& remapParams, ConsumerKeyCode fromKeyCode, Flags fromFlags) {
      if (remapParams.params.key != fromKeyCode) return false;
      bool isKeyDown = (remapParams.params.eventType == EventType::DOWN);
      return isFromKey(isKeyDown, fromFlags);
    }

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
