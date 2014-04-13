#include "CommonData.hpp"
#include "IOLogWrapper.hpp"
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
  void
  FlagStatus::initialize(void)
  {
    for (int i = 0;; ++i) {
      ModifierFlag f = Flags::getModifierFlagByIndex(i);
      if (f == ModifierFlag::NONE) break;

      item_.push_back(Item());
      item_.back().initialize(f);
    }
  }

  FlagStatus::FlagStatus(void)
  {
    initialize();
  }

  FlagStatus::FlagStatus(Flags flags)
  {
    initialize();

    for (size_t i = 0; i < item_.size(); ++i) {
      if (flags.isOn(item_[i].flag_)) {
        item_[i].increase();
      }
    }
  }

  void
  FlagStatus::set(void)
  {
    for (size_t i = 0; i < item_.size(); ++i) {
      item_[i].set();
    }
  }

  void
  FlagStatus::set(KeyCode key, Flags flags)
  {
    for (size_t i = 0; i < item_.size(); ++i) {
      item_[i].set(key, flags);
    }
  }

  void
  FlagStatus::reset(void)
  {
    for (size_t i = 0; i < item_.size(); ++i) {
      item_[i].reset();
    }
  }

  bool
  FlagStatus::isOn(ModifierFlag modifierFlag) const
  {
    for (size_t i = 0; i < item_.size(); ++i) {
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
      if (modifierFlags[i] == ModifierFlag::ZERO) continue;

      if (modifierFlags[i] == ModifierFlag::NONE) {
        strict = true;
      } else {
        if (! isOn(modifierFlags[i])) return false;
      }
    }

    // If modifierFlags contains ModifierFlag::NONE,
    // return false when unspecified modifierflag is pressed.
    if (strict) {
      for (size_t i = 0; i < item_.size(); ++i) {
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
    for (size_t i = 0; i < item_.size(); ++i) {
      if (item_[i].sum() > 0) {
        flags.add(item_[i].flag_);
      }
    }
    return flags;
  }

  ModifierFlag
  FlagStatus::getFlag(size_t index) const
  {
    for (size_t i = 0; i < item_.size(); ++i) {
      if (i == index) {
        return item_[i].flag_;
      }
    }
    return ModifierFlag::ZERO;
  }

  // ------------------------------------------------------------
#define DEFINE_METHODS(METHOD)                                                                   \
  void FlagStatus::METHOD(ModifierFlag modifierFlag) {                                           \
    for (size_t i = 0; i < item_.size(); ++i) {                                                  \
      if (modifierFlag == item_[i].flag_) {                                                      \
        item_[i].METHOD();                                                                       \
      }                                                                                          \
    }                                                                                            \
  }                                                                                              \
  void FlagStatus::METHOD(ModifierFlag modifierFlag, const Vector_ModifierFlag &modifierFlags) { \
    for (size_t i = 0; i < item_.size(); ++i) {                                                  \
      if (modifierFlag == item_[i].flag_ ||                                                      \
          modifierFlags.is_include(item_[i].flag_)) {                                            \
        item_[i].METHOD();                                                                       \
      }                                                                                          \
    }                                                                                            \
  }                                                                                              \
  void FlagStatus::METHOD(const Vector_ModifierFlag &modifierFlags) {                            \
    for (size_t i = 0; i < item_.size(); ++i) {                                                  \
      if (modifierFlags.is_include(item_[i].flag_)) {                                            \
        item_[i].METHOD();                                                                       \
      }                                                                                          \
    }                                                                                            \
  }                                                                                              \

  DEFINE_METHODS(increase)
  DEFINE_METHODS(decrease)
  DEFINE_METHODS(temporary_increase)
  DEFINE_METHODS(temporary_decrease)
#undef DEFINE_METHODS

#define DEFINE_METHODS(METHOD)                                                                   \
  void FlagStatus::METHOD(ModifierFlag modifierFlag) {                                           \
    for (size_t i = 0; i < item_.size(); ++i) {                                                  \
      if (modifierFlag == item_[i].flag_) {                                                      \
        item_[i].METHOD();                                                                       \
        updateStatusMessage();                                                                   \
      }                                                                                          \
    }                                                                                            \
  }                                                                                              \
  void FlagStatus::METHOD(ModifierFlag modifierFlag, const Vector_ModifierFlag &modifierFlags) { \
    for (size_t i = 0; i < item_.size(); ++i) {                                                  \
      if (modifierFlag == item_[i].flag_ ||                                                      \
          modifierFlags.is_include(item_[i].flag_)) {                                            \
        item_[i].METHOD();                                                                       \
        updateStatusMessage();                                                                   \
      }                                                                                          \
    }                                                                                            \
  }                                                                                              \
  void FlagStatus::METHOD(const Vector_ModifierFlag &modifierFlags) {                            \
    for (size_t i = 0; i < item_.size(); ++i) {                                                  \
      if (modifierFlags.is_include(item_[i].flag_)) {                                            \
        item_[i].METHOD();                                                                       \
        updateStatusMessage();                                                                   \
      }                                                                                          \
    }                                                                                            \
  }                                                                                              \

  DEFINE_METHODS(lock_increase)
  DEFINE_METHODS(lock_decrease)
  DEFINE_METHODS(lock_toggle)
  DEFINE_METHODS(sticky_increase)
  DEFINE_METHODS(sticky_decrease)
  DEFINE_METHODS(sticky_toggle)
#undef DEFINE_METHODS

  void
  FlagStatus::sticky_clear(void)
  {
    for (size_t i = 0; i < item_.size(); ++i) {
      item_[i].sticky_decrease();
    }
    updateStatusMessage();
  }

  void
  FlagStatus::lock_clear(void)
  {
    for (size_t i = 0; i < item_.size(); ++i) {
      if (item_[i].lock_count_) {
        item_[i].lock_decrease();
      }
    }
    updateStatusMessage();
  }

  void
  FlagStatus::updateStatusMessage(unsigned int statusMessageIndex)
  {
    CommonData::clear_statusmessage(statusMessageIndex);

    for (size_t i = 0; i < item_.size(); ++i) {
      if (statusMessageIndex == BRIDGE_USERCLIENT_STATUS_MESSAGE_MODIFIER_LOCK) {
        if (item_[i].lock_count_ <= 0) {
          continue;
        }
      }
      if (statusMessageIndex == BRIDGE_USERCLIENT_STATUS_MESSAGE_MODIFIER_STICKY) {
        if (item_[i].sticky_count_ <= 0) {
          continue;
        }
      }

      if (item_[i].flag_ == ModifierFlag::FN) {
        CommonData::append_statusmessage(statusMessageIndex, "FN ");
      }
      if (item_[i].flag_ == ModifierFlag::COMMAND_L || item_[i].flag_ == ModifierFlag::COMMAND_R) {
        CommonData::append_statusmessage(statusMessageIndex, "Cmd ");
      }
      if (item_[i].flag_ == ModifierFlag::CONTROL_L || item_[i].flag_ == ModifierFlag::CONTROL_R) {
        CommonData::append_statusmessage(statusMessageIndex, "Ctrl ");
      }
      if (item_[i].flag_ == ModifierFlag::OPTION_L || item_[i].flag_ == ModifierFlag::OPTION_R) {
        CommonData::append_statusmessage(statusMessageIndex, "Opt ");
      }
      if (item_[i].flag_ == ModifierFlag::SHIFT_L || item_[i].flag_ == ModifierFlag::SHIFT_R) {
        CommonData::append_statusmessage(statusMessageIndex, "Shift ");
      }
      if (item_[i].flag_ == ModifierFlag::EXTRA1) {
        CommonData::append_statusmessage(statusMessageIndex, "Ex1 ");
      }
      if (item_[i].flag_ == ModifierFlag::EXTRA2) {
        CommonData::append_statusmessage(statusMessageIndex, "Ex2 ");
      }
      if (item_[i].flag_ == ModifierFlag::EXTRA3) {
        CommonData::append_statusmessage(statusMessageIndex, "Ex3 ");
      }
      if (item_[i].flag_ == ModifierFlag::EXTRA4) {
        CommonData::append_statusmessage(statusMessageIndex, "Ex4 ");
      }
      if (item_[i].flag_ == ModifierFlag::EXTRA5) {
        CommonData::append_statusmessage(statusMessageIndex, "Ex5 ");
      }
    }
  }

  void
  FlagStatus::updateStatusMessage(void)
  {
    int indexes[] = {
      BRIDGE_USERCLIENT_STATUS_MESSAGE_MODIFIER_LOCK,
      BRIDGE_USERCLIENT_STATUS_MESSAGE_MODIFIER_STICKY,
    };
    for (size_t i = 0; i < sizeof(indexes) / sizeof(indexes[0]); ++i) {
      int idx = indexes[i];

      static char previousMessage[BRIDGE_USERCLIENT_STATUS_MESSAGE_MAXLEN];
      strlcat(previousMessage, CommonData::get_statusmessage(idx), sizeof(previousMessage));

      updateStatusMessage(idx);

      if (strcmp(CommonData::get_statusmessage(idx), previousMessage) != 0) {
        CommonData::send_notification_statusmessage(idx);
      }
    }
  }
}
