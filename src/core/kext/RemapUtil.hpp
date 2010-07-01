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

namespace org_pqrs_KeyRemap4MacBook {
  namespace RemapUtil {
    enum {
      // see IOHIPointing.cpp in darwin.
      POINTING_FIXED_SCALE = 65536, // (== << 16)
      POINTING_POINT_SCALE = 10, // (== SCROLL_WHEEL_TO_PIXEL_SCALE >> 16)
    };

    inline unsigned int abs(int v) { return v > 0 ? v : -v; }

    // XXX: Delete Me. Use RemapFunc::KeyToKey instead.
    class KeyToKey {
    public:
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
      FromKeyChecker fromkeychecker_;
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
    };

    class PointingButtonToKey {
    public:
      PointingButtonToKey(void) : active_(false) {}

#include "generate/output/include.pointingbuttontokey.hpp"

    private:
      KeyToKey keytokey_;
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
  }
}

#endif
