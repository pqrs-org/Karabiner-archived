#include "Config.hpp"
#include "EventOutput.hpp"
#include "PointingRelativeToScroll.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace RemapFunc {
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
    PointingRelativeToScroll::add(Buttons newval)
    {
      fromButtons_ = newval;
    }

    bool
    PointingRelativeToScroll::remap(RemapPointingParams_relative& remapParams)
    {
      bool active = fromkeychecker_.isactive();

      if (remapParams.isremapped) return false;
      if (! fromkeychecker_.isFromButtons(fromButtons_, fromFlags_)) return false;
      remapParams.isremapped = true;

      if (fromButtons_ == PointingButton::NONE) {
        goto doremap;
      }

      // first time
      if (! active) {
        // if the source buttons contains left button, we cancel left click for iPhoto, or some applications.
        // iPhoto store the scroll events when left button is pressed, and restore events after left button is released.
        // PointingRelativeToScroll doesn't aim it, we release the left button and do normal scroll event.
        ButtonStatus::decrease(fromButtons_);
        EventOutput::FireRelativePointer::fire();
        ButtonStatus::increase(fromButtons_);

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
          ButtonStatus::increase(fromButtons_);
          EventOutput::FireRelativePointer::fire();
          ButtonStatus::decrease(fromButtons_);
          EventOutput::FireRelativePointer::fire();
        }
        return true;
      }

    doremap:
      FlagStatus::temporary_decrease(fromFlags_);
      toscroll(remapParams);

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

      // ----------------------------------------
      if (delta1 == 0 && delta2 == 0) return;

      Params_ScrollWheelEventCallback::auto_ptr ptr(Params_ScrollWheelEventCallback::alloc(0, 0, 0,
                                                                                           0, 0, 0,
                                                                                           0, 0, 0,
                                                                                           0));
      if (! ptr) return;
      Params_ScrollWheelEventCallback& params = *ptr;

      params.deltaAxis1 = (delta1 * config.pointing_relative2scroll_rate) / 1024;
      if (params.deltaAxis1 == 0 && delta1 != 0) {
        params.deltaAxis1 = delta1 > 0 ? 1 : -1;
      }
      params.deltaAxis2 = (delta2 * config.pointing_relative2scroll_rate) / 1024;
      if (params.deltaAxis2 == 0 && delta2 != 0) {
        params.deltaAxis2 = delta2 > 0 ? 1 : -1;
      }

      // ----------------------------------------
      params.fixedDelta1 = (delta1 * config.pointing_relative2scroll_rate) * (POINTING_FIXED_SCALE / 1024);
      params.fixedDelta2 = (delta2 * config.pointing_relative2scroll_rate) * (POINTING_FIXED_SCALE / 1024);

      params.pointDelta1 = (delta1 * POINTING_POINT_SCALE * config.pointing_relative2scroll_rate) / 1024;
      params.pointDelta2 = (delta2 * POINTING_POINT_SCALE * config.pointing_relative2scroll_rate) / 1024;

      EventOutput::FireScrollWheel::fire(params);

      absolute_distance_ += abs(delta1) + abs(delta2);
    }
  }
}
