#include "ListHookedPointing.hpp"
#include "Core.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace ListHookedPointing {
    enum {
      MAXNUM = 16,
    };
    Item item[MAXNUM];

    void
    terminate(void)
    {
      for (int i = 0; i < MAXNUM; ++i) {
        item[i].terminate();
      }
    }

    bool
    append(IOHIPointing *pointing)
    {
      for (int i = 0; i < MAXNUM; ++i) {
        if (item[i].get()) continue;

        item[i].initialize(pointing);
      }
      return false;
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
    void
    Item::initialize(IOHIPointing *_pointing)
    {
      pointing = _pointing;
      refresh();
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
  }
}
