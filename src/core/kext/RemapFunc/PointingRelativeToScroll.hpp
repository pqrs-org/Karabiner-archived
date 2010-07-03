#ifndef POINTINGRELATIVETOSCROLL_HPP
#define POINTINGRELATIVETOSCROLL_HPP

#include "RemapFuncBase.hpp"
#include "FromKeyChecker.hpp"
#include "IntervalChecker.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace RemapFunc {
    class PointingRelativeToScroll {
    public:
      PointingRelativeToScroll(void) : index_(0) {}
      void initialize(void);
      void terminate(void);
      bool remap(RemapPointingParams_relative& remapParams);

      // ----------------------------------------
      // [0] => fromFlags_
      // [0] => fromButtons_
      void add(Flags newval);
      void add(Buttons newval);

    private:
      enum {
        // see IOHIPointing.cpp in darwin.
        POINTING_FIXED_SCALE = 65536, // (== << 16)
        POINTING_POINT_SCALE = 10, // (== SCROLL_WHEEL_TO_PIXEL_SCALE >> 16)
      };
      unsigned int abs(int v) { return v > 0 ? v : -v; }
      void toscroll(RemapPointingParams_relative& remapParams);

      // ----------
      size_t index_;
      FromKeyChecker fromkeychecker_;
      Flags fromFlags_;
      Buttons fromButtons_;

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
    };
  }
}

#endif
