#ifndef REMAPUTIL_HPP
#define REMAPUTIL_HPP

#include <IOKit/hidsystem/IOHIKeyboard.h>
#include "bridge.hpp"
#include "remap.hpp"
#include "CommonData.hpp"
#include "KeyCode.hpp"
#include "util/FlagStatus.hpp"
#include "util/PressDownKeys.hpp"
#include "util/IntervalChecker.hpp"
#include "util/EventWatcher.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace RemapUtil {
    enum {
      // see IOHIPointing.cpp in darwin.
      POINTING_FIXED_SCALE = 65536, // (== << 16)
      POINTING_POINT_SCALE = 10, // (== SCROLL_WHEEL_TO_PIXEL_SCALE >> 16)
    };

    class KeyToKey {
    public:
      bool remap(RemapParams& remapParams,
                 const KeyCode& fromKeyCode, const Flags& fromFlags,
                 const KeyCode& toKeyCode,   const Flags& toFlags = ModifierFlag::NONE,
                 bool isSetKeyRepeat = true);
      // no-fromFlags version.
      bool remap(RemapParams& remapParams,
                 const KeyCode& fromKeyCode,
                 const KeyCode& toKeyCode, const Flags& toFlags = ModifierFlag::NONE,
                 bool isSetKeyRepeat = true) {
        return remap(remapParams, fromKeyCode, 0, toKeyCode, toFlags, isSetKeyRepeat);
      }

      // --------------------------------------------------
      // Combination
      bool remap(RemapParams& remapParams,
                 const KeyCode& fromKeyCode, const Flags& fromFlags,
                 const KeyCode& toKeyCode1,  const Flags& toFlags1,
                 const KeyCode& toKeyCode2,  const Flags& toFlags2 = ModifierFlag::NONE);

      // no-toFlag1 version.
      bool remap(RemapParams& remapParams,
                 const KeyCode& fromKeyCode, const Flags& fromFlags,
                 const KeyCode& toKeyCode1,
                 const KeyCode& toKeyCode2,  const Flags& toFlags2 = ModifierFlag::NONE) {
        return remap(remapParams, fromKeyCode, fromFlags, toKeyCode1, ModifierFlag::NONE, toKeyCode2, toFlags2);
      }
      // no fromFlags version.
      bool remap(RemapParams& remapParams,
                 const KeyCode& fromKeyCode,
                 const KeyCode& toKeyCode1,  const Flags& toFlags1,
                 const KeyCode& toKeyCode2,  const Flags& toFlags2 = ModifierFlag::NONE) {
        return remap(remapParams, fromKeyCode, 0, toKeyCode1, toFlags1, toKeyCode2, toFlags2);
      }
      // no fromFlags, toFlags1 version
      bool remap(RemapParams& remapParams,
                 const KeyCode& fromKeyCode,
                 const KeyCode& toKeyCode1,
                 const KeyCode& toKeyCode2,  const Flags& toFlags2 = ModifierFlag::NONE) {
        return remap(remapParams, fromKeyCode, 0, toKeyCode1, ModifierFlag::NONE, toKeyCode2, toFlags2);
      }

    private:
      bool active_;
    };

    class ConsumerToKey {
    public:
      bool remap(RemapConsumerParams& remapParams,
                 const ConsumerKeyCode& fromKeyCode, const Flags& fromFlags,
                 const KeyCode& toKeyCode, const Flags& toFlags = ModifierFlag::NONE);

      // no fromFlags version
      bool remap(RemapConsumerParams& remapParams,
                 const ConsumerKeyCode& fromKeyCode,
                 const KeyCode& toKeyCode, const Flags& toFlags = ModifierFlag::NONE) {
        return remap(remapParams, fromKeyCode, 0, toKeyCode, toFlags);
      }

    private:
      RemapUtil::KeyToKey keytokey_;
    };

    class ConsumerToConsumer {
    public:
      bool remap(RemapConsumerParams& remapParams,
                 const ConsumerKeyCode& fromKeyCode, const Flags& fromFlags,
                 const ConsumerKeyCode& toKeyCode, const Flags& toFlags = ModifierFlag::NONE);

      // no fromFlags version
      bool remap(RemapConsumerParams& remapParams,
                 const ConsumerKeyCode& fromKeyCode,
                 const ConsumerKeyCode& toKeyCode, const Flags& toFlags = ModifierFlag::NONE) {
        return remap(remapParams, fromKeyCode, 0, toKeyCode, toFlags);
      }

    private:
      bool active_;
    };

    class KeyToConsumer {
    public:
      bool remap(RemapParams& remapParams,
                 const KeyCode& fromKeyCode, const Flags& fromFlags,
                 const ConsumerKeyCode& toKeyCode, const Flags& toFlags = ModifierFlag::NONE);

      // no fromFlags version
      bool remap(RemapParams& remapParams,
                 const KeyCode& fromKeyCode,
                 const ConsumerKeyCode& toKeyCode, const Flags& toFlags = ModifierFlag::NONE) {
        return remap(remapParams, fromKeyCode, 0, toKeyCode, toFlags);
      }

    private:
      RemapUtil::ConsumerToConsumer consumertoconsumer_;
    };

    class PointingButtonToPointingButton {
    public:
      bool remap(RemapPointingParams_relative& remapParams,
                 const PointingButton& fromButton, const Flags& fromFlags,
                 const PointingButton& toButton);

      // no fromFlags version
      bool remap(RemapPointingParams_relative& remapParams,
                 const PointingButton& fromButton,
                 const PointingButton& toButton) {
        return remap(remapParams, fromButton, 0, toButton);
      }

    private:
      bool active_;
    };

    class KeyToPointingButton {
    public:
      bool remap(RemapParams& remapParams,
                 const KeyCode& fromKeyCode, const Flags& fromFlags,
                 const PointingButton& toButton);

      // no fromFlags version
      bool remap(RemapParams& remapParams,
                 const KeyCode& fromKeyCode,
                 const PointingButton& toButton) {
        return remap(remapParams, fromKeyCode, 0, toButton);
      }

    private:
      PointingButtonToPointingButton buttontobutton_;
    };

    // ----------------------------------------
    void fireKey(const Params_KeyboardEventCallBack& params, const KeyRemap4MacBook_bridge::GetWorkspaceData::Reply& workspacedata);
    void fireKey_downup(const Flags& flags, const KeyCode& key, const KeyboardType& keyboardType,
                        const KeyRemap4MacBook_bridge::GetWorkspaceData::Reply& workspacedata);

    void fireConsumer(const Params_KeyboardSpecialEventCallback& params);
    void fireRelativePointer(const Params_RelativePointerEventCallback& params);
    void fireScrollWheel(const Params_ScrollWheelEventCallback& params);

    // ----------------------------------------
    void pointingRelativeToScroll(RemapPointingParams_relative& remapParams);
    const Buttons& getRemappedButtons(void);
  }

  class FireModifiers {
  public:
    static void fire(const Flags& toFlags = FlagStatus::makeFlags(), const KeyboardType& keyboardType = CommonData::getcurrent_keyboardType());

  private:
    static Flags lastFlags_;
  };

  // ----------------------------------------------------------------------
  // for SandS like behavior remappings (remap_space2shift, remap_enter2optionL_commandSpace, ...)
  class KeyOverlaidModifier {
  public:
    bool remap(RemapParams& remapParams,
               const KeyCode& fromKeyCode, const Flags& fromFlags,
               const KeyCode& toKeyCode,   const Flags& toFlags,
               const KeyCode& fireKeyCode, const Flags& fireFlags = ModifierFlag::NONE,
               bool isFireRepeat = false);
    // no-fromFlags version.
    bool remap(RemapParams& remapParams,
               const KeyCode& fromKeyCode,
               const KeyCode& toKeyCode,   const Flags& toFlags,
               const KeyCode& fireKeyCode, const Flags& fireFlags = ModifierFlag::NONE,
               bool isFireRepeat = false) {
      return remap(remapParams, fromKeyCode, 0,
                   toKeyCode, toFlags,
                   fireKeyCode, fireFlags, isFireRepeat);
    }
    // no-toFlags version.
    bool remap(RemapParams& remapParams,
               const KeyCode& fromKeyCode, const Flags& fromFlags,
               const KeyCode& toKeyCode,
               const KeyCode& fireKeyCode, const Flags& fireFlags = ModifierFlag::NONE,
               bool isFireRepeat = false) {
      return remap(remapParams, fromKeyCode, fromFlags,
                   toKeyCode, ModifierFlag::NONE,
                   fireKeyCode, fireFlags, isFireRepeat);
    }
    // no-fromFlags, no-toFlags version.
    bool remap(RemapParams& remapParams,
               const KeyCode& fromKeyCode,
               const KeyCode& toKeyCode,
               const KeyCode& fireKeyCode, const Flags& fireFlags = ModifierFlag::NONE,
               bool isFireRepeat = false) {
      return remap(remapParams, fromKeyCode, 0,
                   toKeyCode, ModifierFlag::NONE,
                   fireKeyCode, fireFlags, isFireRepeat);
    }

    // no-isFireRepeat version.
    bool remapWithRepeat(RemapParams& remapParams,
                         const KeyCode& fromKeyCode, const Flags& fromFlags,
                         const KeyCode& toKeyCode,   const Flags& toFlags,
                         const KeyCode& fireKeyCode, const Flags& fireFlags = ModifierFlag::NONE) {
      return remap(remapParams, fromKeyCode, fromFlags,
                   toKeyCode, toFlags,
                   fireKeyCode, fireFlags, true);
    }
    // no-fromFlags, no-isFireRepeat version.
    bool remapWithRepeat(RemapParams& remapParams,
                         const KeyCode& fromKeyCode,
                         const KeyCode& toKeyCode,   const Flags& toFlags,
                         const KeyCode& fireKeyCode, const Flags& fireFlags = ModifierFlag::NONE) {
      return remap(remapParams, fromKeyCode, 0,
                   toKeyCode, toFlags,
                   fireKeyCode, fireFlags, true);
    }
    // no-toFlag, no-isFireRepeat version.
    bool remapWithRepeat(RemapParams& remapParams,
                         const KeyCode& fromKeyCode, const Flags& fromFlags,
                         const KeyCode& toKeyCode,
                         const KeyCode& fireKeyCode, const Flags& fireFlags = ModifierFlag::NONE) {
      return remap(remapParams, fromKeyCode, fromFlags,
                   toKeyCode, ModifierFlag::NONE,
                   fireKeyCode, fireFlags, true);
    }
    // no-fromFlags, no-toFlag, no-isFireRepeat version.
    bool remapWithRepeat(RemapParams& remapParams,
                         const KeyCode& fromKeyCode,
                         const KeyCode& toKeyCode,
                         const KeyCode& fireKeyCode, const Flags& fireFlags = ModifierFlag::NONE) {
      return remap(remapParams, fromKeyCode, 0,
                   toKeyCode, ModifierFlag::NONE,
                   fireKeyCode, fireFlags, true);
    }

  private:
    bool isAnyEventHappen_;
    IntervalChecker ic_;
    RemapUtil::KeyToKey keytokey_;
    unsigned int savedflags_; // save as 'unsigned int' to avoid ___cxa_guard_acquire.
  };

  // ----------------------------------------
  // A modifier has DoublePressed key action.
  class DoublePressModifier {
  public:
    bool remap(RemapParams& remapParams,
               const KeyCode& fromKeyCode, const ModifierFlag& toFlag,
               const KeyCode& fireKeyCode, const Flags& fireFlags = ModifierFlag::NONE);

  private:
    int pressCount_;
    RemapUtil::KeyToKey keytokey_;
  };

  // ----------------------------------------
  // Modifier Holding + Key -> Key
  class ModifierHoldingKeyToKey {
  public:
    bool remap(RemapParams& remapParams, const KeyCode& fromKeyCode, const Flags& fromFlags, const KeyCode& toKeyCode);

  private:
    IntervalChecker ic_;
    RemapUtil::KeyToKey keytokey_;
  };

  // --------------------
  // ex. Ignore JIS_KANA x 2. (validate only the first once)
  class IgnoreMultipleSameKeyPress {
  public:
    bool remap(RemapParams& remapParams, const KeyCode& fromKeyCode, const Flags& fromFlags = 0);

  private:
    unsigned int lastkeycode_; // save as 'unsigned int' to avoid ___cxa_guard_acquire.
  };

  // ----------------------------------------
  class PointingRelativeToScroll {
  public:
    bool remap(RemapPointingParams_relative& remapParams, const Buttons& buttons = 0, const Flags& fromFlags = 0);

  private:
    bool isButtonHeldDown_;
  };
}

#endif
