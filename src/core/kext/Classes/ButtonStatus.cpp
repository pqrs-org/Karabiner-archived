#ifndef BUTTONSTATUS_TEST
#include "CommonData.hpp"
#endif

#include "ButtonStatus.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  ButtonStatus::Item ButtonStatus::item_[ButtonStatus::MAXNUM];
  Buttons ButtonStatus::statusMessageButtons_[BRIDGE_USERCLIENT_STATUS_MESSAGE__END__];

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

  Buttons
  ButtonStatus::getLockedButtons(void)
  {
    Buttons buttons(0);
    for (int i = 0; i < MAXNUM; ++i) {
      if (item_[i].lock_count_) {
        buttons.add(item_[i].button_);
      }
    }
    return buttons;
  }

  // ------------------------------------------------------------
#define FOREACH_TO_BUTTONS(METHOD) {        \
    for (int i = 0; i < MAXNUM; ++i) {      \
      if (buttons.isOn(item_[i].button_)) { \
        item_[i].METHOD();                  \
      }                                     \
    }                                       \
}

  void ButtonStatus::increase(Buttons buttons)      { FOREACH_TO_BUTTONS(increase);      }
  void ButtonStatus::decrease(Buttons buttons)      { FOREACH_TO_BUTTONS(decrease);      }
  void ButtonStatus::lock_increase(Buttons buttons) { FOREACH_TO_BUTTONS(lock_increase); updateStatusMessage(); }
  void ButtonStatus::lock_decrease(Buttons buttons) { FOREACH_TO_BUTTONS(lock_decrease); updateStatusMessage(); }
  void ButtonStatus::lock_toggle(Buttons buttons)   { FOREACH_TO_BUTTONS(lock_toggle);   updateStatusMessage(); }
#undef FOREACH_TO_BUTTONS

  void
  ButtonStatus::updateStatusMessage(void)
  {
#ifndef BUTTONSTATUS_TEST
    int indexes[] = {
      BRIDGE_USERCLIENT_STATUS_MESSAGE_POINTING_BUTTON_LOCK,
    };
    for (size_t i = 0; i < sizeof(indexes) / sizeof(indexes[0]); ++i) {
      int idx = indexes[i];
      Buttons b(0);

      switch (idx) {
        case BRIDGE_USERCLIENT_STATUS_MESSAGE_POINTING_BUTTON_LOCK: b = ButtonStatus::getLockedButtons(); break;
      }

      if (b != statusMessageButtons_[idx]) {
        CommonData::clear_statusmessage(idx);

        if (b.isOn(PointingButton::RIGHT))    { CommonData::append_statusmessage(idx, "Right ");    }
        if (b.isOn(PointingButton::MIDDLE))   { CommonData::append_statusmessage(idx, "Middle ");   }
        if (b.isOn(PointingButton::LEFT))     { CommonData::append_statusmessage(idx, "Left ");     }
        if (b.isOn(PointingButton::BUTTON4))  { CommonData::append_statusmessage(idx, "Button4 ");  }
        if (b.isOn(PointingButton::BUTTON5))  { CommonData::append_statusmessage(idx, "Button5 ");  }
        if (b.isOn(PointingButton::BUTTON6))  { CommonData::append_statusmessage(idx, "Button6 ");  }
        if (b.isOn(PointingButton::BUTTON7))  { CommonData::append_statusmessage(idx, "Button7 ");  }
        if (b.isOn(PointingButton::BUTTON8))  { CommonData::append_statusmessage(idx, "Button8 ");  }
        if (b.isOn(PointingButton::BUTTON9))  { CommonData::append_statusmessage(idx, "Button9 ");  }
        if (b.isOn(PointingButton::BUTTON10)) { CommonData::append_statusmessage(idx, "Button10 "); }
        if (b.isOn(PointingButton::BUTTON11)) { CommonData::append_statusmessage(idx, "Button11 "); }
        if (b.isOn(PointingButton::BUTTON12)) { CommonData::append_statusmessage(idx, "Button12 "); }
        if (b.isOn(PointingButton::BUTTON13)) { CommonData::append_statusmessage(idx, "Button13 "); }
        if (b.isOn(PointingButton::BUTTON14)) { CommonData::append_statusmessage(idx, "Button14 "); }
        if (b.isOn(PointingButton::BUTTON15)) { CommonData::append_statusmessage(idx, "Button15 "); }
        if (b.isOn(PointingButton::BUTTON16)) { CommonData::append_statusmessage(idx, "Button16 "); }

        CommonData::send_notification_statusmessage(idx);
      }
      statusMessageButtons_[idx] = b;
    }
#endif
  }
}
