#include "FlagStatus.hpp"
#include "RemapUtil.hpp"
#include "Config.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  FlagStatus::Item FlagStatus::item_[ModifierFlag::listsize];

  void
  FlagStatus::Item::initialize(ModifierFlag::ModifierFlag f)
  {
    flag_ = f;
    key_ = RemapUtil::getModifierKeyCode(flag_);
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
  FlagStatus::Item::set(const RemapParams& remapParams)
  {
    temporary_count_ = 0;

    // At some keyboard, when we press CapsLock key, the down & up event are thrown at a time.
    // So, we treat the capslock key exceptionally.
    if (key_ == KeyCode::CAPSLOCK) {
      if (RemapUtil::isModifierOn(remapParams, flag_)) {
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

      if (config.debug_devel) {
        printf("KeyRemap4MacBook -Info- FlagStatus::set CAPSLOCK (original_lock_count_:%d, lock_count_:%d)\n", original_lock_count_, lock_count_);
      }
      return;
    }

    if ((remapParams.params)->eventType != KeyEvent::MODIFY) return;
    if (remapParams.ex_origKey != static_cast<unsigned int>(key_)) return;

    if (RemapUtil::isModifierOn(remapParams, flag_)) {
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
    if (key_ == KeyCode::CAPSLOCK) {
      lock_count_ = ! lock_count_;
    } else {
      ++count_;
    }
  }

  void
  FlagStatus::Item::decrease(void)
  {
    if (key_ == KeyCode::CAPSLOCK) {
      // do nothing (toggle at Item::increase).
    } else {
      --count_;
    }
  }

  // ----------------------------------------------------------------------
  void
  FlagStatus::initialize(void)
  {
    for (int i = 0; i < ModifierFlag::listsize; ++i) {
      item_[i].initialize(ModifierFlag::list[i]);
    }
  }

  void
  FlagStatus::set(void)
  {
    for (int i = 0; i < ModifierFlag::listsize; ++i) {
      item_[i].set();
    }
  }

  void
  FlagStatus::set(const RemapParams& remapParams)
  {
    for (int i = 0; i < ModifierFlag::listsize; ++i) {
      item_[i].set(remapParams);
    }
  }

  void
  FlagStatus::reset(void)
  {
    if (config.debug_devel) {
      printf("KeyRemap4MacBook FlagStatus::reset\n");
    }

    for (int i = 0; i < ModifierFlag::listsize; ++i) {
      item_[i].reset();
    }
  }

  unsigned int
  FlagStatus::makeFlags(unsigned int keyCode)
  {
    unsigned int flags = 0;
    for (int i = 0; i < ModifierFlag::listsize; ++i) {
      flags |= item_[i].makeFlag();
    }
    return flags;
  }

  unsigned int
  FlagStatus::makeFlags(const RemapParams& remapParams)
  {
    return makeFlags((remapParams.params)->key);
  }

  FlagStatus::Item*
  FlagStatus::getFlagStatus(ModifierFlag::ModifierFlag flag) {
    for (int i = 0; i < ModifierFlag::listsize; ++i) {
      if (flag == ModifierFlag::list[i]) return item_ + i;
    }
    return NULL;
  }

  // ----------------------------------------
  bool
  FlagStatus::isHeldDown(ModifierFlag::ModifierFlag flag) {
    Item* p = getFlagStatus(flag);
    if (! p) return false;
    return p->isHeldDown();
  }

  void
  FlagStatus::increase(ModifierFlag::ModifierFlag flag) {
    Item* p = getFlagStatus(flag);
    if (! p) return;
    return p->increase();
  }

  void
  FlagStatus::decrease(ModifierFlag::ModifierFlag flag) {
    Item* p = getFlagStatus(flag);
    if (! p) return;
    return p->decrease();
  }

  void
  FlagStatus::temporary_increase(ModifierFlag::ModifierFlag flag) {
    Item* p = getFlagStatus(flag);
    if (! p) return;
    return p->temporary_increase();
  }

  void
  FlagStatus::temporary_decrease(ModifierFlag::ModifierFlag flag) {
    Item* p = getFlagStatus(flag);
    if (! p) return;
    return p->temporary_decrease();
  }

  void
  FlagStatus::lock_increase(ModifierFlag::ModifierFlag flag) {
    Item* p = getFlagStatus(flag);
    if (! p) return;
    return p->lock_increase();
  }

  void
  FlagStatus::lock_decrease(ModifierFlag::ModifierFlag flag) {
    Item* p = getFlagStatus(flag);
    if (! p) return;
    return p->lock_decrease();
  }
}
