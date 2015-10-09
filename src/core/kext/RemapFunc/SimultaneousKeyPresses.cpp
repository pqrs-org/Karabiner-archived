#include "diagnostic_macros.hpp"

BEGIN_IOKIT_INCLUDE;
#include <IOKit/IOLib.h>
END_IOKIT_INCLUDE;

#include "CommonData.hpp"
#include "EventInputQueue.hpp"
#include "IOLogWrapper.hpp"
#include "SimultaneousKeyPresses.hpp"

namespace org_pqrs_Karabiner {
namespace RemapFunc {
List SimultaneousKeyPresses::ActiveFromInfos::list_;

void SimultaneousKeyPresses::add(AddDataType datatype, AddValue newval) {
  switch (datatype) {
  case BRIDGE_DATATYPE_KEYCODE:
  case BRIDGE_DATATYPE_CONSUMERKEYCODE:
  case BRIDGE_DATATYPE_POINTINGBUTTON: {
    // ----------------------------------------
    // The first data is virtualkey_.
    if (index_ == 0) {
      if (datatype != BRIDGE_DATATYPE_KEYCODE) {
        IOLOG_ERROR("Invalid SimultaneousKeyPresses::add\n");
        return;
      }
      virtualkey_ = KeyCode(newval);
      keytokey_.add(virtualkey_);
      goto finish;
    }

    // ----------------------------------------
    // From keys
    if (!isFromInfoFull_) {
      fromInfo_.push_back(FromInfo(datatype, newval));

      if (isUseSeparator_ == false) {
        if (index_ == 2) {
          isFromInfoFull_ = true;
        }
      }
      goto finish;
    }

    // ----------------------------------------
    // To keys
    if (keytokey_.toKeysSize() == 0 &&
        datatype == BRIDGE_DATATYPE_KEYCODE) {
      toKey_raw_ = KeyCode(newval);
    }
    keytokey_.add(datatype, newval);

  finish:
    ++index_;

    break;
  }

  case BRIDGE_DATATYPE_MODIFIERFLAG:
  case BRIDGE_DATATYPE_MODIFIERFLAGS_END: {
    if (index_ < 2) {
      IOLOG_ERROR("Invalid SimultaneousKeyPresses::add\n");
      return;
    } else {
      if (keytokey_.toKeysSize() == 0) {
        fromModifierFlags_.push_back(ModifierFlag(datatype, newval));
      }
      keytokey_.add(datatype, newval);
    }
    break;
  }

  case BRIDGE_DATATYPE_OPTION: {
    Option option(newval);
    if (Option::SIMULTANEOUSKEYPRESSES_RAW == option) {
      isToRaw_ = true;
    } else if (Option::SIMULTANEOUSKEYPRESSES_POST_FROM_EVENTS_AS_RAW == option) {
      isPostFromEventsAsRaw_ = true;
    } else if (Option::SIMULTANEOUSKEYPRESSES_STRICT_KEY_ORDER == option) {
      isStrictKeyOrder_ = true;

    } else if (Option::USE_SEPARATOR == option) {
      isUseSeparator_ = true;

    } else if (Option::SEPARATOR == option) {
      isFromInfoFull_ = true;

    } else if (Option::NOREPEAT == option ||
               Option::KEYTOKEY_BEFORE_KEYDOWN == option ||
               Option::KEYTOKEY_AFTER_KEYUP == option) {
      keytokey_.add(option);

    } else {
      IOLOG_ERROR("SimultaneousKeyPresses::add unknown option:%u\n", static_cast<unsigned int>(newval));
    }
    break;
  }

  case BRIDGE_DATATYPE_DELAYUNTILREPEAT:
  case BRIDGE_DATATYPE_KEYREPEAT: {
    keytokey_.add(datatype, newval);
    break;
  }

  default:
    IOLOG_ERROR("SimultaneousKeyPresses::add invalid datatype:%u\n", static_cast<unsigned int>(datatype));
    break;
  }
}

RemapSimultaneousKeyPressesResult::Value
SimultaneousKeyPresses::remapSimultaneousKeyPresses(void) {
  // We consider "Shift_L+Shift_R to Space".
  // When we press keys by the following order.
  //
  // (1) Shift_L Down
  // (2) Shift_R Down
  // (3) Shift_L Up
  // (4) Shift_R Up
  //
  // First remap():
  //   (1) Shift_L Down -> to virtualkey_
  //   (2) Shift_R Down -> removed
  //   (3) Shift_L Up   -> no change
  //   (4) Shift_R Up   -> no change
  //
  // Second remap():
  //   (3) Shift_L Up   -> removed
  //   (4) Shift_R Up   -> no change
  // However, we need to remove (4) at the same time.
  // If (4) is alive, Shift_R Up event which we don't intend is fired in EventInputQueue.
  // So, we retry handling KeyUp event once more when we drop KeyUp event.

  auto front = static_cast<EventInputQueue::Item*>(EventInputQueue::queue_.safe_front());
  if (!front) {
    return RemapSimultaneousKeyPressesResult::NOT_CHANGED;
  }

  if (!(front->isSimultaneousKeyPressesTarget)) {
    return RemapSimultaneousKeyPressesResult::NOT_CHANGED;
  }

  // backup device information.
  DeviceIdentifier frontDeviceIdentifier(front->deviceIdentifier);
  ListHookedDevice::WeakPointer_Item frontDevice(front->deviceWeakPointer);

  // ------------------------------------------------------------
  // fire KeyUp event if needed.
  for (size_t i = 0; i < fromInfo_.size(); ++i) {
    if (!fromInfo_[i].isActive(frontDevice)) continue;
    if (!fromInfo_[i].fromEvent().isTargetUpEvent(front->getParamsBase())) continue;

    auto activeItem = fromInfo_[i].getActiveFromInfosItem(frontDevice);
    if (!activeItem) continue;

    auto serialNumber = activeItem->getEventInputQueueSerialNumber();
    auto deviceIdentifier = activeItem->getFrontDeviceIdentifier();
    auto device = activeItem->getFrontDevice();

    // --------------------
    if (isPostFromEventsAsRaw_) {
      front->isSimultaneousKeyPressesTarget = false;
    } else {
      EventInputQueue::queue_.pop_front();
    }
    fromInfo_[i].deactivate(frontDevice);
    // activeItem has been invalidated by `deactivate`.
    activeItem = nullptr;

    // --------------------
    // if all keys are released, fire KeyUp event.
    bool isAllDeactived = true;
    for (size_t j = 0; j < fromInfo_.size(); ++j) {
      if (fromInfo_[j].isActiveBySerialNumber(serialNumber)) {
        isAllDeactived = false;
      }
    }
    if (!isAllDeactived) {
      return RemapSimultaneousKeyPressesResult::QUEUE_CHANGED;
    }

    push_remapped(false, deviceIdentifier, device);
    return RemapSimultaneousKeyPressesResult::APPLIED;
  }

  // ------------------------------------------------------------
  // handle KeyDown event.
  if (!FlagStatus::globalFlagStatus().isOn(fromModifierFlags_)) return RemapSimultaneousKeyPressesResult::NOT_CHANGED;

  // Check the first item in queue_ is target.
  //
  // We need to skip when the first item is not target.
  // In this case:
  //   - shift+[a+s] to space
  //   - [a+s] to return
  // When queue_ is [shift, a, s], we need to change these events to space.
  // If we do not check the first item is target,
  // [shift, a, s] will be changed to [shift, return].
  // It's not intended.
  for (size_t i = 0; i < fromInfo_.size(); ++i) {
    if (fromInfo_[i].fromEvent().isTargetDownEvent(front->getParamsBase())) {
      goto scan;
    }
  }
  // skip
  return RemapSimultaneousKeyPressesResult::NOT_CHANGED;

scan:
  // --------------------
  // scan items in queue_.
  while (downKeys_.size() < fromInfo_.size()) {
    downKeys_.push_back(nullptr);
  }
  // Then, downKeys_.size() >= fromInfo_.size()

  for (size_t i = 0; i < fromInfo_.size(); ++i) {
    downKeys_[i].item = nullptr;
  }

  for (;;) {
    // We consider "Shift_L+Shift_R to Space".
    // When we press keys by the following order.
    //
    // (1) Shift_L Down
    // (2) Shift_L Up
    // (3) Shift_R Down
    // (4) Shift_R Up
    //
    // If fromKey was released before all keys are pressed,
    // we must not handle these keys as SimultaneousKeyPresses.
    //
    for (size_t i = 0; i < fromInfo_.size(); ++i) {
      if (fromInfo_[i].fromEvent().isTargetDownEvent(front->getParamsBase())) {
        downKeys_[i].item = front;
        break;
      } else if (fromInfo_[i].fromEvent().isTargetUpEvent(front->getParamsBase())) {
        return RemapSimultaneousKeyPressesResult::NOT_CHANGED;
      }
    }

    // ----------------------------------------
    bool isAllKeysDown = true;
    for (size_t i = 0; i < fromInfo_.size(); ++i) {
      if (!downKeys_[i].item) {
        isAllKeysDown = false;
      } else {
        // Checking strict key order.
        //
        // If isStrictKeyOrder_ == true,
        // we must not handle the following state as SimultaneousKeyPresses.
        //
        // - downKeys_[0] == nullptr
        // - downKeys_[1] != nullptr
        //
        if (!isAllKeysDown && isStrictKeyOrder_) {
          return RemapSimultaneousKeyPressesResult::NOT_CHANGED;
        }
      }
    }

    if (isAllKeysDown) {
      // We use the reverse iterator for isPostFromEventsAsRaw_.
      for (int i = static_cast<int>(fromInfo_.size()) - 1; i >= 0; --i) {
        if (!downKeys_[i].item) continue;

        fromInfo_[i].activate((downKeys_[i].item)->deviceWeakPointer,
                              frontDeviceIdentifier,
                              frontDevice,
                              EventInputQueue::currentSerialNumber());

        if (isPostFromEventsAsRaw_) {
          bool retainFlagStatusTemporaryCount = false;
          bool push_back = false;
          bool isSimultaneousKeyPressesTarget = false;
          EventInputQueue::enqueue_((downKeys_[i].item)->getParamsBase(),
                                    retainFlagStatusTemporaryCount,
                                    frontDeviceIdentifier,
                                    frontDevice,
                                    push_back,
                                    isSimultaneousKeyPressesTarget);
        }
        EventInputQueue::queue_.erase_and_delete(downKeys_[i].item);
      }
      push_remapped(true, frontDeviceIdentifier, frontDevice);
      return RemapSimultaneousKeyPressesResult::APPLIED;
    }

    // ----------------------------------------
    // get next target item.
    for (;;) {
      front = static_cast<EventInputQueue::Item*>(front->getnext());
      if (!front) {
        return RemapSimultaneousKeyPressesResult::NOT_CHANGED;
      }
      if (front->isSimultaneousKeyPressesTarget) {
        break;
      }
    }
  }

  return RemapSimultaneousKeyPressesResult::NOT_CHANGED;
}

void SimultaneousKeyPresses::push_remapped(bool isKeyDown, const DeviceIdentifier& deviceIdentifier, const ListHookedDevice::WeakPointer_Item& device) {
  EventType eventType = isKeyDown ? EventType::DOWN : EventType::UP;

  KeyCode key = virtualkey_;
  if (isToRaw_) {
    key = toKey_raw_;
  }
  if (key == KeyCode::VK_NONE) return;

  Flags flags(0);
  if (isKeyDown) {
    flags.add(key.getModifierFlag());
  }

  // ----------------------------------------
  Params_KeyboardEventCallBack params(eventType, flags, key, CommonData::getcurrent_keyboardType(), false);

  bool retainFlagStatusTemporaryCount = false;
  bool push_back = false;
  bool isSimultaneousKeyPressesTarget = false;
  EventInputQueue::enqueue_(params, retainFlagStatusTemporaryCount, deviceIdentifier, device, push_back, isSimultaneousKeyPressesTarget);
}

bool SimultaneousKeyPresses::remap(RemapParams& remapParams) {
  return keytokey_.remap(remapParams);
}
}
}
