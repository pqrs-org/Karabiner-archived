#include "Config.hpp"
#include "EventOutputQueue.hpp"
#include "PointingRelativeToScroll.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace RemapFunc {
    List* PointingRelativeToScroll::queue_ = NULL;
    Flags PointingRelativeToScroll::currentFromFlags_ = NULL;
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

    PointingRelativeToScroll::PointingRelativeToScroll(void) : index_(0)
    {}

    PointingRelativeToScroll::~PointingRelativeToScroll(void)
    {}

    void
    PointingRelativeToScroll::add(unsigned int datatype, unsigned int newval)
    {
      switch (datatype) {
        case BRIDGE_DATATYPE_FLAGS:
        {
          fromFlags_ = newval;
          break;
        }

        case BRIDGE_DATATYPE_POINTINGBUTTON:
        {
          fromButton_ = newval;
          break;
        }

        default:
          IOLOG_ERROR("PointingRelativeToScroll::add invalid datatype:%d\n", datatype);
          break;
      }
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
      toscroll(remapParams);

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
      if (Config::get_essential_config(BRIDGE_ESSENTIAL_CONFIG_INDEX_option_pointing_disable_vertical_scroll)) delta1 = 0;
      if (Config::get_essential_config(BRIDGE_ESSENTIAL_CONFIG_INDEX_option_pointing_disable_horizontal_scroll)) delta2 = 0;

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

      if (Config::get_essential_config(BRIDGE_ESSENTIAL_CONFIG_INDEX_option_pointing_enable_scrollwheel_fixation)) {
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
      queue_->push_back(new Item(chained_delta1_ * EventOutputQueue::FireScrollWheel::DELTA_SCALE, chained_delta2_ * EventOutputQueue::FireScrollWheel::DELTA_SCALE));

      currentFromFlags_ = fromFlags_;
      timer_.setTimeoutMS(SCROLL_INTERVAL_MS, false);
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
      FlagStatus::temporary_decrease(currentFromFlags_);
      EventOutputQueue::FireScrollWheel::fire(delta1, delta2);
      // We need to call temporary_increase.
      // Because normal cursor move event don't restore temporary_count_.
      // (See EventInputQueue::push_RelativePointerEventCallback.)
      FlagStatus::temporary_increase(currentFromFlags_);

      // ----------------------------------------
      if (! Config::get_essential_config(BRIDGE_ESSENTIAL_CONFIG_INDEX_option_pointing_disable_momentum_scroll)) {
        if (delta1 != 0 || delta2 != 0) {
          queue_->push_back(new Item(delta1 / 2, delta2 / 2));
        }
      }

      timer_.setTimeoutMS(SCROLL_INTERVAL_MS, false);
    }
  }
}
