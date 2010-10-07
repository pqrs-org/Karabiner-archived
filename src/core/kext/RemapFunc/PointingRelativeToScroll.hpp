#ifndef POINTINGRELATIVETOSCROLL_HPP
#define POINTINGRELATIVETOSCROLL_HPP

#include "RemapFuncBase.hpp"
#include "FromKeyChecker.hpp"
#include "IntervalChecker.hpp"
#include "List.hpp"
#include "TimerWrapper.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace RemapFunc {
    class PointingRelativeToScroll {
    public:
      static void static_initialize(IOWorkLoop& workloop);
      static void static_terminate(void);
      static void cancelScroll(void);

      PointingRelativeToScroll(void) : index_(0) {}
      void initialize(void);
      void terminate(void);
      bool remap(RemapPointingParams_relative& remapParams);

      // ----------------------------------------
      // [0] => fromFlags_
      // [0] => fromButtons_
      void add(unsigned int datatype, unsigned int newval);

    private:
      enum {
        SCROLL_INTERVAL_MS = 10,
      };

      class Item : public List::Item {
      public:
        Item(int d1, int d2) : delta1(d1), delta2(d2) {}
        virtual ~Item(void) {}

        int delta1;
        int delta2;
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

      static void callback(OSObject* owner, IOTimerEventSource* sender);

      // ----------
      size_t index_;
      FromKeyChecker fromkeychecker_;
      Flags fromFlags_;
      PointingButton fromButton_;

      // ----------
      unsigned int absolute_distance_;
      IntervalChecker begin_ic_;

      IntervalChecker chained_ic_;
      int chained_delta1_;
      int chained_delta2_;

      IntervalChecker fixation_ic_;
      IntervalChecker fixation_begin_ic_;
      int fixation_delta1_;
      int fixation_delta2_;

      // ----------
      static List* queue_;
      static Flags currentFromFlags_;
      static TimerWrapper timer_;
    };
  }
}

#endif
