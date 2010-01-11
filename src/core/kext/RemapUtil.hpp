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
      KeyToKey(void) : active_(false) {}

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
#include "generate/output/include.keytokey.hpp"

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
      ConsumerToConsumer(void) : active_(false) {}

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
      PointingButtonToPointingButton(void) : active_(false) {}

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
    class PointingRelativeToScroll {
    public:
      PointingRelativeToScroll(void) :
        active_(false),
        buffered_delta1(0), buffered_delta2(0),
        fixation_delta1(0), fixation_delta2(0) {}

      bool remap(RemapPointingParams_relative& remapParams, const Buttons& buttons = 0, const Flags& fromFlags = 0);

    private:
      void toscroll(RemapPointingParams_relative& remapParams);

      bool active_;

      IntervalChecker buffered_ic_;
      int buffered_delta1;
      int buffered_delta2;

      IntervalChecker fixation_ic_;
      IntervalChecker fixation_begin_ic_;
      int fixation_delta1;
      int fixation_delta2;
    };

    // ----------------------------------------
    void fireKey(const Params_KeyboardEventCallBack& params, const KeyRemap4MacBook_bridge::GetWorkspaceData::Reply& workspacedata);
    void fireKey_downup(const Flags& flags, const KeyCode& key, const KeyboardType& keyboardType,
                        const KeyRemap4MacBook_bridge::GetWorkspaceData::Reply& workspacedata);

    void fireConsumer(const Params_KeyboardSpecialEventCallback& params);
    void fireRelativePointer(const Params_RelativePointerEventCallback& params);
    void fireScrollWheel(const Params_ScrollWheelEventCallback& params);

    // ----------------------------------------
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
    KeyOverlaidModifier(void) : isAnyEventHappen_(false), savedflags_(0) {}

#include "generate/output/include.keyoverlaidmodifier.hpp"

  private:
    bool isAnyEventHappen_;
    IntervalChecker ic_;
    RemapUtil::KeyToKey keytokey_;
    Flags savedflags_;
  };

  // ----------------------------------------
  // A modifier has DoublePressed key action.
  class DoublePressModifier {
  public:
    DoublePressModifier(void) : pressCount_(0) {}

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
    IgnoreMultipleSameKeyPress(void) : lastkeycode_(KeyCode::VK_NONE) {};

    bool remap(RemapParams& remapParams, const KeyCode& fromKeyCode, const Flags& fromFlags = 0);

  private:
    KeyCode lastkeycode_;
  };
}

#endif
