#include "Config.hpp"
#include "EventOutputQueue.hpp"
#include "PointingRelativeToScroll.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace RemapFunc {
    TimerWrapper PointingRelativeToScroll::timer_;
    int PointingRelativeToScroll::momentumCounter_;
    int PointingRelativeToScroll::momentumDelta1_;
    int PointingRelativeToScroll::momentumDelta2_;

    void
    PointingRelativeToScroll::static_initialize(IOWorkLoop& workloop)
    {
      timer_.initialize(&workloop, NULL, PointingRelativeToScroll::fireMomentumScroll);
      momentumCounter_ = 0;
      momentumDelta1_ = 0;
      momentumDelta2_ = 0;
    }

    void
    PointingRelativeToScroll::static_terminate(void)
    {
      timer_.terminate();
    }

    void
    PointingRelativeToScroll::cancelMomentumScroll(void)
    {
      timer_.cancelTimeout();
    }

    void
    PointingRelativeToScroll::initialize(void)
    {}

    void
    PointingRelativeToScroll::terminate(void)
    {}

    void
    PointingRelativeToScroll::add(Flags newval)
    {
      fromFlags_ = newval;
    }

    void
    PointingRelativeToScroll::add(PointingButton newval)
    {
      fromButton_ = newval;
    }

    bool
    PointingRelativeToScroll::remap(RemapPointingParams_relative& remapParams)
    {
      bool active = fromkeychecker_.isactive();

      if (remapParams.isremapped) return false;
      if (fromButton_ == PointingButton::NONE) {
        if (! FlagStatus::makeFlags().isOn(fromFlags_)) return false;
      } else {
        if (! fromkeychecker_.isFromPointingButton(remapParams.params, fromButton_, fromFlags_) && ! active) return false;
      }
      remapParams.isremapped = true;

      if (fromButton_ == PointingButton::NONE) {
        goto doremap;
      }

      // first time
      if (! active) {
        // if the source buttons contains left button, we cancel left click for iPhoto, or some applications.
        // iPhoto store the scroll events when left button is pressed, and restore events after left button is released.
        // PointingRelativeToScroll doesn't aim it, we release the left button and do normal scroll event.
        ButtonStatus::decrease(fromButton_);
        EventOutputQueue::FireRelativePointer::fire();
        ButtonStatus::increase(fromButton_);

        absolute_distance_ = 0;
        begin_ic_.begin();
        buffered_delta1 = 0;
        buffered_delta2 = 0;

        goto doremap;
      }

      // last time
      if (! fromkeychecker_.isactive()) {
        const uint32_t DISTANCE_THRESHOLD = 5;
        const uint32_t TIME_THRESHOLD = 300;
        if (absolute_distance_ <= DISTANCE_THRESHOLD && begin_ic_.getmillisec() < TIME_THRESHOLD) {
          // Fire by a click event.
          ButtonStatus::increase(fromButton_);
          EventOutputQueue::FireRelativePointer::fire();
          ButtonStatus::decrease(fromButton_);
          EventOutputQueue::FireRelativePointer::fire();
        }
        return true;
      }

    doremap:
      FlagStatus::temporary_decrease(fromFlags_);
      toscroll(remapParams);
      // We need to call temporary_increase.
      // Because when SimultaneousKeyPresses is enabled, temporary flags will be reset in unexpected timing.
      // So, we need to restore temporary flags explicitly.
      FlagStatus::temporary_increase(fromFlags_);

      return true;
    }

    void
    PointingRelativeToScroll::toscroll(RemapPointingParams_relative& remapParams)
    {
      // ----------------------------------------
      // Buffer processing

      // buffer events in 20ms (60fps)
      const uint32_t BUFFER_MILLISEC = 20;

      buffered_delta1 += -remapParams.params.dy;
      buffered_delta2 += -remapParams.params.dx;

      if (buffered_ic_.getmillisec() < BUFFER_MILLISEC) {
        return;
      }

      int delta1 = buffered_delta1;
      int delta2 = buffered_delta2;
      buffered_delta1 = 0;
      buffered_delta2 = 0;
      buffered_ic_.begin();

      // ----------------------------------------
      if (config.option_pointing_disable_vertical_scroll) delta1 = 0;
      if (config.option_pointing_disable_horizontal_scroll) delta2 = 0;

      // ----------------------------------------
      // ignore minuscule move
      const unsigned int abs1 = abs(delta1);
      const unsigned int abs2 = abs(delta2);

      if (abs1 > abs2 * 2) {
        delta2 = 0;
      }
      if (abs2 > abs1 * 2) {
        delta1 = 0;
      }

      // ----------------------------------------
      // Fixation processing

      if (config.option_pointing_enable_scrollwheel_fixation) {
        // When 300ms passes from the last event, we reset a value.
        const uint32_t FIXATION_MILLISEC = 300;
        if (fixation_ic_.getmillisec() > FIXATION_MILLISEC) {
          fixation_begin_ic_.begin();
          fixation_delta1 = 0;
          fixation_delta2 = 0;
        }
        fixation_ic_.begin();

        if (fixation_delta1 > fixation_delta2 * 2) {
          delta2 = 0;
        }
        if (fixation_delta2 > fixation_delta1 * 2) {
          delta1 = 0;
        }

        // Only first 1000ms performs the addition of fixation_delta1, fixation_delta2.
        const uint32_t FIXATION_EARLY_MILLISEC  = 1000;
        if (fixation_begin_ic_.getmillisec() < FIXATION_EARLY_MILLISEC) {
          if (delta1 == 0) fixation_delta2 += abs2;
          if (delta2 == 0) fixation_delta1 += abs1;
        }
      }

      firescroll(delta1, delta2);
      absolute_distance_ += abs(delta1) + abs(delta2);

      momentumCounter_ = absmax(delta1, delta2);
      momentumDelta1_ = delta1;
      momentumDelta2_ = delta2;
      timer_.setTimeoutMS(MOMENTUM_INTERVAL);
    }

    void
    PointingRelativeToScroll::firescroll(int delta1, int delta2)
    {
      if (delta1 == 0 && delta2 == 0) return;

      short deltaAxis1;
      short deltaAxis2;
      IOFixed fixedDelta1;
      IOFixed fixedDelta2;
      SInt32 pointDelta1;
      SInt32 pointDelta2;

      deltaAxis1 = (delta1 * config.pointing_relative2scroll_rate) / 1024;
      if (deltaAxis1 == 0 && delta1 != 0) {
        deltaAxis1 = delta1 > 0 ? 1 : -1;
      }
      deltaAxis2 = (delta2 * config.pointing_relative2scroll_rate) / 1024;
      if (deltaAxis2 == 0 && delta2 != 0) {
        deltaAxis2 = delta2 > 0 ? 1 : -1;
      }

      // ----------------------------------------
      fixedDelta1 = (delta1 * config.pointing_relative2scroll_rate) * (POINTING_FIXED_SCALE / 1024);
      fixedDelta2 = (delta2 * config.pointing_relative2scroll_rate) * (POINTING_FIXED_SCALE / 1024);

      pointDelta1 = (delta1 * POINTING_POINT_SCALE * config.pointing_relative2scroll_rate) / 1024;
      pointDelta2 = (delta2 * POINTING_POINT_SCALE * config.pointing_relative2scroll_rate) / 1024;

      Params_ScrollWheelEventCallback::auto_ptr ptr(Params_ScrollWheelEventCallback::alloc(deltaAxis1,  deltaAxis2, 0,
                                                                                           fixedDelta1, fixedDelta2, 0,
                                                                                           pointDelta1, pointDelta2, 0,
                                                                                           0));
      if (! ptr) return;
      EventOutputQueue::FireScrollWheel::fire(*ptr);
    }

    void
    PointingRelativeToScroll::fireMomentumScroll(OSObject* owner, IOTimerEventSource* sender)
    {
      IOLockWrapper::ScopedLock lk(timer_.getlock());

      if (momentumDelta1_ == 0 && momentumDelta2_ == 0) return;

      firescroll(momentumDelta1_, momentumDelta2_);

      --momentumCounter_;
      if (momentumCounter_ < 0) {
        momentumCounter_ = absmax(momentumDelta1_, momentumDelta2_);

        momentumDelta1_ /= 2;
        momentumDelta2_ /= 2;
        if (abs(momentumDelta1_) <= 2) momentumDelta1_ = 0;
        if (abs(momentumDelta2_) <= 2) momentumDelta2_ = 0;
      }

      timer_.setTimeoutMS(MOMENTUM_INTERVAL);
    }
  }
}
