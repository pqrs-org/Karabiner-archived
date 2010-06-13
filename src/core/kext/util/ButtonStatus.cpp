#include "ButtonStatus.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  ButtonStatus::Item ButtonStatus::item_[ButtonStatus::MAXNUM];
  Buttons ButtonStatus::justPressed_;
  Buttons ButtonStatus::justReleased_;

  // ----------------------------------------------------------------------
  bool
  ButtonStatus::initialize(void)
  {
    for (int i = 0; i < MAXNUM; ++i) {
      item_[i].initialize(1 << i);
    }

    justPressed_ = Buttons(0);
    justReleased_ = Buttons(0);

    return true;
  }

  int
  ButtonStatus::set(Buttons current, Buttons previous)
  {
    justPressed_ = Buttons(0);
    justReleased_ = Buttons(0);

    int diff = 0;
    for (int i = 0; i < MAXNUM; ++i) {
      int v = 0;
      v = item_[i].set(current, previous);
      if (v == 1) {
        justPressed_.add(item_[i].button_);
      } else if (v == -1) {
        justReleased_.add(item_[i].button_);
      }

      diff += v;
    }

    return diff;
  }

  void
  ButtonStatus::reset(void)
  {
    for (int i = 0; i < MAXNUM; ++i) {
      item_[i].reset();
    }
  }

  Buttons
  ButtonStatus::makeButtons(void)
  {
    Buttons flags;
    for (int i = 0; i < MAXNUM; ++i) {
      flags.add(item_[i].makeButtons());
    }
    return flags;
  }

  // ------------------------------------------------------------
#define FOREACH_TO_FLAGS(METHOD) {          \
    for (int i = 0; i < MAXNUM; ++i) {      \
      if (buttons.isOn(item_[i].button_)) { \
        item_[i].METHOD();                  \
      }                                     \
    }                                       \
}

  void ButtonStatus::increase(Buttons buttons) { FOREACH_TO_FLAGS(increase); }
  void ButtonStatus::decrease(Buttons buttons) { FOREACH_TO_FLAGS(decrease); }
#undef FOREACH_TO_FLAGS
}
