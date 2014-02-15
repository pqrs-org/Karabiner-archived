#include <IOKit/IOLib.h>

#include "Config.hpp"
#include "EventOutputQueue.hpp"
#include "EventWatcher.hpp"
#include "IOLogWrapper.hpp"
#include "PointingRelativeToScroll.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace RemapFunc {
    List* PointingRelativeToScroll::queue_ = NULL;
    Flags PointingRelativeToScroll::currentFromFlags_ = Flags();
    Flags PointingRelativeToScroll::currentToFlags_ = Flags();
    TimerWrapper PointingRelativeToScroll::timer_;

    void
    PointingRelativeToScroll::static_initialize(IOWorkLoop& workloop)
    {
      queue_ = new List();
      timer_.initialize(&workloop, NULL, PointingRelativeToScroll::timer_callback);
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

    PointingRelativeToScroll::PointingRelativeToScroll(void) :
      error_(false),
      index_(0),
      index_type_(INDEX_TYPE_DEFAULT),
      isToKeysDefined_(false),
      absolute_distance_(0),
      chained_delta1_(0),
      chained_delta2_(0),
      fixation_delta1_(0),
      fixation_delta2_(0)
    {}

    PointingRelativeToScroll::~PointingRelativeToScroll(void)
    {}

    void
    PointingRelativeToScroll::add(unsigned int datatype, unsigned int newval)
    {
      switch (datatype) {
        case BRIDGE_DATATYPE_FLAGS:
        {
          switch (index_type_) {
            case INDEX_TYPE_DEFAULT:
              fromFlags_ = Flags(newval);
              break;
            case INDEX_TYPE_TOFLAGS:
              toFlags_ = Flags(newval);
              break;
            case INDEX_TYPE_TOKEYS:
              keytokey_.add(datatype, newval);
              break;
          }
          break;
        }

        case BRIDGE_DATATYPE_KEYCODE:
        case BRIDGE_DATATYPE_CONSUMERKEYCODE:
        case BRIDGE_DATATYPE_POINTINGBUTTON:
        {
          switch (index_type_) {
            case INDEX_TYPE_DEFAULT:
              if (datatype == BRIDGE_DATATYPE_POINTINGBUTTON) {
                fromEvent_ = FromEvent(datatype, newval);
              } else {
                IOLOG_ERROR("PointingRelativeToScroll::add invalid BRIDGE_DATATYPE_POINTINGBUTTON\n");
                error_ = true;
              }
              break;
            case INDEX_TYPE_TOKEYS:
              keytokey_.add(datatype, newval);
              break;
            default:
              IOLOG_ERROR("PointingRelativeToScroll::add invalid BRIDGE_DATATYPE_POINTINGBUTTON\n");
              error_ = true;
              break;
          }
          break;
        }

        case BRIDGE_DATATYPE_OPTION:
        {
          Option option(newval);
          if (Option::POINTINGRELATIVETOSCROLL_TOFLAGS == option) {
            index_type_ = INDEX_TYPE_TOFLAGS;
          }
          if (Option::POINTINGRELATIVETOSCROLL_TOKEYS == option) {
            index_type_ = INDEX_TYPE_TOKEYS;
            isToKeysDefined_ = true;
            keytokey_.add(KeyCode::VK_PSEUDO_KEY);
            keytokey_.add(fromFlags_);
          }
          break;
        }

        default:
          IOLOG_ERROR("PointingRelativeToScroll::add invalid datatype:%d\n", datatype);
          error_ = true;
          break;
      }
    }

    bool
    PointingRelativeToScroll::remap(RemapParams& remapParams)
    {
      Params_RelativePointerEventCallback* params = remapParams.paramsUnion.get_Params_RelativePointerEventCallback();
      if (! params) return false;

      // PointingRelativeToScroll grabs all pointing movement events.
      // Therefore, if user write inappropriate <autogen> (empty flags and empty buttons),
      // user cannot control pointing device at all.
      //
      // For example:
      //   <autogen>__PointingRelativeToScroll__ KeyCode::FN</autogen>
      //
      // (KeyCode::FN will be ignored. So, this autogen is interpreted as
      // <autogen>__PointingRelativeToScroll__</autogen>.)
      //
      // Skip on error in order to avoid this situation.
      if (error_) return false;

      bool active = fromEvent_.isPressing();

      if (remapParams.isremapped) return false;
      if (fromEvent_.getPointingButton() == PointingButton::NONE) {
        if (! FlagStatus::makeFlags().isOn(fromFlags_)) return false;
      } else {
        if (! fromEvent_.changePressingState(remapParams.paramsUnion, FlagStatus::makeFlags(), fromFlags_) &&
            ! active) {
          return false;
        }
      }
      remapParams.isremapped = true;

      if (fromEvent_.getPointingButton() == PointingButton::NONE) {
        goto doremap;
      }

      // first time
      if (! active) {
        // if the source buttons contains left button, we cancel left click for iPhoto, or some applications.
        // iPhoto store the scroll events when left button is pressed, and restore events after left button is released.
        // PointingRelativeToScroll doesn't aim it, we release the left button and do normal scroll event.
        ButtonStatus::decrease(fromEvent_.getPointingButton());
        EventOutputQueue::FireRelativePointer::fire();
        ButtonStatus::increase(fromEvent_.getPointingButton());

        absolute_distance_ = 0;
        begin_ic_.begin();
        chained_ic_.begin();
        chained_delta1_ = 0;
        chained_delta2_ = 0;

        goto doremap;
      }

      // last time
      if (! fromEvent_.isPressing()) {
        cancelScroll();

        const uint32_t DISTANCE_THRESHOLD = 5;
        const uint32_t TIME_THRESHOLD = 300;
        if (absolute_distance_ <= DISTANCE_THRESHOLD && begin_ic_.getmillisec() < TIME_THRESHOLD) {
          // Fire by a click event.
          if (isToKeysDefined_) {
            keytokey_.call_remap_with_VK_PSEUDO_KEY(EventType::DOWN);
            keytokey_.call_remap_with_VK_PSEUDO_KEY(EventType::UP);

          } else {
            ButtonStatus::increase(fromEvent_.getPointingButton());
            EventOutputQueue::FireRelativePointer::fire();
            ButtonStatus::decrease(fromEvent_.getPointingButton());
            EventOutputQueue::FireRelativePointer::fire();
          }
        }
        return true;
      }

    doremap:
      // We need to call EventWatcher::on here.
      // See the comments in EventInputQueue::fire_timer_callback.
      EventWatcher::on();
      toscroll(remapParams);

      return true;
    }

    void
    PointingRelativeToScroll::toscroll(RemapParams& remapParams)
    {
      Params_RelativePointerEventCallback* params = remapParams.paramsUnion.get_Params_RelativePointerEventCallback();
      if (! params) return;

      if (! queue_) return;

      // ----------------------------------------
      const uint32_t CANCEL_THRESHOLD = 100;
      if (chained_ic_.getmillisec() > CANCEL_THRESHOLD) {
        chained_delta1_ = 0;
        chained_delta2_ = 0;
        cancelScroll();
      }

      int delta1 = -params->dy;
      int delta2 = -params->dx;

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
      currentToFlags_ = toFlags_;
      timer_.setTimeoutMS(SCROLL_INTERVAL_MS, false);
    }

    void
    PointingRelativeToScroll::timer_callback(OSObject* owner, IOTimerEventSource* sender)
    {
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
      FlagStatus::temporary_increase(currentToFlags_);
      {
        int d1 = delta1;
        int d2 = delta2;
        if (Config::get_essential_config(BRIDGE_ESSENTIAL_CONFIG_INDEX_option_pointing_reverse_vertical_scrolling)) {
          d1 = -d1;
        }
        if (Config::get_essential_config(BRIDGE_ESSENTIAL_CONFIG_INDEX_option_pointing_reverse_horizontal_scrolling)) {
          d2 = -d2;
        }
        EventOutputQueue::FireScrollWheel::fire(d1, d2);
      }
      // We need to restore temporary flags.
      // Because normal cursor move event don't restore temporary_count_.
      // (See EventInputQueue::push_RelativePointerEventCallback.)
      FlagStatus::temporary_decrease(currentToFlags_);
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
