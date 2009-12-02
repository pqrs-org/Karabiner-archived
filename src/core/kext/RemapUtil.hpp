#ifndef REMAPUTIL_HPP
#define REMAPUTIL_HPP

#include <IOKit/hidsystem/IOHIKeyboard.h>
#include "bridge.hpp"
#include "remap.hpp"
#include "keycode.hpp"
#include "util/FlagStatus.hpp"
#include "util/PressDownKeys.hpp"
#include "util/ListFireConsumerKey.hpp"
#include "util/ListFireRelativePointer.hpp"
#include "util/IntervalChecker.hpp"
#include "util/EventWatcher.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace RemapUtil {
    enum {
      // see IOHIPointing.cpp in darwin.
      POINTING_FIXED_SCALE = 65536, // (== << 16)
      POINTING_POINT_SCALE = 10, // (== SCROLL_WHEEL_TO_PIXEL_SCALE >> 16)
    };

    inline bool isKey(const RemapParams& remapParams, unsigned int keyCode) {
      if (remapParams.isremapped) return false;
      return remapParams.params.key == keyCode;
    }
    inline bool isKey(const RemapConsumerParams& remapParams, ConsumerKeyCode::ConsumerKeyCode keyCode) {
      return remapParams.params.key == static_cast<unsigned int>(keyCode);
    }
    inline bool isEvent_Down(const RemapParams& remapParams) {
      return remapParams.params.eventType == KeyEvent::DOWN;
    }
    inline bool isEvent_Up(const RemapParams& remapParams) {
      return remapParams.params.eventType == KeyEvent::UP;
    }
    inline bool isEvent_Modify(const RemapParams& remapParams) {
      return remapParams.params.eventType == KeyEvent::MODIFY;
    }
    inline bool isAlreadyRemapped(const RemapParams& remapParams) {
      return (remapParams.isremapped == true);
    }
    inline void drop(const RemapParams& remapParams) {
      remapParams.params.key = KeyCode::VK_NONE;
      remapParams.isremapped = true;
    }

    inline bool isModifierOn(unsigned int flags, ModifierFlag::ModifierFlag f) {
      return ((flags & f) == static_cast<unsigned int>(f));
    }
    inline bool isModifierOn(const RemapParams& remapParams, ModifierFlag::ModifierFlag flag) {
      return isModifierOn(remapParams.params.flags, flag);
    }

    KeyCode::KeyCode getModifierKeyCode(ModifierFlag::ModifierFlag flag);
    ModifierFlag::ModifierFlag getKeyCodeModifier(unsigned int keycode);

    inline bool isKeyDown(const RemapParams& remapParams, unsigned int keyCode) {
      if (isEvent_Down(remapParams)) {
        return isKey(remapParams, keyCode);
      } else if (isEvent_Modify(remapParams)) {
        return isModifierOn(remapParams, getKeyCodeModifier(keyCode));
      } else {
        return false;
      }
    }

    bool isInternalKeyboard(unsigned int keyboardType);

    class KeyToKey {
    public:
      // KeyToKey(void) : active_(false) {}

      bool remap(const RemapParams& remapParams, KeyCode::KeyCode fromKeyCode, unsigned int fromFlags, KeyCode::KeyCode toKeyCode, unsigned int toFlags = ModifierFlag::NONE, bool isSetKeyRepeat = true);
      // no-fromFlags version.
      bool remap(const RemapParams& remapParams, KeyCode::KeyCode fromKeyCode, KeyCode::KeyCode toKeyCode, unsigned int toFlags = ModifierFlag::NONE, bool isSetKeyRepeat = true) {
        return remap(remapParams, fromKeyCode, 0, toKeyCode, toFlags, isSetKeyRepeat);
      }

      bool remap(const RemapParams& remapParams, KeyCode::KeyCode fromKeyCode, unsigned int fromFlags,
                 KeyCode::KeyCode toKeyCode1, unsigned int toFlags1,
                 KeyCode::KeyCode toKeyCode2, unsigned int toFlags2 = ModifierFlag::NONE);
      // no-toFlag1 version.
      bool remap(const RemapParams& remapParams, KeyCode::KeyCode fromKeyCode, unsigned int fromFlags,
                 KeyCode::KeyCode toKeyCode1,
                 KeyCode::KeyCode toKeyCode2, unsigned int toFlags2 = ModifierFlag::NONE) {
        return remap(remapParams, fromKeyCode, fromFlags, toKeyCode1, ModifierFlag::NONE, toKeyCode2, toFlags2);
      }
      // no fromFlags version.
      bool remap(const RemapParams& remapParams, KeyCode::KeyCode fromKeyCode,
                 KeyCode::KeyCode toKeyCode1, unsigned int toFlags1,
                 KeyCode::KeyCode toKeyCode2, unsigned int toFlags2 = ModifierFlag::NONE) {
        return remap(remapParams, fromKeyCode, 0, toKeyCode1, toFlags1, toKeyCode2, toFlags2);
      }
      // no fromFlags, toFlags1 version
      bool remap(const RemapParams& remapParams, KeyCode::KeyCode fromKeyCode,
                 KeyCode::KeyCode toKeyCode1,
                 KeyCode::KeyCode toKeyCode2, unsigned int toFlags2 = ModifierFlag::NONE) {
        return remap(remapParams, fromKeyCode, 0, toKeyCode1, ModifierFlag::NONE, toKeyCode2, toFlags2);
      }

    private:
      bool active_;
    };

    void keyToPointingButton(const RemapParams& remapParams, KeyCode::KeyCode fromKeyCode, PointingButton::PointingButton toButton);

    bool keyToConsumer(const RemapParams& remapParams,
                       KeyCode::KeyCode fromKeyCode, unsigned int fromFlags,
                       ConsumerKeyCode::ConsumerKeyCode toKeyCode, unsigned int toFlags = ModifierFlag::NONE);
    inline bool keyToConsumer(const RemapParams& remapParams,
                              KeyCode::KeyCode fromKeyCode,
                              ConsumerKeyCode::ConsumerKeyCode toKeyCode, unsigned int toFlags = ModifierFlag::NONE) {
      return keyToConsumer(remapParams, fromKeyCode, 0, toKeyCode, toFlags);
    }

    bool consumerToKey(const RemapConsumerParams& remapParams,
                       ConsumerKeyCode::ConsumerKeyCode fromKeyCode, unsigned int fromFlags,
                       KeyCode::KeyCode toKeyCode, unsigned int toFlags = ModifierFlag::NONE);
    inline bool consumerToKey(const RemapConsumerParams& remapParams,
                              ConsumerKeyCode::ConsumerKeyCode fromKeyCode,
                              KeyCode::KeyCode toKeyCode, unsigned int toFlags = ModifierFlag::NONE) {
      return consumerToKey(remapParams, fromKeyCode, 0, toKeyCode, toFlags);
    }

    bool consumerToConsumer(const RemapConsumerParams& remapParams,
                            ConsumerKeyCode::ConsumerKeyCode fromKeyCode, unsigned int fromFlags,
                            ConsumerKeyCode::ConsumerKeyCode toKeyCode, unsigned int toFlags = ModifierFlag::NONE);
    inline bool consumerToConsumer(const RemapConsumerParams& remapParams,
                                   ConsumerKeyCode::ConsumerKeyCode fromKeyCode,
                                   ConsumerKeyCode::ConsumerKeyCode toKeyCode, unsigned int toFlags = ModifierFlag::NONE) {
      return consumerToConsumer(remapParams, fromKeyCode, 0, toKeyCode, toFlags);
    }

    bool pointingButtonToPointingButton(const RemapPointingParams_relative& remapParams,
                                        PointingButton::PointingButton fromButton, unsigned int fromFlags,
                                        PointingButton::PointingButton toButton);
    inline bool pointingButtonToPointingButton(const RemapPointingParams_relative& remapParams,
                                               PointingButton::PointingButton fromButton,
                                               PointingButton::PointingButton toButton) {
      return pointingButtonToPointingButton(remapParams, fromButton, 0, toButton);
    }

    // ----------------------------------------
    void fireModifiers(const Params_KeyboardEventCallBack& params);

    void fireKey(const Params_KeyboardEventCallBack& params, const KeyRemap4MacBook_bridge::GetWorkspaceData::Reply& workspacedata);

    void fireKey(KeyEvent::KeyEvent eventType, unsigned int flags, KeyCode::KeyCode key, KeyboardType::KeyboardType keyboardType, const AbsoluteTime& ts,
                 const KeyRemap4MacBook_bridge::GetWorkspaceData::Reply& workspacedata);

    inline void fireKey(KeyEvent::KeyEvent eventType, unsigned int flags, KeyCode::KeyCode key,
                        const Params_KeyboardEventCallBack& params, const KeyRemap4MacBook_bridge::GetWorkspaceData::Reply& workspacedata) {
      RemapUtil::fireKey(eventType,
                         flags, key,
                         static_cast<KeyboardType::KeyboardType>(params.keyboardType),
                         params.ts,
                         workspacedata);

    }

    inline void fireKey(unsigned int flags, KeyCode::KeyCode key,
                        const Params_KeyboardEventCallBack& params, const KeyRemap4MacBook_bridge::GetWorkspaceData::Reply& workspacedata) {
      RemapUtil::fireKey(static_cast<KeyEvent::KeyEvent>(params.eventType), flags, key, params, workspacedata);
    }

    inline void fireKey_downup(unsigned int flags, KeyCode::KeyCode key,
                               const Params_KeyboardEventCallBack& params, const KeyRemap4MacBook_bridge::GetWorkspaceData::Reply& workspacedata) {
      if (RemapUtil::getKeyCodeModifier(key) == ModifierFlag::NONE) {
        RemapUtil::fireKey(KeyEvent::DOWN,   flags, key, params, workspacedata);
        RemapUtil::fireKey(KeyEvent::UP,     flags, key, params, workspacedata);
      } else {
        RemapUtil::fireKey(KeyEvent::MODIFY, flags, key, params, workspacedata);
      }
    }

    void fireConsumer(const Params_KeyboardSpecialEventCallback& params);

    // ----------------------------------------
    void pointingRelativeToScroll(const RemapPointingParams_relative& remapParams);
  }

  extern ListFireConsumerKey listFireConsumerKey;

  // ----------------------------------------------------------------------
  // for SandS like behavior remappings (remap_space2shift, remap_enter2optionL_commandSpace, ...)
  class KeyOverlaidModifier {
  public:
    void remap(const RemapParams& remapParams, KeyCode::KeyCode fromKeyCode, unsigned int fromFlags,
               ModifierFlag::ModifierFlag toFlag, KeyCode::KeyCode fireKeyCode, unsigned int fireFlags = ModifierFlag::NONE, bool isFireRepeat = false);

    // no-fromFlags version.
    void remap(const RemapParams& remapParams, KeyCode::KeyCode fromKeyCode,
               ModifierFlag::ModifierFlag toFlag, KeyCode::KeyCode fireKeyCode, unsigned int fireFlags = ModifierFlag::NONE, bool isFireRepeat = false) {
      remap(remapParams, fromKeyCode, 0, toFlag, fireKeyCode, fireFlags, isFireRepeat);
    }

    // no-isFireRepeat version.
    void remapWithRepeat(const RemapParams& remapParams, KeyCode::KeyCode fromKeyCode, unsigned int fromFlags,
                         ModifierFlag::ModifierFlag toFlag, KeyCode::KeyCode fireKeyCode, unsigned int fireFlags = ModifierFlag::NONE) {
      remap(remapParams, fromKeyCode, fromFlags, toFlag, fireKeyCode, fireFlags, true);
    }
    // no-fromFlags, no-isFireRepeat version.
    void remapWithRepeat(const RemapParams& remapParams, KeyCode::KeyCode fromKeyCode,
                         ModifierFlag::ModifierFlag toFlag, KeyCode::KeyCode fireKeyCode, unsigned int fireFlags = ModifierFlag::NONE) {
      remap(remapParams, fromKeyCode, toFlag, fireKeyCode, fireFlags, true);
    }

  private:
    bool isAnyEventHappen_;
    IntervalChecker ic_;
    RemapUtil::KeyToKey keytokey_;
    unsigned int savedflags_;
  };

  // ----------------------------------------
  // Command_R+Command_L to Escape, ...
  class KeyOverlaidModifierCombination {
  public:
    void remap(const RemapParams& remapParams, ModifierFlag::ModifierFlag fromFlag1, ModifierFlag::ModifierFlag fromFlag2,
               KeyCode::KeyCode fireKeyCode, unsigned int fireFlags = ModifierFlag::NONE);

  private:
    bool isAnyEventHappen_;
    bool isFireKey_;
    unsigned int savedflags_;
  };

  // ----------------------------------------
  // A modifier has DoublePressed key action.
  class DoublePressModifier {
  public:
    void remap(const RemapParams& remapParams, KeyCode::KeyCode fromKeyCode, ModifierFlag::ModifierFlag toFlag, KeyCode::KeyCode fireKeyCode, unsigned int fireFlags = ModifierFlag::NONE);

  private:
    int pressCount;
    RemapUtil::KeyToKey keytokey_;
  };

  // ----------------------------------------
  // Modifier Holding + Key -> Key
  class ModifierHoldingKeyToKey {
  public:
    void remap(const RemapParams& remapParams, ModifierFlag::ModifierFlag fromFlag, KeyCode::KeyCode fromKeyCode, KeyCode::KeyCode toKeyCode);

  private:
    IntervalChecker ic;
    bool doremap;
    bool first;
    RemapUtil::KeyToKey keytokey_;
  };

  // --------------------
  // ex. Ignore JIS_KANA x 2. (validate only the first once)
  class IgnoreMultipleSameKeyPress {
  public:
    bool remap(const RemapParams& remapParams, KeyCode::KeyCode fromKeyCode, unsigned int fromFlags = 0);

  private:
    unsigned int lastkeycode_;
  };

  // --------------------
  extern ListFireRelativePointer listFireRelativePointer;

  // --------------------
  class FirePointingScroll {
  public:
    void set(short int _deltaAxis1, short int _deltaAxis2, short int _deltaAxis3, IOFixed _fixedDelta1, IOFixed _fixedDelta2, IOFixed _fixedDelta3, SInt32 _pointDelta1, SInt32 _pointDelta2, SInt32 _pointDelta3) {
      enable = true;
      deltaAxis1 = _deltaAxis1;
      deltaAxis2 = _deltaAxis2;
      deltaAxis3 = _deltaAxis3;
      fixedDelta1 = _fixedDelta1;
      fixedDelta2 = _fixedDelta2;
      fixedDelta3 = _fixedDelta3;
      pointDelta1 = _pointDelta1;
      pointDelta2 = _pointDelta2;
      pointDelta3 = _pointDelta3;
    }
    void fire(ScrollWheelEventCallback callback, OSObject* target, IOHIPointing* pointing, AbsoluteTime ts);
    bool isEnable(void) const { return enable; }

  private:
    bool enable;
    short int deltaAxis1;
    short int deltaAxis2;
    short int deltaAxis3;
    IOFixed fixedDelta1;
    IOFixed fixedDelta2;
    IOFixed fixedDelta3;
    SInt32 pointDelta1;
    SInt32 pointDelta2;
    SInt32 pointDelta3;
  };

  extern FirePointingScroll firePointingScroll;

  // ----------------------------------------
  class PointingRelativeToScroll {
  public:
    bool remap(const RemapPointingParams_relative& remapParams, unsigned int button = 0, unsigned int fromFlags = 0);

  private:
    bool isButtonHeldDown;
  };
}

#endif
