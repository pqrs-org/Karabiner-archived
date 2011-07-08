#ifndef DEPENDINGPRESSINGPERIODKEYTOKEY_HPP
#define DEPENDINGPRESSINGPERIODKEYTOKEY_HPP

#include "RemapFuncClasses.hpp"
#include "FromKeyChecker.hpp"
#include "KeyToKey.hpp"
#include "TimerWrapper.hpp"

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
          SHORT_PERIOD,
          LONG_PERIOD,
          LONG_LONG_PERIOD,
          PRESSING_TARGET_KEY_ONLY,
          END_,
        };
      };
      class PeriodType {
      public:
        enum Value {
          NONE,
          SHORT_PERIOD,             // (1) in above description.
          LONG_PERIOD,              // (2) in above description.
          LONG_LONG_PERIOD,         // (3) in above description.
          PRESSING_TARGET_KEY_ONLY, // (4) in above description.
          END_,
        };
      };

      // ----------------------------------------
      static void static_initialize(IOWorkLoop& workloop);
      static void static_terminate(void);

      DependingPressingPeriodKeyToKey(void);
      ~DependingPressingPeriodKeyToKey(void);

      bool remap(RemapParams& remapParams);

      void add(KeyToKeyType::Value type, unsigned int datatype, unsigned int newval);

      void setPeriodMS(PeriodType::Value type, unsigned int newval);

    private:
      void dokeydown(void);
      void dokeyup(void);
      static void fire_timer_callback(OSObject* owner, IOTimerEventSource* sender);

      static TimerWrapper fire_timer_;
      static DependingPressingPeriodKeyToKey* target_;

      Flags savedflags_;

      bool active_;
      PeriodType::Value periodtype_;

      KeyToKey keytokey_[KeyToKeyType::END_];
      unsigned int periodMS_[PeriodType::END_];
    };
  }
}

#endif
