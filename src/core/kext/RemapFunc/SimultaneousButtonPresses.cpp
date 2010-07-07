#include "SimultaneousButtonPresses.hpp"
#include "EventInputQueue.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace RemapFunc {
    void
    SimultaneousButtonPresses::initialize(void)
    {}

    void
    SimultaneousButtonPresses::terminate(void)
    {}

    void
    SimultaneousButtonPresses::add(PointingButton newval)
    {
      switch (index_) {
        case 0:
          fromButton1_ = newval;
          break;

        case 1:
          fromButton2_ = newval;
          break;

        case 2:
          toButton_ = newval;
          break;

        default:
          IOLOG_ERROR("Invalid SimultaneousButtonPresses::add\n");
          break;
      }
      ++index_;
    }

    void
    SimultaneousButtonPresses::add(Flags newval)
    {
      switch (index_) {
        case 0:
          IOLOG_ERROR("Invalid SimultaneousButtonPresses::add\n");
          break;

        case 1:
        case 2:
          fromFlags_ = newval;
          break;

        default:
          IOLOG_ERROR("Invalid SimultaneousButtonPresses::add\n");
          break;
      }
    }

    void
    SimultaneousButtonPresses::remap(void)
    {
      if (! EventInputQueue::queue_) return;

      // ------------------------------------------------------------
      if (active1_ || active2_) {
        // restore ButtonUp Event (fromButton1_).
        // drop    ButtonUp Event (fromButton2_).

        for (EventInputQueue::Item* p = static_cast<EventInputQueue::Item*>(EventInputQueue::queue_->front()); p; p = static_cast<EventInputQueue::Item*>(p->getnext())) {
          if (p->dropped) continue;
          if (p->params.type != ParamsUnion::RELATIVE_POINTER) continue;
          if (! (p->params).params.params_RelativePointerEventCallback) continue;
          Params_RelativePointerEventCallback& params = *((p->params).params.params_RelativePointerEventCallback);

          if (active1_ && params.ex_justReleased.isOn(fromButton1_)) {
            push_remapped(params, false);

            p->dropped = true;
            active1_ = false;
          }
          if (active2_ && params.ex_justReleased.isOn(fromButton2_)) {
            p->dropped = true;
            active2_ = false;
          }
        }
      }

      // ------------------------------------------------------------
      // find fromButtonCode*_
      //
      // We consider "LeftClick+RightClick to MiddleClick".
      // In this case, we change button in case1, but don't change button in case2.
      // Because LeftButton is released in case2.
      //
      // case1) "LeftButton Down", "RightButton Down"                   (=> change button)
      // case2) "LeftButton Down", "LeftButton Up", "RightButton Down". (=> don't change button)
      //
      // So, we check buttons from tail.

      EventInputQueue::Item* item1 = NULL;
      EventInputQueue::Item* item2 = NULL;
      EventInputQueue::Item* base = NULL; // later item of item1 or item2

      for (EventInputQueue::Item* p = static_cast<EventInputQueue::Item*>(EventInputQueue::queue_->back()); p; p = static_cast<EventInputQueue::Item*>(p->getprev())) {
        if (p->dropped) continue;
        if ((p->params).type != ParamsUnion::RELATIVE_POINTER) continue;
        if (! (p->params).params.params_RelativePointerEventCallback) continue;
        Params_RelativePointerEventCallback& params = *((p->params).params.params_RelativePointerEventCallback);

        if ((params.ex_justPressed.isOn(fromButton1_) || params.ex_justReleased.isOn(fromButton1_)) && ! item1) {
          item1 = p;
          if (! base) base = p;
        }
        if ((params.ex_justPressed.isOn(fromButton2_) || params.ex_justReleased.isOn(fromButton2_)) && ! item2) {
          item2 = p;
          if (! base) base = p;
        }
      }

      if (! item1 || ! item2 || ! base) return;

      Params_RelativePointerEventCallback& params1 = *((item1->params).params.params_RelativePointerEventCallback);
      Params_RelativePointerEventCallback& params2 = *((item2->params).params.params_RelativePointerEventCallback);
      Params_RelativePointerEventCallback& paramsbase = *((base->params).params.params_RelativePointerEventCallback);

      // replace first fromKeyCode1. and drop first fromKeyCode2
      if (params1.ex_justReleased.isOn(fromButton1_) ||
          params1.ex_justReleased.isOn(fromButton2_) ||
          params2.ex_justReleased.isOn(fromButton1_) ||
          params2.ex_justReleased.isOn(fromButton2_)) return;

      if (! FlagStatus::makeFlags().isOn(fromFlags_)) return;

      item1->dropped = true;
      active1_ = true;

      item2->dropped = true;
      active2_ = true;

      push_remapped(paramsbase, true);

      return;
    }

    void
    SimultaneousButtonPresses::push_remapped(const Params_RelativePointerEventCallback& baseparams, bool isButtonDown)
    {
      Params_RelativePointerEventCallback::auto_ptr ptr(Params_RelativePointerEventCallback::alloc(baseparams));
      if (! ptr) return;

      Params_RelativePointerEventCallback& params = *ptr;
      if (isButtonDown) {
        params.buttons = toButton_;
        params.ex_justPressed = toButton_;
        params.ex_justReleased = Buttons(0);
      } else {
        params.buttons = Buttons(0);
        params.ex_justPressed = Buttons(0);
        params.ex_justReleased = toButton_;
      }

      EventInputQueue::enqueue_(params);
    }
  }
}
