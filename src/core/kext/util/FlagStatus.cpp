#include "FlagStatus.hpp"

#ifndef FLAGSTATUS_TEST
#include "base.hpp"
#include "Config.hpp"
#include "Client.hpp"
#endif

namespace org_pqrs_KeyRemap4MacBook {
  FlagStatus::Item FlagStatus::item_[FlagStatus::MAXNUM];
  Flags FlagStatus::statusMessageFlags_(0);

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
       so we need remember the status of CapsLock, NumLock, ...
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
  bool
  FlagStatus::initialize(void)
  {
    for (int i = 0;; ++i) {
      if (i >= MAXNUM) {
        IOLOG_ERROR("FlagStatus::initialize MAXNUM is too small. Expand it.");
        return false;
      }

      ModifierFlag f = Flags::getModifierFlagByIndex(i);
      item_[i].initialize(f);
      if (f == ModifierFlag::NONE) break;
    }

    return true;
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

  Flags
  FlagStatus::makeFlags(void)
  {
    Flags flags;
    for (int i = 0; item_[i].flag_ != ModifierFlag::NONE; ++i) {
      flags.add(item_[i].makeFlag());
    }
    return flags;
  }

  ModifierFlag
  FlagStatus::getFlag(int index)
  {
    for (int i = 0; item_[i].flag_ != ModifierFlag::NONE; ++i) {
      if (i == index) {
        return item_[i].flag_;
      }
    }
    return ModifierFlag::NONE;
  }

  Flags
  FlagStatus::getLockedFlags(void)
  {
    Flags f(0);
    for (int i = 0; item_[i].flag_ != ModifierFlag::NONE; ++i) {
      if (item_[i].lock_count_) {
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
  void FlagStatus::increase(Flags flags) { FOREACH_TO_FLAGS(increase); }
  void FlagStatus::decrease(Flags flags) { FOREACH_TO_FLAGS(decrease); }
  void FlagStatus::temporary_increase(Flags flags) { FOREACH_TO_FLAGS(temporary_increase); }
  void FlagStatus::temporary_decrease(Flags flags) { FOREACH_TO_FLAGS(temporary_decrease); }
  void FlagStatus::lock_increase(Flags flags) { FOREACH_TO_FLAGS(lock_increase); updateStatusMessage(); }
  void FlagStatus::lock_decrease(Flags flags) { FOREACH_TO_FLAGS(lock_decrease); updateStatusMessage(); }
  void FlagStatus::lock_toggle(Flags flags) { FOREACH_TO_FLAGS(lock_toggle); updateStatusMessage(); }
  void FlagStatus::sticky_increase(Flags flags) { FOREACH_TO_FLAGS(sticky_increase); updateStatusMessage(); }
  void FlagStatus::sticky_decrease(Flags flags) { FOREACH_TO_FLAGS(sticky_decrease); updateStatusMessage(); }
  void FlagStatus::sticky_toggle(Flags flags) { FOREACH_TO_FLAGS(sticky_toggle); updateStatusMessage(); }
#undef FOREACH_TO_FLAGS

  void FlagStatus::sticky_clear(void) {
    for (int i = 0; item_[i].flag_ != ModifierFlag::NONE; ++i) {
      item_[i].sticky_decrease();
    }
  }

  void
  FlagStatus::updateStatusMessage(void)
  {
#ifndef FLAGSTATUS_TEST
    Flags f = FlagStatus::getLockedFlags();
    if (f != statusMessageFlags_) {
      KeyRemap4MacBook_bridge::StatusMessage::Request request(KeyRemap4MacBook_bridge::StatusMessage::MESSAGETYPE_MODIFIER_LOCK, "Lock: ");
      bool isempty = true;

      if (f.isOn(ModifierFlag::FN)) {
        isempty = false;
        strlcat(request.message, "FN ", sizeof(request.message));
      }
      if (f.isOn(ModifierFlag::COMMAND_L) || f.isOn(ModifierFlag::COMMAND_R)) {
        isempty = false;
        strlcat(request.message, "Cmd ", sizeof(request.message));
      }
      if (f.isOn(ModifierFlag::SHIFT_L) || f.isOn(ModifierFlag::SHIFT_R)) {
        isempty = false;
        strlcat(request.message, "Shift ", sizeof(request.message));
      }

      if (isempty) {
        request.message[0] = '\0';
      }
      KeyRemap4MacBook_client::sendmsg(KeyRemap4MacBook_bridge::REQUEST_STATUS_MESSAGE, &request, sizeof(request), NULL, 0);
    }
    statusMessageFlags_ = f;
#endif
  }
}
