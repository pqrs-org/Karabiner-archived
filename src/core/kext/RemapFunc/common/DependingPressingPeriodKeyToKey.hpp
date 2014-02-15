#ifndef DEPENDINGPRESSINGPERIODKEYTOKEY_HPP
#define DEPENDINGPRESSINGPERIODKEYTOKEY_HPP

#include "RemapFuncClasses.hpp"
#include "KeyToKey.hpp"
#include "TimerWrapper.hpp"
#include "IntervalChecker.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace RemapFunc {
    // ======================================================================
    // This class is an implementation of HoldingKeyToKey and KeyOverlaidModifier.
    //
    // DependingPressingPeriodKeyToKey modifies key event depending the pressing period.
    //
    // (1) Short period
    // (2) Long period
    // (3) Long Long period
    // (4) When we press the key only in (2)
    //
    // There are some parameters.
    // (A) The period of changing (2) from (1)
    // (B) The period of changing (3) from (2)
    // (C) The canceling period of (4)
    //
    //
    // When (A) == 200ms, (B) == 2000ms, (C) == 1000ms
    //
    // ---------+-------------------------------------------------------
    //          |  0ms    200ms(A)           1200ms(A+C)  2200ms(A+B)
    // Time     |   +-------+-------------------+-----------+---------->
    // (1)      |   <--(1)-->
    // (2)      |           <--------------(2)-------------->
    // (3)      |                                           <---(3)-----
    // (4)      |           <-------(4)--------->
    // ---------+-------------------------------------------------------
    //
    // ======================================================================
    // Case1: HoldingKeyToKey "Escape to Space (Holding Escape to Tab)"
    //
    // (1) Escape to Space
    // (2) Escape to Tab (with Repeat)
    // (3) Not Use
    // (4) Not Use
    //
    // (A) [Holding Key to Key] Holding Threshold
    // (B) Not Use
    // (C) Not Use
    //
    // ======================================================================
    // Case2: KeyOverlaidModifier "Space to Shift (when type Space only, send Tab)"
    //
    // (1) Space to Space (Don't change original key behavior)
    // (2) Space to Shift
    // (3) Not Use
    // (4) Space to Tab
    //
    // (A) [Key Overlaid Modifier] Initial Modifier Wait
    // (B) Not Use
    // (C) [Key Overlaid Modifier] Timeout
    //
    // ======================================================================
    // Case3: KeyOverlaidModifier "Space to Shift (when type Space only, send Tab) + [KeyRepeat]"
    //
    // (1) Space to Space (Don't change original key behavior)
    // (2) Space to Shift
    // (3) Space to Tab (Key Repeat)
    // (4) Space to Tab
    //
    // (A) [Key Overlaid Modifier] Initial Modifier Wait
    // (B) [Key Overlaid Modifier's Key Repeat] Initial Wait
    // (C) [Key Overlaid Modifier] Timeout
    //
    class DependingPressingPeriodKeyToKey {
    public:
      class KeyToKeyType {
      public:
        enum Value {
          FROM,
          SHORT_PERIOD,             // (1) in above description.
          LONG_PERIOD,              // (2) in above description.
          LONG_LONG_PERIOD,         // (3) in above description.
          PRESSING_TARGET_KEY_ONLY, // (4) in above description.
          END_,
        };
      };
      class PeriodMS {
      public:
        class Mode {
        public:
          enum Value {
            NONE,
            HOLDING_KEY_TO_KEY,
            KEY_OVERLAID_MODIFIER,
            KEY_OVERLAID_MODIFIER_WITH_REPEAT,
          };
        };
        class Type {
        public:
          enum Value {
            SHORT_PERIOD,             // (A) in above description.
            LONG_LONG_PERIOD,         // (B) in above description.
            PRESSING_TARGET_KEY_ONLY, // (C) in above description.
          };
        };

        PeriodMS(void);

        void set(Mode::Value newval);

        unsigned int get(Type::Value type);
        bool enabled(Type::Value type);

      private:
        Mode::Value mode_;
      };

      // ----------------------------------------
      static void static_initialize(IOWorkLoop& workloop);
      static void static_terminate(void);

      DependingPressingPeriodKeyToKey(void);
      ~DependingPressingPeriodKeyToKey(void);

      bool remap(RemapParams& remapParams);

      // About trick of KeyToKeyType::FROM's ModifierFlag.
      // Let's consider the following KeyOverlaidModifier.
      //
      // __KeyOverlaidModifier__ KeyCode::A, ModifierFlag::SHIFT_L, KeyCode::CONTROL_L, KeyCode::SPACE
      //
      // We separate the keys to KeyToKeyType::*.
      //
      // ------------------------------------------------------------
      // "KeyCode::A, ModifierFlag::SHIFT_L" -> KeyToKeyType::FROM
      // "KeyCode::CONTROL_L"                -> KeyToKeyType::LONG_PERIO
      // "KeyCode::SPACE"                    -> KeyToKeyType::SHORT_PERIOD,
      //                                        KeyToKeyType::PRESSING_TARGET_KEY_ONLY
      // --------------------
      // ** KeyToKeyType::FROM **
      //   KeyCode::A, ModifierFlag::SHIFT_L to KeyCode::VK_NONE
      //
      // ** KeyToKeyType::LONG_PERIO **
      //   KeyCode::VK_PSEUDO_KEY            to KeyCode::CONTROL_L
      //
      // ** KeyToKeyType::SHORT_PERIOD **
      //   KeyCode::VK_PSEUDO_KEY            to KeyCode::SPACE
      // ------------------------------------------------------------
      //
      //
      // However, in this case, ModifierFlag::SHIFT_L of KeyToKeyType::FROM is temporary flags
      // because toKey of KeyToKeyType::FROM is VK_NONE (== not modifier).
      // So, ModifierFlag::SHIFT_L will be restored if other keys are pressed.
      //
      // Therefore, we use a trick around KeyToKeyType::FROM's ModifierFlag.
      //
      // --------------------
      // ** KeyToKeyType::FROM **
      //   KeyCode::A,             ModifierFlag::SHIFT_L to KeyCode::VK_NONE
      //
      // ** KeyToKeyType::LONG_PERIO **
      //   KeyCode::VK_PSEUDO_KEY, ModifierFlag::SHIFT_L to KeyCode::CONTROL_L
      //
      // ** KeyToKeyType::SHORT_PERIOD **
      //   KeyCode::VK_PSEUDO_KEY, ModifierFlag::SHIFT_L to KeyCode::SPACE
      // ------------------------------------------------------------
      //
      // Then, ModifierFlag::SHIFT_L is treated properly.
      //
      void add(KeyToKeyType::Value type, unsigned int datatype, unsigned int newval);
      void add(KeyToKeyType::Value type, KeyCode newval) { add(type, BRIDGE_DATATYPE_KEYCODE, newval.get()); }
      void add(KeyToKeyType::Value type, Flags newval)   { add(type, BRIDGE_DATATYPE_FLAGS,   newval.get()); }
      void add(KeyToKeyType::Value type, Option newval)  { add(type, BRIDGE_DATATYPE_OPTION,  newval.get()); }

      void setPeriodMS(PeriodMS::Mode::Value newval) { periodMS_.set(newval); }

    private:
      class PeriodType {
      public:
        enum Value {
          NONE,
          SHORT_PERIOD,             // (1) in above description.
          LONG_PERIOD,              // (2) in above description.
          LONG_LONG_PERIOD,         // (3) in above description.
        };
      };

      void dokeydown(void);
      void dokeyup(void);
      static void fire_timer_callback(OSObject* owner, IOTimerEventSource* sender);

      static TimerWrapper fire_timer_;
      static DependingPressingPeriodKeyToKey* target_;

      Flags savedflags_;

      bool active_;
      PeriodType::Value periodtype_;

      KeyToKey keytokey_[KeyToKeyType::END_];
      PeriodMS periodMS_;

      bool isAnyEventHappen_;
      IntervalChecker ic_;
      int keyboardRepeatID_;

      bool interruptibleByScrollWheel_;
    };
  }
}

#endif
