#include "Config.hpp"
#include "EventOutputQueue.hpp"
#include "PointingRelativeToScroll.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace RemapFunc {
    List* PointingRelativeToScroll::queue_ = NULL;
    TimerWrapper PointingRelativeToScroll::timer_;

    void
    PointingRelativeToScroll::static_initialize(IOWorkLoop& workloop)
    {
      queue_ = new List();
      timer_.initialize(&workloop, NULL, PointingRelativeToScroll::callback);
    }

    void
    PointingRelativeToScroll::static_terminate(void)
    {
      timer_.terminate();

      if (queue_) {
        delete queue_;
      }
    }

    void
    PointingRelativeToScroll::cancelScroll(void)
    {
      timer_.cancelTimeout();

      if (queue_) {
        queue_->clear();
      }
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
        chained_ic_.begin();
        chained_delta1_ = 0;
        chained_delta2_ = 0;

        goto doremap;
      }

      // last time
      if (! fromkeychecker_.isactive()) {
        cancelScroll();

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
      if (! queue_) return;

      // ----------------------------------------
      const uint32_t CANCEL_THRESHOLD = 100;
      if (chained_ic_.getmillisec() > CANCEL_THRESHOLD) {
        chained_delta1_ = 0;
        chained_delta2_ = 0;
        cancelScroll();
      }

      int delta1 = -remapParams.params.dy;
      int delta2 = -remapParams.params.dx;

      chained_ic_.begin();

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
          fixation_delta1_ = 0;
          fixation_delta2_ = 0;
        }
        fixation_ic_.begin();

        if (fixation_delta1_ > fixation_delta2_ * 2) {
          delta2 = 0;
        }
        if (fixation_delta2_ > fixation_delta1_ * 2) {
          delta1 = 0;
        }

        // Only first 1000ms performs the addition of fixation_delta1, fixation_delta2.
        const uint32_t FIXATION_EARLY_MILLISEC  = 1000;
        if (fixation_begin_ic_.getmillisec() < FIXATION_EARLY_MILLISEC) {
          if (delta1 == 0) fixation_delta2_ += abs2;
          if (delta2 == 0) fixation_delta1_ += abs1;
        }
      }

      // ------------------------------------------------------------
      // when sign is different
      if (0 > delta1 * chained_delta1_ ||
          0 > delta2 * chained_delta2_) {
        queue_->clear();
        chained_delta1_ = delta1;
        chained_delta2_ = delta2;

      } else if (abs(delta1) > abs(chained_delta1_) ||
                 abs(delta2) > abs(chained_delta2_)) {
        // greater delta.
        chained_delta1_ = delta1;
        chained_delta2_ = delta2;
      }

      absolute_distance_ += abs(chained_delta1_) + abs(chained_delta2_);
      queue_->push_back(new Item(chained_delta1_ * DELTA_SCALE, chained_delta2_ * DELTA_SCALE));

      timer_.setTimeoutMS(SCROLL_INTERVAL_MS, false);
    }

    void
    PointingRelativeToScroll::firescroll(int delta1, int delta2)
    {
      short deltaAxis1;
      short deltaAxis2;
      IOFixed fixedDelta1;
      IOFixed fixedDelta2;
      SInt32 pointDelta1;
      SInt32 pointDelta2;

      deltaAxis1 = (delta1 * config.pointing_relative2scroll_rate) / 1024 / DELTA_SCALE;
      deltaAxis2 = (delta2 * config.pointing_relative2scroll_rate) / 1024 / DELTA_SCALE;

      fixedDelta1 = (delta1 * config.pointing_relative2scroll_rate) * (POINTING_FIXED_SCALE / 1024) / DELTA_SCALE;
      fixedDelta2 = (delta2 * config.pointing_relative2scroll_rate) * (POINTING_FIXED_SCALE / 1024) / DELTA_SCALE;

      pointDelta1 = (delta1 * POINTING_POINT_SCALE * config.pointing_relative2scroll_rate) / 1024 / DELTA_SCALE;
      pointDelta2 = (delta2 * POINTING_POINT_SCALE * config.pointing_relative2scroll_rate) / 1024 / DELTA_SCALE;

      // see IOHIDSystem/IOHIDevicePrivateKeys.h about options.
      const int kScrollTypeContinuous_ = 0x0001;
      int options = kScrollTypeContinuous_;

      Params_ScrollWheelEventCallback::auto_ptr ptr(Params_ScrollWheelEventCallback::alloc(deltaAxis1,  deltaAxis2, 0,
                                                                                           fixedDelta1, fixedDelta2, 0,
                                                                                           pointDelta1, pointDelta2, 0,
                                                                                           options));
      if (! ptr) return;
      EventOutputQueue::FireScrollWheel::fire(*ptr);
    }

    void
    PointingRelativeToScroll::callback(OSObject* owner, IOTimerEventSource* sender)
    {
      IOLockWrapper::ScopedLock lk(timer_.getlock());

      if (! queue_) return;

      // ----------------------------------------
      int delta1 = 0;
      int delta2 = 0;
      {
        Item* p = static_cast<Item*>(queue_->front());
        if (! p) return;

        delta1 = p->delta1;
        delta2 = p->delta2;

        queue_->pop_front();
      }

      // ----------------------------------------
      firescroll(delta1, delta2);

      if (! config.option_pointing_disable_momentum_scroll) {
        if (delta1 != 0 || delta2 != 0) {
          queue_->push_back(new Item(delta1 / 2, delta2 / 2));
        }
      }

      timer_.setTimeoutMS(SCROLL_INTERVAL_MS, false);
    }
  }
}
