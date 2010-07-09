#include "ButtonStatus.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  ButtonStatus::Item ButtonStatus::item_[ButtonStatus::MAXNUM];

  // ----------------------------------------------------------------------
  bool
  ButtonStatus::initialize(void)
  {
    for (int i = 0; i < MAXNUM; ++i) {
      item_[i].initialize(PointingButton(1 << i));
    }

    return true;
  }

  void
  ButtonStatus::set(PointingButton button, bool isbuttondown)
  {
    for (int i = 0; i < MAXNUM; ++i) {
      item_[i].set(button, isbuttondown);
    }
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
