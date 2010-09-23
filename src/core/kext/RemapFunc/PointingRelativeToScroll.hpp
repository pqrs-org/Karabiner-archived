#ifndef POINTINGRELATIVETOSCROLL_HPP
#define POINTINGRELATIVETOSCROLL_HPP

#include "RemapFuncBase.hpp"
#include "FromKeyChecker.hpp"
#include "IntervalChecker.hpp"
#include "TimerWrapper.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace RemapFunc {
    class PointingRelativeToScroll {
    public:
      static void static_initialize(IOWorkLoop& workloop);
      static void static_terminate(void);
      static void cancelMomentumScroll(void);

      PointingRelativeToScroll(void) : index_(0) {}
      void initialize(void);
      void terminate(void);
      bool remap(RemapPointingParams_relative& remapParams);

      // ----------------------------------------
      // [0] => fromFlags_
      // [0] => fromButtons_
      void add(Flags newval);
      void add(PointingButton newval);

    private:
      enum {
        // see IOHIPointing.cpp in darwin.
        POINTING_FIXED_SCALE = 65536, // (== << 16)
        POINTING_POINT_SCALE = 10, // (== SCROLL_WHEEL_TO_PIXEL_SCALE >> 16)

        MOMENTUM_INTERVAL = 50,
        MOMENTUM_COUNT_MAX = 10,
      };
      void toscroll(RemapPointingParams_relative& remapParams);
      static unsigned int abs(int v) { return v > 0 ? v : -v; }
      static unsigned int absmax(int v1, int v2) {
        int a1 = abs(v1); int a2 = abs(v2);
        return (a1 > a2) ? a1 : a2;
      }
      static unsigned int absmin(int v1, int v2) {
        int a1 = abs(v1); int a2 = abs(v2);
        return (a1 > a2) ? a2 : a1;
      }
      static void firescroll(int delta1, int delta2);

      static void fireMomentumScroll(OSObject* owner, IOTimerEventSource* sender);

      // ----------
      size_t index_;
      FromKeyChecker fromkeychecker_;
      Flags fromFlags_;
      PointingButton fromButton_;

      // ----------
      unsigned int absolute_distance_;
      IntervalChecker begin_ic_;

      IntervalChecker buffered_ic_;
      int buffered_delta1;
      int buffered_delta2;

      IntervalChecker fixation_ic_;
      IntervalChecker fixation_begin_ic_;
      int fixation_delta1;
      int fixation_delta2;

      // ----------
      static TimerWrapper timer_;
      static int momentumCounter_;
      static int momentumDelta1_;
      static int momentumDelta2_;
    };
  }
}

#endif
