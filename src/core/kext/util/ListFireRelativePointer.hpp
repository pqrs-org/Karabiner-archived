#ifndef LISTFIREPOINTER_HPP
#define LISTFIREPOINTER_HPP

#include "base.hpp"
#include "keycode.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  class ListFireRelativePointer {
  public:
    enum {
      MAXNUM = 4,
    };
    void reset(void) { size = 0; }
    bool isEmpty(void) { return size == 0; }
    void add(PointingButton::PointingButton button);
    void fire(RelativePointerEventCallback callback, OSObject *target, OSObject *sender, AbsoluteTime ts);

  private:
    class Item {
    public:
      void set(PointingButton::PointingButton _button) {
        button = _button;
      }
      PointingButton::PointingButton getButton(void) const { return button; }

    private:
      PointingButton::PointingButton button;
    };
    Item list[MAXNUM];
    int size;
  };
}

#endif
