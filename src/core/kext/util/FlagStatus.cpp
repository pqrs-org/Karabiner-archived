#include "FlagStatus.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  FlagStatus::Item FlagStatus::item_[ModifierFlagList::listsize];

  void
  FlagStatus::Item::initialize(const ModifierFlag& f)
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
  FlagStatus::Item::set(const KeyCode& key, const Flags& flags)
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
  void
  FlagStatus::initialize(void)
  {
    for (int i = 0; i < ModifierFlagList::listsize; ++i) {
      item_[i].initialize(ModifierFlagList::list[i]);
    }
  }

  void
  FlagStatus::set(void)
  {
    for (int i = 0; i < ModifierFlagList::listsize; ++i) {
      item_[i].set();
    }
  }

  void
  FlagStatus::set(const KeyCode& key, const Flags& flags)
  {
    for (int i = 0; i < ModifierFlagList::listsize; ++i) {
      item_[i].set(key, flags);
    }
  }

  void
  FlagStatus::reset(void)
  {
    for (int i = 0; i < ModifierFlagList::listsize; ++i) {
      item_[i].reset();
    }
  }

  Flags
  FlagStatus::makeFlags(void)
  {
    Flags flags;
    for (int i = 0; i < ModifierFlagList::listsize; ++i) {
      flags.add(item_[i].makeFlag());
    }
    return flags;
  }

  // ------------------------------------------------------------
#define FOREACH_TO_FLAGS(METHOD) {                          \
    for (int i = 0; i < ModifierFlagList::listsize; ++i) {      \
      if (flags.isOn(ModifierFlagList::list[i])) {              \
        item_[i].METHOD();                                  \
      }                                                     \
    }                                                       \
  }
  void FlagStatus::increase(const Flags& flags) { FOREACH_TO_FLAGS(increase); }
  void FlagStatus::decrease(const Flags& flags) { FOREACH_TO_FLAGS(decrease); }
  void FlagStatus::temporary_increase(const Flags& flags) { FOREACH_TO_FLAGS(temporary_increase); }
  void FlagStatus::temporary_decrease(const Flags& flags) { FOREACH_TO_FLAGS(temporary_decrease); }
  void FlagStatus::lock_increase(const Flags& flags) { FOREACH_TO_FLAGS(lock_increase); }
  void FlagStatus::lock_decrease(const Flags& flags) { FOREACH_TO_FLAGS(lock_decrease); }
#undef FOREACH_TO_FLAGS
}
