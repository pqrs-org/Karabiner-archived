#ifndef FLAGSTATUS_TEST
#include "CommonData.hpp"
#include "IOLogWrapper.hpp"
#endif

#include "FlagStatus.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  FlagStatus globalFlagStatus_;

  FlagStatus&
  FlagStatus::globalFlagStatus(void) {
    return globalFlagStatus_;
  }

  void
  FlagStatus::Item::initialize(ModifierFlag f)
  {
    flag_ = f;
    count_ = 0;
    temporary_count_ = 0;
    lock_count_ = 0;
    sticky_count_ = 0;
  }

  void
  FlagStatus::Item::set(void)
  {
    temporary_count_ = 0;
  }

  void
  FlagStatus::Item::set(KeyCode key, Flags flags)
  {
    temporary_count_ = 0;

    // ------------------------------------------------------------
    if (key != flag_.getKeyCode()) return;

    // ------------------------------------------------------------
    if (flag_ == ModifierFlag::CAPSLOCK) {
      if (flags.isOn(flag_)) {
        lock_count_ = 1;
      } else {
        lock_count_ = 0;
      }

    } else {
      if (flags.isOn(flag_)) {
        increase();
      } else {
        decrease();
      }
    }
  }

  void
  FlagStatus::Item::reset(void)
  {
    count_ = 0;
    temporary_count_ = 0;

    /*
       preserve lock_count.

       FlagStatus::reset is called when NumHeldDownKeys == 0,
       so we need remember the status of CapsLock.
     */
  }

  void
  FlagStatus::Item::increase(void)
  {
    if (flag_ == ModifierFlag::CAPSLOCK) {
      lock_count_ = ! lock_count_;
    } else {
      ++count_;
    }
  }

  void
  FlagStatus::Item::decrease(void)
  {
    if (flag_ == ModifierFlag::CAPSLOCK) {
      // do nothing (toggle at Item::increase).
    } else {
      --count_;
    }
  }

  // ----------------------------------------------------------------------
  FlagStatus::FlagStatus(void)
  {
    for (int i = 0;; ++i) {
      if (i >= MAXNUM) {
#ifndef FLAGSTATUS_TEST
        IOLOG_ERROR("FlagStatus::initialize MAXNUM is too small. Expand it.");
#endif
      }

      ModifierFlag f = Flags::getModifierFlagByIndex(i);
      item_[i].initialize(f);
      if (f == ModifierFlag::NONE) break;
    }
  }

  void
  FlagStatus::set(void)
  {
    for (int i = 0; item_[i].flag_ != ModifierFlag::NONE; ++i) {
      item_[i].set();
    }
  }

  void
  FlagStatus::set(KeyCode key, Flags flags)
  {
    for (int i = 0; item_[i].flag_ != ModifierFlag::NONE; ++i) {
      item_[i].set(key, flags);
    }
  }

  void
  FlagStatus::reset(void)
  {
    for (int i = 0; item_[i].flag_ != ModifierFlag::NONE; ++i) {
      item_[i].reset();
    }
  }

  bool
  FlagStatus::isOn(ModifierFlag modifierFlag) const
  {
    for (int i = 0; item_[i].flag_ != ModifierFlag::NONE; ++i) {
      if (item_[i].flag_ == modifierFlag) {
        return item_[i].sum() > 0;
      }
    }
    return false;
  }

  bool
  FlagStatus::isOn(const Vector_ModifierFlag& modifierFlags) const
  {
    bool strict = false;

    for (size_t i = 0; i < modifierFlags.size(); ++i) {
      if (modifierFlags[i] == ModifierFlag::NONE) {
        strict = true;
      } else {
        if (! isOn(modifierFlags[i])) return false;
      }
    }

    // If modifierFlags contains ModifierFlag::NONE,
    // return false when unspecified modifierflag is pressed.
    if (strict) {
      for (int i = 0; item_[i].flag_ != ModifierFlag::NONE; ++i) {
        if (item_[i].sum() > 0 &&
            ! modifierFlags.is_include(item_[i].flag_)) {
          return false;
        }
      }
    }

    return true;
  }

  Flags
  FlagStatus::makeFlags(void) const
  {
    Flags flags;
    for (int i = 0; item_[i].flag_ != ModifierFlag::NONE; ++i) {
      flags.add(item_[i].makeFlag());
    }
    return flags;
  }

  ModifierFlag
  FlagStatus::getFlag(int index) const
  {
    for (int i = 0; item_[i].flag_ != ModifierFlag::NONE; ++i) {
      if (i == index) {
        return item_[i].flag_;
      }
    }
    return ModifierFlag::NONE;
  }

  Flags
  FlagStatus::getLockedFlags(void) const
  {
    Flags f(0);
    for (int i = 0; item_[i].flag_ != ModifierFlag::NONE; ++i) {
      if (item_[i].lock_count_) {
        f.add(item_[i].flag_);
      }
    }
    return f;
  }

  Flags
  FlagStatus::getStickyFlags(void) const
  {
    Flags f(0);
    for (int i = 0; item_[i].flag_ != ModifierFlag::NONE; ++i) {
      if (item_[i].sticky_count_) {
        f.add(item_[i].flag_);
      }
    }
    return f;
  }

  // ------------------------------------------------------------
#define FOREACH_TO_FLAGS(METHOD) {                               \
    for (int i = 0; item_[i].flag_ != ModifierFlag::NONE; ++i) { \
      if (flags.isOn(item_[i].flag_)) {                          \
        item_[i].METHOD();                                       \
      }                                                          \
    }                                                            \
}
  void FlagStatus::increase(Flags flags)           { FOREACH_TO_FLAGS(increase);                               }
  void FlagStatus::decrease(Flags flags)           { FOREACH_TO_FLAGS(decrease);                               }
  void FlagStatus::temporary_increase(Flags flags) { FOREACH_TO_FLAGS(temporary_increase);                     }
  void FlagStatus::temporary_decrease(Flags flags) { FOREACH_TO_FLAGS(temporary_decrease);                     }
  void FlagStatus::lock_increase(Flags flags)      { FOREACH_TO_FLAGS(lock_increase);   updateStatusMessage(); }
  void FlagStatus::lock_decrease(Flags flags)      { FOREACH_TO_FLAGS(lock_decrease);   updateStatusMessage(); }
  void FlagStatus::lock_toggle(Flags flags)        { FOREACH_TO_FLAGS(lock_toggle);     updateStatusMessage(); }
  void FlagStatus::sticky_increase(Flags flags)    { FOREACH_TO_FLAGS(sticky_increase); updateStatusMessage(); }
  void FlagStatus::sticky_decrease(Flags flags)    { FOREACH_TO_FLAGS(sticky_decrease); updateStatusMessage(); }
  void FlagStatus::sticky_toggle(Flags flags)      { FOREACH_TO_FLAGS(sticky_toggle);   updateStatusMessage(); }
#undef FOREACH_TO_FLAGS

  void FlagStatus::sticky_clear(void) {
    for (int i = 0; item_[i].flag_ != ModifierFlag::NONE; ++i) {
      item_[i].sticky_decrease();
    }
    updateStatusMessage();
  }

  void
  FlagStatus::updateStatusMessage(void)
  {
#ifndef FLAGSTATUS_TEST
    int indexes[] = {
      BRIDGE_USERCLIENT_STATUS_MESSAGE_MODIFIER_LOCK,
      BRIDGE_USERCLIENT_STATUS_MESSAGE_MODIFIER_STICKY,
    };
    for (size_t i = 0; i < sizeof(indexes) / sizeof(indexes[0]); ++i) {
      int idx = indexes[i];
      Flags f(0);

      switch (idx) {
        case BRIDGE_USERCLIENT_STATUS_MESSAGE_MODIFIER_LOCK:   f = FlagStatus::getLockedFlags(); break;
        case BRIDGE_USERCLIENT_STATUS_MESSAGE_MODIFIER_STICKY: f = FlagStatus::getStickyFlags(); break;
      }

      if (f != statusMessageFlags_[idx]) {
        CommonData::clear_statusmessage(idx);

        if (f.isOn(ModifierFlag::FN))                                           { CommonData::append_statusmessage(idx, "FN "); }
        if (f.isOn(ModifierFlag::COMMAND_L) || f.isOn(ModifierFlag::COMMAND_R)) { CommonData::append_statusmessage(idx, "Cmd "); }
        if (f.isOn(ModifierFlag::CONTROL_L) || f.isOn(ModifierFlag::CONTROL_R)) { CommonData::append_statusmessage(idx, "Ctrl "); }
        if (f.isOn(ModifierFlag::OPTION_L) || f.isOn(ModifierFlag::OPTION_R))   { CommonData::append_statusmessage(idx, "Opt "); }
        if (f.isOn(ModifierFlag::SHIFT_L) || f.isOn(ModifierFlag::SHIFT_R))     { CommonData::append_statusmessage(idx, "Shift "); }
        if (f.isOn(ModifierFlag::EXTRA1))                                       { CommonData::append_statusmessage(idx, "Ex1 "); }
        if (f.isOn(ModifierFlag::EXTRA2))                                       { CommonData::append_statusmessage(idx, "Ex2 "); }
        if (f.isOn(ModifierFlag::EXTRA3))                                       { CommonData::append_statusmessage(idx, "Ex3 "); }
        if (f.isOn(ModifierFlag::EXTRA4))                                       { CommonData::append_statusmessage(idx, "Ex4 "); }
        if (f.isOn(ModifierFlag::EXTRA5))                                       { CommonData::append_statusmessage(idx, "Ex5 "); }

        CommonData::send_notification_statusmessage(idx);
      }
      statusMessageFlags_[idx] = f;
    }
#endif
  }
}
