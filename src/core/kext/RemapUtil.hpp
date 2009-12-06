#ifndef REMAPUTIL_HPP
#define REMAPUTIL_HPP

#include <IOKit/hidsystem/IOHIKeyboard.h>
#include "bridge.hpp"
#include "remap.hpp"
#include "keycode.hpp"
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

    inline bool isKey(const RemapParams& remapParams, const KeyCode& key) {
      if (remapParams.isremapped) return false;
      return remapParams.params.key == key;
    }
    inline bool isKey(const RemapConsumerParams& remapParams, const ConsumerKeyCode& key) {
      return remapParams.params.key == key;
    }
    inline bool isEvent_Down(const RemapParams& remapParams) {
      return remapParams.params.eventType == EventType::DOWN;
    }
    inline bool isEvent_Up(const RemapParams& remapParams) {
      return remapParams.params.eventType == EventType::UP;
    }
    inline bool isEvent_Modify(const RemapParams& remapParams) {
      return remapParams.params.eventType == EventType::MODIFY;
    }
    inline bool isAlreadyRemapped(const RemapParams& remapParams) {
      return (remapParams.isremapped == true);
    }
    inline void drop(const RemapParams& remapParams) {
      remapParams.isremapped = true;
    }
    inline void drop(const RemapConsumerParams& remapParams) {
      remapParams.isremapped = true;
    }

    inline bool isKeyDown(const RemapParams& remapParams, const KeyCode& key) {
      if (isEvent_Down(remapParams)) {
        return isKey(remapParams, key);
      } else if (isEvent_Modify(remapParams)) {
        return remapParams.params.flags.isOn(key.getModifierFlag());
      } else {
        return false;
      }
    }

    class KeyToKey {
    public:
      bool remap(const RemapParams& remapParams,
                 const KeyCode& fromKeyCode, const Flags& fromFlags,
                 const KeyCode& toKeyCode,   const Flags& toFlags = ModifierFlag::NONE,
                 bool isSetKeyRepeat = true);
      // no-fromFlags version.
      bool remap(const RemapParams& remapParams,
                 const KeyCode& fromKeyCode,
                 const KeyCode& toKeyCode, const Flags& toFlags = ModifierFlag::NONE,
                 bool isSetKeyRepeat = true) {
        return remap(remapParams, fromKeyCode, 0, toKeyCode, toFlags, isSetKeyRepeat);
      }

      // --------------------------------------------------
      // Combination
      bool remap(const RemapParams& remapParams,
                 const KeyCode& fromKeyCode, const Flags& fromFlags,
                 const KeyCode& toKeyCode1,  const Flags& toFlags1,
                 const KeyCode& toKeyCode2,  const Flags& toFlags2 = ModifierFlag::NONE);

      // no-toFlag1 version.
      bool remap(const RemapParams& remapParams,
                 const KeyCode& fromKeyCode, const Flags& fromFlags,
                 const KeyCode& toKeyCode1,
                 const KeyCode& toKeyCode2,  const Flags& toFlags2 = ModifierFlag::NONE) {
        return remap(remapParams, fromKeyCode, fromFlags, toKeyCode1, ModifierFlag::NONE, toKeyCode2, toFlags2);
      }
      // no fromFlags version.
      bool remap(const RemapParams& remapParams,
                 const KeyCode& fromKeyCode,
                 const KeyCode& toKeyCode1,  const Flags& toFlags1,
                 const KeyCode& toKeyCode2,  const Flags& toFlags2 = ModifierFlag::NONE) {
        return remap(remapParams, fromKeyCode, 0, toKeyCode1, toFlags1, toKeyCode2, toFlags2);
      }
      // no fromFlags, toFlags1 version
      bool remap(const RemapParams& remapParams,
                 const KeyCode& fromKeyCode,
                 const KeyCode& toKeyCode1,
                 const KeyCode& toKeyCode2,  const Flags& toFlags2 = ModifierFlag::NONE) {
        return remap(remapParams, fromKeyCode, 0, toKeyCode1, ModifierFlag::NONE, toKeyCode2, toFlags2);
      }

    private:
      bool active_;
    };

    bool keyToPointingButton(const RemapParams& remapParams, const KeyCode& fromKeyCode, const PointingButton& toButton);

    bool keyToConsumer(const RemapParams& remapParams,
                       const KeyCode& fromKeyCode, const Flags& fromFlags,
                       const ConsumerKeyCode& toKeyCode, const Flags& toFlags = ModifierFlag::NONE);
    inline bool keyToConsumer(const RemapParams& remapParams,
                              const KeyCode& fromKeyCode,
                              const ConsumerKeyCode& toKeyCode, const Flags& toFlags = ModifierFlag::NONE) {
      return keyToConsumer(remapParams, fromKeyCode, 0, toKeyCode, toFlags);
    }

    bool consumerToKey(const RemapConsumerParams& remapParams,
                       const ConsumerKeyCode& fromKeyCode, const Flags& fromFlags,
                       const KeyCode& toKeyCode, const Flags& toFlags = ModifierFlag::NONE);
    inline bool consumerToKey(const RemapConsumerParams& remapParams,
                              const ConsumerKeyCode& fromKeyCode,
                              const KeyCode& toKeyCode, const Flags& toFlags = ModifierFlag::NONE) {
      return consumerToKey(remapParams, fromKeyCode, 0, toKeyCode, toFlags);
    }

    bool consumerToConsumer(const RemapConsumerParams& remapParams,
                            const ConsumerKeyCode& fromKeyCode, const Flags& fromFlags,
                            const ConsumerKeyCode& toKeyCode, const Flags& toFlags = ModifierFlag::NONE);
    inline bool consumerToConsumer(const RemapConsumerParams& remapParams,
                                   const ConsumerKeyCode& fromKeyCode,
                                   const ConsumerKeyCode& toKeyCode, const Flags& toFlags = ModifierFlag::NONE) {
      return consumerToConsumer(remapParams, fromKeyCode, 0, toKeyCode, toFlags);
    }

    bool pointingButtonToPointingButton(const RemapPointingParams_relative& remapParams,
                                        const PointingButton& fromButton, const Flags& fromFlags,
                                        const PointingButton& toButton);
    inline bool pointingButtonToPointingButton(const RemapPointingParams_relative& remapParams,
                                               const PointingButton& fromButton,
                                               const PointingButton& toButton) {
      return pointingButtonToPointingButton(remapParams, fromButton, 0, toButton);
    }

    // ----------------------------------------
    void fireModifiers(const Params_KeyboardEventCallBack& params);

    void fireKey(const Params_KeyboardEventCallBack& params, const KeyRemap4MacBook_bridge::GetWorkspaceData::Reply& workspacedata);

    void fireKey(const EventType& eventType, const Flags& flags, const KeyCode& key,
                 const KeyboardType& keyboardType, const AbsoluteTime& ts,
                 const KeyRemap4MacBook_bridge::GetWorkspaceData::Reply& workspacedata);

    inline void fireKey(const EventType& eventType, const Flags& flags, const KeyCode& key,
                        const Params_KeyboardEventCallBack& params, const KeyRemap4MacBook_bridge::GetWorkspaceData::Reply& workspacedata) {
      RemapUtil::fireKey(eventType,
                         flags, key,
                         params.keyboardType, params.ts,
                         workspacedata);

    }

    inline void fireKey(const Flags& flags, const KeyCode& key,
                        const Params_KeyboardEventCallBack& params, const KeyRemap4MacBook_bridge::GetWorkspaceData::Reply& workspacedata) {
      RemapUtil::fireKey(params.eventType, flags, key, params, workspacedata);
    }

    inline void fireKey_downup(const Flags& flags, const KeyCode& key, const KeyboardType& keyboardType, const AbsoluteTime& ts,
                               const KeyRemap4MacBook_bridge::GetWorkspaceData::Reply& workspacedata) {
      if (key.isModifier()) {
        RemapUtil::fireKey(EventType::MODIFY, flags, key, keyboardType, ts, workspacedata);
      } else {
        RemapUtil::fireKey(EventType::DOWN,   flags, key, keyboardType, ts, workspacedata);
        RemapUtil::fireKey(EventType::UP,     flags, key, keyboardType, ts, workspacedata);
      }
    }

    inline void fireKey_downup(const Flags& flags, const KeyCode& key,
                               const Params_KeyboardEventCallBack& params, const KeyRemap4MacBook_bridge::GetWorkspaceData::Reply& workspacedata) {
      RemapUtil::fireKey_downup(flags, key,
                                params.keyboardType, params.ts,
                                workspacedata);
    }

    void fireConsumer(const Params_KeyboardSpecialEventCallback& params);
    void fireRelativePointer(const Buttons& buttons);
    void fireScrollWheel(short int deltaAxis1, short int deltaAxis2, short int deltaAxis3,
                         IOFixed fixedDelta1, IOFixed fixedDelta2, IOFixed fixedDelta3,
                         SInt32 pointDelta1, SInt32 pointDelta2, SInt32 pointDelta3);

    // ----------------------------------------
    void pointingRelativeToScroll(const RemapPointingParams_relative& remapParams);
    const Buttons& getRemappedButtons(void);
  }

  // ----------------------------------------------------------------------
  // for SandS like behavior remappings (remap_space2shift, remap_enter2optionL_commandSpace, ...)
  class KeyOverlaidModifier {
  public:
    bool remap(const RemapParams& remapParams,
               const KeyCode& fromKeyCode, const Flags& fromFlags,
               const KeyCode& toKeyCode,   const Flags& toFlags,
               const KeyCode& fireKeyCode, const Flags& fireFlags = ModifierFlag::NONE,
               bool isFireRepeat = false);
    // no-fromFlags version.
    bool remap(const RemapParams& remapParams,
               const KeyCode& fromKeyCode,
               const KeyCode& toKeyCode,   const Flags& toFlags,
               const KeyCode& fireKeyCode, const Flags& fireFlags = ModifierFlag::NONE,
               bool isFireRepeat = false) {
      return remap(remapParams, fromKeyCode, 0,
                   toKeyCode, toFlags,
                   fireKeyCode, fireFlags, isFireRepeat);
    }
    // no-toFlags version.
    bool remap(const RemapParams& remapParams,
               const KeyCode& fromKeyCode, const Flags& fromFlags,
               const KeyCode& toKeyCode,
               const KeyCode& fireKeyCode, const Flags& fireFlags = ModifierFlag::NONE,
               bool isFireRepeat = false) {
      return remap(remapParams, fromKeyCode, fromFlags,
                   toKeyCode, ModifierFlag::NONE,
                   fireKeyCode, fireFlags, isFireRepeat);
    }
    // no-fromFlags, no-toFlags version.
    bool remap(const RemapParams& remapParams,
               const KeyCode& fromKeyCode,
               const KeyCode& toKeyCode,
               const KeyCode& fireKeyCode, const Flags& fireFlags = ModifierFlag::NONE,
               bool isFireRepeat = false) {
      return remap(remapParams, fromKeyCode, 0,
                   toKeyCode, ModifierFlag::NONE,
                   fireKeyCode, fireFlags, isFireRepeat);
    }

    // no-isFireRepeat version.
    bool remapWithRepeat(const RemapParams& remapParams,
                         const KeyCode& fromKeyCode, const Flags& fromFlags,
                         const KeyCode& toKeyCode,   const Flags& toFlags,
                         const KeyCode& fireKeyCode, const Flags& fireFlags = ModifierFlag::NONE) {
      return remap(remapParams, fromKeyCode, fromFlags,
                   toKeyCode, toFlags,
                   fireKeyCode, fireFlags, true);
    }
    // no-fromFlags, no-isFireRepeat version.
    bool remapWithRepeat(const RemapParams& remapParams,
                         const KeyCode& fromKeyCode,
                         const KeyCode& toKeyCode,   const Flags& toFlags,
                         const KeyCode& fireKeyCode, const Flags& fireFlags = ModifierFlag::NONE) {
      return remap(remapParams, fromKeyCode, 0,
                   toKeyCode, toFlags,
                   fireKeyCode, fireFlags, true);
    }
    // no-toFlag, no-isFireRepeat version.
    bool remapWithRepeat(const RemapParams& remapParams,
                         const KeyCode& fromKeyCode, const Flags& fromFlags,
                         const KeyCode& toKeyCode,
                         const KeyCode& fireKeyCode, const Flags& fireFlags = ModifierFlag::NONE) {
      return remap(remapParams, fromKeyCode, fromFlags,
                   toKeyCode, ModifierFlag::NONE,
                   fireKeyCode, fireFlags, true);
    }
    // no-fromFlags, no-toFlag, no-isFireRepeat version.
    bool remapWithRepeat(const RemapParams& remapParams,
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
    bool remap(const RemapParams& remapParams,
               const KeyCode& fromKeyCode, const ModifierFlag& toFlag,
               const KeyCode& fireKeyCode, const Flags& fireFlags = ModifierFlag::NONE);

  private:
    int pressCount;
    RemapUtil::KeyToKey keytokey_;
  };

  // ----------------------------------------
  // Modifier Holding + Key -> Key
  class ModifierHoldingKeyToKey {
  public:
    bool remap(const RemapParams& remapParams, const KeyCode& fromKeyCode, const Flags& fromFlags, const KeyCode& toKeyCode);

  private:
    bool isAnyEventHappen_;
    IntervalChecker ic_;
    RemapUtil::KeyToKey keytokey_;
  };

  // --------------------
  // ex. Ignore JIS_KANA x 2. (validate only the first once)
  class IgnoreMultipleSameKeyPress {
  public:
    bool remap(const RemapParams& remapParams, const KeyCode& fromKeyCode, const Flags& fromFlags = 0);

  private:
    unsigned int lastkeycode_; // save as 'unsigned int' to avoid ___cxa_guard_acquire.
  };

  // ----------------------------------------
  class PointingRelativeToScroll {
  public:
    bool remap(const RemapPointingParams_relative& remapParams, const Buttons& buttons = 0, const Flags& fromFlags = 0);

  private:
    bool isButtonHeldDown_;
  };
}

#endif
