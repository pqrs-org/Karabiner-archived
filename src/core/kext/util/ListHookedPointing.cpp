#include "ListHookedPointing.hpp"
#include "Core.hpp"
#include "Config.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace ListHookedPointing {
    enum {
      MAXNUM = 16,
    };
    Item item[MAXNUM];

    bool
    append(IOHIPointing *pointing)
    {
      for (int i = 0; i < MAXNUM; ++i) {
        if (item[i].get()) continue;

        return item[i].initialize(pointing);
      }
      return false;
    }

    void
    terminate(void)
    {
      for (int i = 0; i < MAXNUM; ++i) {
        item[i].terminate();
      }
    }

    bool
    terminate(const IOHIPointing *pointing)
    {
      Item *p = get(pointing);
      if (p == NULL) return false;
      p->terminate();
      return true;
    }

    Item *
    get(const IOHIPointing *pointing)
    {
      if (pointing == NULL) return NULL;

      for (int i = 0; i < MAXNUM; ++i) {
        if (item[i].get() == pointing) return item + i;
      }
      return NULL;
    }

    Item *
    get(void)
    {
      for (int i = 0; i < MAXNUM; ++i) {
        if (item[i].get()) return item + i;
      }
      return NULL;
    }

    void
    refresh(void)
    {
      for (int i = 0; i < MAXNUM; ++i) {
        item[i].refresh();
      }
    }

    // ======================================================================
    bool
    Item::initialize(IOHIPointing *_pointing)
    {
      const char *name = _pointing->getName();
      if (! config.general_remap_thirdvendor_pointing) {
        if (strcmp(name, "IOHIDPointing") != 0 &&
            strcmp(name, "AppleUSBGrIIITrackpad") != 0 &&
            strcmp(name, "AppleADBMouseType4") != 0) {
          IOLog("KeyRemap4MacBook replacePointingEvent ignore device [%s]\n", name);
          return false;
        }
      }

      IOLog("KeyRemap4MacBook replacePointingEvent name = %s\n", name);
      pointing = _pointing;
      refresh();
      return true;
    }

    void
    Item::refresh(void)
    {
      if (pointing == NULL) return;

      {
        RelativePointerEventCallback old = getCurrent_relativePointerEventAction();

        if (old != hook_RelativePointerEventCallback) {
          IOLog("KeyRemap4MacBook [refresh] RelativePointerEventAction\n");

          orig_relativePointerEventAction = old;
          pointing->_relativePointerEventAction = reinterpret_cast<RelativePointerEventAction>(hook_RelativePointerEventCallback);

          orig_relativePointerEventTarget = pointing->_relativePointerEventTarget;
        }
      }
      {
        ScrollWheelEventCallback old = getCurrent_scrollWheelEventAction();

        if (old != hook_ScrollWheelEventCallback) {
          IOLog("KeyRemap4MacBook [refresh] ScrollWheelEventAction\n");

          orig_scrollWheelEventAction = old;
          pointing->_scrollWheelEventAction = reinterpret_cast<ScrollWheelEventAction>(hook_ScrollWheelEventCallback);

          orig_scrollWheelEventTarget = pointing->_scrollWheelEventTarget;
        }
      }
    }

    void
    Item::terminate(void)
    {
      if (! pointing) return;

      if (getCurrent_relativePointerEventAction() == hook_RelativePointerEventCallback) {
        pointing->_relativePointerEventAction = reinterpret_cast<RelativePointerEventAction>(orig_relativePointerEventAction);
      }
      if (getCurrent_scrollWheelEventAction() == hook_ScrollWheelEventCallback) {
        pointing->_scrollWheelEventAction = reinterpret_cast<ScrollWheelEventAction>(orig_scrollWheelEventAction);
      }
      pointing = NULL;
    }

    // ----------------------------------------------------------------------
    void
    hook_RelativePointerEventCallback(OSObject *target,
                                      int buttons,
                                      int dx,
                                      int dy,
                                      AbsoluteTime ts,
                                      OSObject *sender,
                                      void *refcon)
    {
      Params_RelativePointerEventCallback params = {
        target, buttons, dx, dy, ts, sender, refcon,
      };
      Core::remap_RelativePointerEventCallback(&params);
    }

    void
    hook_ScrollWheelEventCallback(OSObject * target,
                                  short deltaAxis1,
                                  short deltaAxis2,
                                  short deltaAxis3,
                                  IOFixed fixedDelta1,
                                  IOFixed fixedDelta2,
                                  IOFixed fixedDelta3,
                                  SInt32 pointDelta1,
                                  SInt32 pointDelta2,
                                  SInt32 pointDelta3,
                                  SInt32 options,
                                  AbsoluteTime ts,
                                  OSObject *sender,
                                  void *refcon)
    {
      Params_ScrollWheelEventCallback params = {
        target,
        deltaAxis1, deltaAxis2, deltaAxis3,
        fixedDelta1, fixedDelta2, fixedDelta3,
        pointDelta1, pointDelta2, pointDelta3,
        options, ts, sender, refcon,
      };
      Core::remap_ScrollWheelEventCallback(&params);
    }
  }
}
