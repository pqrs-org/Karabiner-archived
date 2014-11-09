#include "CommonData.hpp"
#include "EventInputQueue.hpp"
#include "EventOutputQueue.hpp"
#include "IOLogWrapper.hpp"
#include "KeyToKey.hpp"
#include "KeyboardRepeat.hpp"
#include "VirtualKey.hpp"

namespace org_pqrs_Karabiner {
namespace RemapFunc {
void
KeyToKey::add(AddDataType datatype, AddValue newval) {
  switch (datatype) {
  case BRIDGE_DATATYPE_KEYCODE:
  case BRIDGE_DATATYPE_CONSUMERKEYCODE:
  case BRIDGE_DATATYPE_POINTINGBUTTON: {
    switch (index_) {
    case 0:
      fromEvent_ = FromEvent(datatype, newval);
      break;
    default:
      getCurrentToEvent().push_back(ToEvent(datatype, newval));
      break;
    }
    ++index_;

    break;
  }

  case BRIDGE_DATATYPE_MODIFIERFLAG:
  case BRIDGE_DATATYPE_MODIFIERFLAGS_END: {
    switch (index_) {
    case 0:
      IOLOG_ERROR("Invalid KeyToKey::add\n");
      break;
    case 1: {
      ModifierFlag modifierFlag(datatype, newval);
      fromModifierFlags_.push_back(modifierFlag);
      if (fromEvent_.getModifierFlag() != modifierFlag) {
        pureFromModifierFlags_.push_back(modifierFlag);
      }
      break;
    }
    default:
      if (!getCurrentToEvent().empty()) {
        getCurrentToEvent().back().addModifierFlag(datatype, newval);
      }
      break;
    }
    break;
  }

  case BRIDGE_DATATYPE_OPTION: {
    Option option(newval);
    if (Option::NOREPEAT == option) {
      isRepeatEnabled_ = false;
    } else if (Option::KEYTOKEY_BEFORE_KEYDOWN == option) {
      currentToEvent_ = CurrentToEvent::BEFOREKEYS;
    } else if (Option::KEYTOKEY_AFTER_KEYUP == option) {
      currentToEvent_ = CurrentToEvent::AFTERKEYS;
    } else if (Option::USE_SEPARATOR == option ||
               Option::SEPARATOR == option) {
      // do nothing
    } else {
      IOLOG_ERROR("KeyToKey::add unknown option:%u\n", static_cast<unsigned int>(newval));
    }
    break;
  }

  case BRIDGE_DATATYPE_DELAYUNTILREPEAT: {
    // Use 1ms if newval == 0 because Terminal.app cannot treat too rapid key repeat events.)
    delayUntilRepeat_ = max(newval, 1);
    break;
  }

  case BRIDGE_DATATYPE_KEYREPEAT: {
    // Use 1ms if newval == 0 because Terminal.app cannot treat too rapid key repeat events.)
    keyRepeat_ = max(newval, 1);
    break;
  }

  default:
    IOLOG_ERROR("KeyToKey::add invalid datatype:%u\n", static_cast<unsigned int>(datatype));
    break;
  }
}

void
KeyToKey::clearToKeys(void) {
  if (index_ > 1) {
    index_ = 1;
  }

  toKeys_.clear();
  beforeKeys_.clear();
  afterKeys_.clear();

  currentToEvent_ = CurrentToEvent::TOKEYS;
}

bool
KeyToKey::remap(RemapParams& remapParams) {
  if (remapParams.isremapped) return false;
  if (!fromEvent_.changePressingState(remapParams.paramsBase,
                                      FlagStatus::globalFlagStatus(),
                                      fromModifierFlags_)) return false;
  remapParams.isremapped = true;

  // ------------------------------------------------------------
  // handle EventType & Modifiers

  // Let's consider the following setting.
  //   __KeyToKey__ KeyCode::SHIFT_R, ModifierFlag::SHIFT_R | ModifierFlag::NONE, KeyCode::A, ModifierFlag::SHIFT_R
  // In this setting, we need decrease SHIFT_R only once.
  // So, we use pureFromModifierFlags_.
  //
  // [before]
  //   fromEvent_         : KeyCode::SHIFT_R
  //   fromModifierFlags_ : ModifierFlag::SHIFT_R | ModifierFlag::NONE
  //
  // [after]
  //   fromEvent_             : KeyCode::SHIFT_R
  //   pureFromModifierFlags_ : ModifierFlag::NONE
  //
  // Note: we need to use pureFromModifierFlags_ after calling fromEvent_.changePressingState.

  if (fromEvent_.isPressing()) {
    FlagStatus::globalFlagStatus().decrease(fromEvent_.getModifierFlag());
    ButtonStatus::decrease(fromEvent_.getPointingButton());
  } else {
    FlagStatus::globalFlagStatus().increase(fromEvent_.getModifierFlag());
    ButtonStatus::increase(fromEvent_.getPointingButton());
  }

  // ----------------------------------------
  // Handle beforeKeys_
  if (fromEvent_.isPressing()) {
    FlagStatus::globalFlagStatus().temporary_decrease(pureFromModifierFlags_);

    for (size_t i = 0; i < beforeKeys_.size(); ++i) {
      FlagStatus::globalFlagStatus().temporary_increase(beforeKeys_[i].getModifierFlags());

      beforeKeys_[i].fire_downup();

      FlagStatus::globalFlagStatus().temporary_decrease(beforeKeys_[i].getModifierFlags());
    }

    FlagStatus::globalFlagStatus().temporary_increase(pureFromModifierFlags_);
  }

  // ----------------------------------------
  // Handle toKeys_
  bool add_to_keyrepeat = true;
  if (fromEvent_.isPressing() && !isRepeatEnabled_) {
    add_to_keyrepeat = false;
  }

  switch (toKeys_.size()) {
  case 0:
    break;

  case 1: {
    EventType newEventType = fromEvent_.isPressing() ? EventType::DOWN : EventType::UP;
    ModifierFlag toModifierFlag = toKeys_[0].getModifierFlag();

    if (toModifierFlag == ModifierFlag::ZERO && !toKeys_[0].isEventLikeModifier()) {
      // toKey

      // Consider these rules:
      //
      //   * Change Shift-P to Control-P
      //   * Change Shift-N to Control-N
      //
      // Case 1:
      //   Actual input:
      //     1. shift down
      //     2. p down
      //     3. p up
      //     4. n down
      //     5. n up
      //     6. shift up
      //
      //   Desirable results:
      //     1. shift down
      //     2. shift up, control down, p down
      //     3. p up
      //     4. n down
      //     5. n up
      //     6. control up
      //
      //   *** ModifierFlag manipulation when key up ***
      //   In this case, in step 3 and 5, we should increase control and decrease shift.
      //   (== temporary_decrease shift and temporary_increase control in 2,3,4,5.)
      //
      //
      // Case 2:
      //   Actual input:
      //     1. shift down
      //     2. p down
      //     3. command down
      //     4. p up
      //     5. shift up
      //     6. command up
      //
      //   Desirable results:
      //     1. shift down
      //     2. shift up, control down, p down
      //     3. control up, command down, shift down (== shift-command)
      //     4. p up
      //     5. shift up
      //     6. command up
      //
      //   *** ModifierFlag manipulation when key up ***
      //   In this case, in step 4, we should not increase control and not decrease shift.
      //   (== Do not temporary_decrease shift and temporary_increase control in 4.)
      //
      //
      // Case 3:
      //   Actual input:
      //     1. shift down
      //     2. p down
      //     3. shift up
      //     4. p up
      //
      //   Desirable results:
      //     1. shift down
      //     2. shift up, control down, p down
      //     3. control up
      //     4. p up
      //
      //   *** ModifierFlag manipulation when key up ***
      //   In this case, in step 4, we should not increase control and not decrease shift.
      //   (== Do not temporary_decrease shift and temporary_increase control in 4.)
      //
      //
      // *** Summary of ModifierFlag manipulation ***
      //
      //   If the last sent modifierflag contains toflags, we should keep flags.
      //   (temporary_decrease pureFromModifierFlags_ and temporary_increase toflags)
      //
      //   Otherwise, we should not manipulate modifiers.

      bool needToManipulateModifiers = false;

      if (fromEvent_.isPressing()) {
        needToManipulateModifiers = true;
      } else {
        Flags toFlags(toKeys_[0].getModifierFlags());
        if ((EventOutputQueue::FireModifiers::getLastFlags() & toFlags) == toFlags) {
          needToManipulateModifiers = true;
        }
      }

      if (needToManipulateModifiers) {
        FlagStatus::globalFlagStatus().temporary_decrease(pureFromModifierFlags_);
        FlagStatus::globalFlagStatus().temporary_increase(toKeys_[0].getModifierFlags());
      }

    } else {
      // toModifier or VirtualKey::isKeyLikeModifier
      if (toModifierFlag != ModifierFlag::ZERO) {
        newEventType = EventType::MODIFY;
      }

      if (fromEvent_.isPressing()) {
        FlagStatus::globalFlagStatus().increase(toModifierFlag, toKeys_[0].getModifierFlags());
        FlagStatus::globalFlagStatus().decrease(pureFromModifierFlags_);
      } else {
        FlagStatus::globalFlagStatus().decrease(toModifierFlag, toKeys_[0].getModifierFlags());
        FlagStatus::globalFlagStatus().increase(pureFromModifierFlags_);
      }
    }

    toKeys_[0].fire(newEventType, FlagStatus::globalFlagStatus().makeFlags(),
                    add_to_keyrepeat, getDelayUntilRepeat(), getKeyRepeat());

    if (!add_to_keyrepeat) {
      KeyboardRepeat::cancel();
    }

    break;
  }

  default:
    ToEvent& lastToEvent = toKeys_[toKeys_.size() - 1];
    ModifierFlag lastToEventModifierFlag = lastToEvent.getModifierFlag();
    bool isLastToEventModifierKey = (lastToEventModifierFlag != ModifierFlag::ZERO);
    bool isLastToEventLikeModifier = lastToEvent.isEventLikeModifier();

    if (fromEvent_.isPressing()) {
      KeyboardRepeat::cancel();

      FlagStatus::globalFlagStatus().temporary_decrease(pureFromModifierFlags_);

      size_t size = toKeys_.size();
      // If the last key is modifier, we give it special treatment.
      // - Don't fire key repeat.
      // - Synchronous the key press status and the last modifier status.
      if (isLastToEventModifierKey || isLastToEventLikeModifier) {
        --size;
      }

      for (size_t i = 0; i < size; ++i) {
        FlagStatus::globalFlagStatus().temporary_increase(toKeys_[i].getModifierFlags());

        toKeys_[i].fire_downup(true);

        FlagStatus::globalFlagStatus().temporary_decrease(toKeys_[i].getModifierFlags());
      }

      if (isLastToEventModifierKey || isLastToEventLikeModifier) {
        // restore temporary flag.
        FlagStatus::globalFlagStatus().temporary_increase(pureFromModifierFlags_);

        FlagStatus::globalFlagStatus().increase(lastToEventModifierFlag, lastToEvent.getModifierFlags());
        FlagStatus::globalFlagStatus().decrease(pureFromModifierFlags_);

        if (isLastToEventLikeModifier) {
          // Don't call EventOutputQueue::FireModifiers::fire here.
          //
          // Intentionally VK_LAZY_* stop sending MODIFY events.
          // EventOutputQueue::FireModifiers::fire destroys this behavior.
          lastToEvent.fire(EventType::DOWN, FlagStatus::globalFlagStatus().makeFlags(), false);

        } else {
          EventOutputQueue::FireModifiers::fire();
        }
      }

      if (isLastToEventModifierKey || isLastToEventLikeModifier) {
        KeyboardRepeat::cancel();
      } else {
        if (isRepeatEnabled_) {
          keyboardRepeatID_ = KeyboardRepeat::primitive_start(getDelayUntilRepeat(), getKeyRepeat());
        } else {
          keyboardRepeatID_ = -1;
        }
      }

    } else {
      if (isLastToEventModifierKey || isLastToEventLikeModifier) {
        // For Lazy-Modifiers (KeyCode::VK_LAZY_*),
        // we need to handle these keys before restoring pureFromModifierFlags_, lastKeyFlags and lastKeyModifierFlag.
        // The unnecessary modifier events occur unless we do it.
        if (isLastToEventLikeModifier) {
          lastToEvent.fire(EventType::UP, FlagStatus::globalFlagStatus().makeFlags(), false);
        }

        FlagStatus::globalFlagStatus().decrease(lastToEventModifierFlag, lastToEvent.getModifierFlags());
        FlagStatus::globalFlagStatus().increase(pureFromModifierFlags_);
        EventOutputQueue::FireModifiers::fire();

      } else {
        if (KeyboardRepeat::getID() == keyboardRepeatID_) {
          KeyboardRepeat::cancel();
        }
      }
    }
    break;
  }

  // ----------------------------------------
  // Handle afterKeys_
  if (!fromEvent_.isPressing()) {
    // We need to keep temporary flags for "general.lazy_modifiers_with_mouse_event" when afterKeys_ is empty.
    if (afterKeys_.size() > 0) {
      // clear temporary flags.
      FlagStatus::globalFlagStatus().globalFlagStatus().set();

      FlagStatus::globalFlagStatus().temporary_decrease(pureFromModifierFlags_);

      for (size_t i = 0; i < afterKeys_.size(); ++i) {
        FlagStatus::globalFlagStatus().temporary_increase(afterKeys_[i].getModifierFlags());

        afterKeys_[i].fire_downup();

        FlagStatus::globalFlagStatus().temporary_decrease(afterKeys_[i].getModifierFlags());
      }

      FlagStatus::globalFlagStatus().temporary_increase(pureFromModifierFlags_);
    }
  }

  return true;
}

bool
KeyToKey::call_remap_with_VK_PSEUDO_KEY(EventType eventType) {
  bool result = false;

  // ----------------------------------------
  if (eventType == EventType::DOWN) {
    FlagStatus::globalFlagStatus().lazy_enable();
  } else {
    FlagStatus::globalFlagStatus().lazy_disable_if_off();
  }

  // ----------------------------------------
  Params_KeyboardEventCallBack params(eventType,
                                      FlagStatus::globalFlagStatus().makeFlags(),
                                      KeyCode::VK_PSEUDO_KEY,
                                      CommonData::getcurrent_keyboardType(),
                                      false);
  RemapParams rp(params);
  result = remap(rp);

  return result;
}

int
KeyToKey::getDelayUntilRepeat(void) {
  if (delayUntilRepeat_ >= 0) {
    return delayUntilRepeat_;
  } else {
    // If all ToEvent is consumer, use repeat.consumer_initial_wait.
    for (size_t i = 0; i < toKeys_.size(); ++i) {
      if (toKeys_[i].getType() != ToEvent::Type::CONSUMER_KEY) {
        return Config::get_repeat_initial_wait();
      }
    }

    return Config::get_repeat_consumer_initial_wait();
  }
}

int
KeyToKey::getKeyRepeat(void) {
  if (keyRepeat_ >= 0) {
    return keyRepeat_;
  } else {
    // If all ToEvent is consumer, use repeat.consumer_wait.
    for (size_t i = 0; i < toKeys_.size(); ++i) {
      if (toKeys_[i].getType() != ToEvent::Type::CONSUMER_KEY) {
        return Config::get_repeat_wait();
      }
    }

    return Config::get_repeat_wait();
  }
}
}
}
