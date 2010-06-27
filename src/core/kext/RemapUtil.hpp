#ifndef REMAPUTIL_HPP
#define REMAPUTIL_HPP

#include <IOKit/hidsystem/IOHIKeyboard.h>
#include "bridge.hpp"
#include "remap.hpp"
#include "CommonData.hpp"
#include "KeyCode.hpp"
#include "util/ButtonStatus.hpp"
#include "util/FlagStatus.hpp"
#include "util/FromKeyChecker.hpp"
#include "util/PressDownKeys.hpp"
#include "util/IntervalChecker.hpp"
#include "util/EventWatcher.hpp"
#include "util/Vector.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace RemapUtil {
    struct PairKeyFlags {
      PairKeyFlags(void) {}
      PairKeyFlags(KeyCode k) : key(k) {}

      KeyCode key;
      Flags   flags;
    };
    DECLARE_VECTOR(PairKeyFlags);

    struct PairConsumerKeyFlags {
      PairConsumerKeyFlags(void) {}
      PairConsumerKeyFlags(ConsumerKeyCode k) : key(k) {}

      ConsumerKeyCode key;
      Flags           flags;
    };
    DECLARE_VECTOR(PairConsumerKeyFlags);

    enum {
      // see IOHIPointing.cpp in darwin.
      POINTING_FIXED_SCALE = 65536, // (== << 16)
      POINTING_POINT_SCALE = 10, // (== SCROLL_WHEEL_TO_PIXEL_SCALE >> 16)
    };

    inline unsigned int abs(int v) { return v > 0 ? v : -v; }

    class KeyToKey {
    public:
      KeyToKey(void) : index_(0) {}
      bool remap(RemapParams& remapParams);

      // ----------------------------------------
      // [0] => fromKey_
      // [1] => toKeys_[0]
      // [2] => toKeys_[1]
      // [3] => ...
      void add(KeyCode newval);
      void add(Flags newval);

      // Don't use reference for a Flags argument.
      // Because Flags is generated from a combination of ModifierFlag anytime,
      // it wastes the stack of the caller if we use a reference argument.
      bool remap(RemapParams& remapParams,
                 KeyCode fromKeyCode, Flags fromFlags,
                 KeyCode toKeyCode,   Flags toFlags = ModifierFlag::NONE,
                 bool isSetKeyRepeat = true);
      // no-fromFlags version.
      bool remap(RemapParams& remapParams,
                 KeyCode fromKeyCode,
                 KeyCode toKeyCode, Flags toFlags = ModifierFlag::NONE,
                 bool isSetKeyRepeat = true) {
        return remap(remapParams, fromKeyCode, 0, toKeyCode, toFlags, isSetKeyRepeat);
      }

      // --------------------------------------------------
      // Combination
#include "generate/output/include.keytokey.hpp"

    private:
      size_t index_;
      FromKeyChecker fromkeychecker_;
      PairKeyFlags fromKey_;
      Vector_PairKeyFlags toKeys_;
    };

    class ConsumerToConsumer {
    public:
      ConsumerToConsumer(void) : index_(0) {}
      bool remap(RemapConsumerParams& remapParams);

      // ----------------------------------------
      // [0] => fromKey_
      // [1] => toKeys_[0]
      // [2] => toKeys_[1]
      // [3] => ...
      void add(ConsumerKeyCode newval) {
        switch (index_) {
          case 0:
            fromKey_.key = newval;
            break;
          default:
            toKeys_.push_back(PairConsumerKeyFlags(newval));
            break;
        }
        ++index_;
      }
      void add(Flags newval) {
        switch (index_) {
          case 0:
            IOLOG_ERROR("Invalid ConsumerToConsumer::add\n");
            break;
          case 1:
            fromKey_.flags = newval;
            break;
          default:
            toKeys_.back().flags = newval;
            break;
        }
      }

    private:
      size_t index_;
      FromKeyChecker fromkeychecker_;
      PairConsumerKeyFlags fromKey_;
      Vector_PairConsumerKeyFlags toKeys_;
    };

    class KeyToConsumer {
    public:
      KeyToConsumer(void) : index_(0) {}
      bool remap(RemapParams& remapParams);

      // ----------------------------------------
      // [0] => fromKey
      // [1] => toKeys[0]
      // [2] => toKeys[1]
      // [3] => ...
      void add(KeyCode newval) {
        switch (index_) {
          case 0:
            fromKey_.key = newval;
            break;
          default:
            IOLOG_ERROR("Invalid KeyToConsumer::add\n");
            break;
        }
        ++index_;
      }
      void add(ConsumerKeyCode newval) {
        switch (index_) {
          case 0:
            IOLOG_ERROR("Invalid KeyToConsumer::add\n");
            break;
          case 1:
            consumertoconsumer_.add(ConsumerKeyCode::VK_PSEUDO_KEY);
            consumertoconsumer_.add(fromKey_.flags);
            // pass-through
          default:
            consumertoconsumer_.add(newval);
            break;
        }
        ++index_;
      }
      void add(Flags newval) {
        switch (index_) {
          case 0:
            IOLOG_ERROR("Invalid KeyToConsumer::add\n");
            break;
          case 1:
            fromKey_.flags = newval;
            break;
          default:
            consumertoconsumer_.add(newval);
            break;
        }
      }

    private:
      size_t index_;
      FromKeyChecker fromkeychecker_;
      PairKeyFlags fromKey_;
      KeyToKey keytokey_;
      ConsumerToConsumer consumertoconsumer_;
    };

    class ConsumerToKey {
    public:
      ConsumerToKey(void) : index_(0) {}

      bool remap(RemapConsumerParams& remapParams);

      // [0] => fromKey
      // [1] => toKeys[0]
      // [2] => toKeys[1]
      // [3] => ...
      void add(ConsumerKeyCode newval) {
        switch (index_) {
          case 0:
            fromKey_.key = newval;
            consumertoconsumer_.add(newval);
            consumertoconsumer_.add(ConsumerKeyCode::VK_NONE);
            break;
          default:
            IOLOG_ERROR("Invalid ConsumerToKey::add\n");
            break;
        }
        ++index_;
      }
      void add(KeyCode newval) {
        switch (index_) {
          case 0:
            IOLOG_ERROR("Invalid ConsumerToKey::add\n");
            break;
          default:
            toKeys_.push_back(PairKeyFlags(newval));
            break;
        }
        ++index_;
      }
      void add(Flags newval) {
        switch (index_) {
          case 0:
            break;
          case 1:
            fromKey_.flags = newval;
            break;
          default:
            toKeys_.back().flags = newval;
            break;
        }
      }

    private:
      size_t index_;
      FromKeyChecker fromkeychecker_;
      PairConsumerKeyFlags fromKey_;
      KeyToKey keytokey_;
      ConsumerToConsumer consumertoconsumer_;

      // XXX: A hack until KeyToKey being completed
      Vector_PairKeyFlags toKeys_;
    };

    class PointingButtonToPointingButton {
    public:
      PointingButtonToPointingButton(void) : active_(false) {}

      bool remap(RemapPointingParams_relative& remapParams,
                 PointingButton fromButton, Flags fromFlags,
                 PointingButton toButton,   Flags toFlags = ModifierFlag::NONE);

      // no fromFlags version
      bool remap(RemapPointingParams_relative& remapParams,
                 PointingButton fromButton,
                 PointingButton toButton,   Flags toFlags = ModifierFlag::NONE) {
        return remap(remapParams, fromButton, 0, toButton, toFlags);
      }

    private:
      bool active_;
    };

    class KeyToPointingButton {
    public:
      KeyToPointingButton(void) : active_(false) {}

      bool remap(RemapParams& remapParams,
                 KeyCode fromKeyCode, Flags fromFlags,
                 PointingButton toButton);

      // no fromFlags version
      bool remap(RemapParams& remapParams,
                 KeyCode fromKeyCode,
                 PointingButton toButton) {
        return remap(remapParams, fromKeyCode, 0, toButton);
      }

    private:
      bool active_;
      PointingButtonToPointingButton buttontobutton_;
    };

    class PointingButtonToKey {
    public:
      PointingButtonToKey(void) : active_(false) {}

#include "generate/output/include.pointingbuttontokey.hpp"

    private:
      KeyToKey keytokey_;
      PointingButtonToPointingButton buttontobutton_;
      bool active_;
    };

    // ----------------------------------------
    class PointingRelativeToScroll {
    public:
      PointingRelativeToScroll(void) :
        active_(false),
        absolute_distance_(0),
        buffered_delta1(0), buffered_delta2(0),
        fixation_delta1(0), fixation_delta2(0) {}

      bool remap(RemapPointingParams_relative& remapParams, Buttons buttons = 0, Flags fromFlags = 0);

    private:
      void toscroll(RemapPointingParams_relative& remapParams);

      bool active_;

      unsigned int absolute_distance_;
      IntervalChecker begin_ic_;

      IntervalChecker buffered_ic_;
      int buffered_delta1;
      int buffered_delta2;

      IntervalChecker fixation_ic_;
      IntervalChecker fixation_begin_ic_;
      int fixation_delta1;
      int fixation_delta2;
    };

    // ----------------------------------------
    void fireKey(const Params_KeyboardEventCallBack& params);
    void fireKey_downup(Flags flags, KeyCode key, KeyboardType keyboardType);

    void fireConsumer(const Params_KeyboardSpecialEventCallback& params);
    void fireScrollWheel(const Params_ScrollWheelEventCallback& params);
  }

  class FireModifiers {
  public:
    static void fire(Flags toFlags = FlagStatus::makeFlags(), KeyboardType keyboardType = CommonData::getcurrent_keyboardType());

  private:
    static Flags lastFlags_;
  };

  class FireRelativePointer {
  public:
    static void fire(Buttons toButtons = ButtonStatus::makeButtons(), int dx = 0, int dy = 0);

  private:
    static Buttons lastButtons_;
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
               KeyCode fromKeyCode, KeyCode toKeyCode,
               KeyCode fireKeyCode, Flags fireFlags = ModifierFlag::NONE);

  private:
    int pressCount_;
    IntervalChecker ic_;
    RemapUtil::KeyToKey keytokey_;
  };

  // ----------------------------------------
  // Modifier Holding + Key -> Key
  class ModifierHoldingKeyToKey {
  public:
    bool remap(RemapParams& remapParams, KeyCode fromKeyCode, Flags fromFlags, KeyCode toKeyCode);

  private:
    IntervalChecker ic_;
    RemapUtil::KeyToKey keytokey_;
  };

  class HoldingKeyToKey {
  public:
    static void initialize(IOWorkLoop& workloop);
    static void terminate(void);

    bool remap(RemapParams& remapParams, KeyCode fromKeyCode, Flags fromFlags,
               KeyCode toKeyCode_normal,  Flags toFlags_normal,
               KeyCode toKeyCode_holding, Flags toFlags_holding);

    // no toFlags_normal
    bool remap(RemapParams& remapParams, KeyCode fromKeyCode, Flags fromFlags,
               KeyCode toKeyCode_normal,
               KeyCode toKeyCode_holding, Flags toFlags_holding) {
      return remap(remapParams, fromKeyCode, fromFlags,
                   toKeyCode_normal,  ModifierFlag::NONE,
                   toKeyCode_holding, toFlags_holding);
    }
    // no toFlags_holding
    bool remap(RemapParams& remapParams, KeyCode fromKeyCode, Flags fromFlags,
               KeyCode toKeyCode_normal,  Flags toFlags_normal,
               KeyCode toKeyCode_holding) {
      return remap(remapParams, fromKeyCode, fromFlags,
                   toKeyCode_normal,  toFlags_normal,
                   toKeyCode_holding, ModifierFlag::NONE);
    }
    // no toFlags_normal, toFlags_holding
    bool remap(RemapParams& remapParams, KeyCode fromKeyCode, Flags fromFlags,
               KeyCode toKeyCode_normal,
               KeyCode toKeyCode_holding) {
      return remap(remapParams, fromKeyCode, fromFlags,
                   toKeyCode_normal,  ModifierFlag::NONE,
                   toKeyCode_holding, ModifierFlag::NONE);
    }

    // no fromFlags
    bool remap(RemapParams& remapParams, KeyCode fromKeyCode,
               KeyCode toKeyCode_normal,  Flags toFlags_normal,
               KeyCode toKeyCode_holding, Flags toFlags_holding) {
      return remap(remapParams, fromKeyCode, Flags(0),
                   toKeyCode_normal,  toFlags_normal,
                   toKeyCode_holding, toFlags_holding);
    }

    // no fromFlags, toFlags_normal
    bool remap(RemapParams& remapParams, KeyCode fromKeyCode,
               KeyCode toKeyCode_normal,
               KeyCode toKeyCode_holding, Flags toFlags_holding) {
      return remap(remapParams, fromKeyCode, Flags(0),
                   toKeyCode_normal,  ModifierFlag::NONE,
                   toKeyCode_holding, toFlags_holding);
    }
    // no fromFlags, toFlags_holding
    bool remap(RemapParams& remapParams, KeyCode fromKeyCode,
               KeyCode toKeyCode_normal,  Flags toFlags_normal,
               KeyCode toKeyCode_holding) {
      return remap(remapParams, fromKeyCode, Flags(0),
                   toKeyCode_normal,  toFlags_normal,
                   toKeyCode_holding, ModifierFlag::NONE);
    }
    // no fromFlags, toFlags_normal, toFlags_holding
    bool remap(RemapParams& remapParams, KeyCode fromKeyCode,
               KeyCode toKeyCode_normal,
               KeyCode toKeyCode_holding) {
      return remap(remapParams, fromKeyCode, Flags(0),
                   toKeyCode_normal,  ModifierFlag::NONE,
                   toKeyCode_holding, ModifierFlag::NONE);
    }

  private:
    static void fireholding(OSObject* owner, IOTimerEventSource* sender);

    static TimerWrapper timer_;
    static KeyCode toKeyCode_holding_;
    static Flags toFlags_holding_;
    static bool isfirenormal_;
    static bool isfireholding_;
    RemapUtil::KeyToKey keytokey_;
  };

  // --------------------
  // ex. Ignore JIS_KANA x 2. (validate only the first once)
  class IgnoreMultipleSameKeyPress {
  public:
    IgnoreMultipleSameKeyPress(void) : lastkeycode_(KeyCode::VK_NONE) {};

    bool remap(RemapParams& remapParams);

    // ----------------------------------------
    void add(KeyCode newval) {
      fromKey_.key = newval;
    }
    void add(Flags newval) {
      fromKey_.flags = newval;
    }

  private:
    RemapUtil::PairKeyFlags fromKey_;
    KeyCode lastkeycode_;
  };
}

#endif
