#include "FlagStatus.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  FlagStatus::Item FlagStatus::item_[FlagStatus::MAXNUM];

  void
  FlagStatus::Item::initialize(ModifierFlag f)
  {
    flag_ = f;
    count_ = 0;
    temporary_count_ = 0;
    lock_count_ = 0;
    original_lock_count_ = 0;
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
    // At some keyboard, when we press CapsLock key, the down & up event are thrown at a time.
    // So, we treat the capslock key exceptionally.
    if (flag_ == ModifierFlag::CAPSLOCK) {
      if (flags.isOn(flag_)) {
        if (! original_lock_count_) {
          original_lock_count_ = 1;
          lock_count_ = 0; // clear remapped lock_count_ when original changed.
        }
      } else {
        if (original_lock_count_) {
          original_lock_count_ = 0;
          lock_count_ = 0; // clear remapped lock_count_ when original changed.
        }
      }

      return;
    }

    // ------------------------------------------------------------
    if (key != flag_.getKeyCode()) return;

    if (flags.isOn(flag_)) {
      increase();
    } else {
      decrease();
    }
  }

  void
  FlagStatus::Item::reset(void)
  {
    count_ = 0;
    temporary_count_ = 0;

    /*
       preserve lock_count, original_lock_count.

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
#define PUSH_ITEM(FLAG) {           \
    if (i >= MAXNUM) return false;  \
    item_[i].initialize(FLAG);      \
    i++;                            \
}

    int i = 0;
    PUSH_ITEM(ModifierFlag::CAPSLOCK);
    PUSH_ITEM(ModifierFlag::SHIFT_L);
    PUSH_ITEM(ModifierFlag::SHIFT_R);
    PUSH_ITEM(ModifierFlag::CONTROL_L);
    PUSH_ITEM(ModifierFlag::CONTROL_R);
    PUSH_ITEM(ModifierFlag::OPTION_L);
    PUSH_ITEM(ModifierFlag::OPTION_R);
    PUSH_ITEM(ModifierFlag::COMMAND_L);
    PUSH_ITEM(ModifierFlag::COMMAND_R);
    PUSH_ITEM(ModifierFlag::FN);
    PUSH_ITEM(ModifierFlag::NONE);

#undef PUSH_ITEM

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
  void FlagStatus::lock_increase(Flags flags) { FOREACH_TO_FLAGS(lock_increase); }
  void FlagStatus::lock_decrease(Flags flags) { FOREACH_TO_FLAGS(lock_decrease); }
#undef FOREACH_TO_FLAGS
}
