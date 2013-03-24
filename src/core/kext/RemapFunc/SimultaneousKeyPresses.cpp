#include "CommonData.hpp"
#include "EventInputQueue.hpp"
#include "SimultaneousKeyPresses.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace RemapFunc {
    SimultaneousKeyPresses::SimultaneousKeyPresses(void) :
      isUseSeparator_(false),
      index_(0),
      isFromInfoFull_(false),
      toType_(TOTYPE_NONE),
      isToRaw_(false),
      isStrictKeyOrder_(false)
    {}

    SimultaneousKeyPresses::~SimultaneousKeyPresses(void)
    {}

    void
    SimultaneousKeyPresses::add(unsigned int datatype, unsigned int newval)
    {
      switch (datatype) {
        case BRIDGE_DATATYPE_KEYCODE:
        case BRIDGE_DATATYPE_POINTINGBUTTON:
        {
          // ----------------------------------------
          // The first data is virtualkey_.
          if (index_ == 0) {
            if (datatype != BRIDGE_DATATYPE_KEYCODE) {
              IOLOG_ERROR("Invalid SimultaneousKeyPresses::add\n");
              return;
            }
            virtualkey_ = newval;
            goto finish;
          }

          // ----------------------------------------
          // From keys
          if (! isFromInfoFull_) {
            FromInfo fi;
            switch (datatype) {
              case BRIDGE_DATATYPE_KEYCODE:        fi.set(KeyCode(newval));        break;
              case BRIDGE_DATATYPE_POINTINGBUTTON: fi.set(PointingButton(newval)); break;
              default:
                IOLOG_ERROR("Invalid SimultaneousKeyPresses::add unknown datatype\n");
                return;
            }
            fromInfo_.push_back(fi);

            if (isUseSeparator_ == false) {
              if (index_ == 2) {
                isFromInfoFull_ = true;
              }
            }
            goto finish;
          }

          // ----------------------------------------
          // To keys
          {
            ToType type = TOTYPE_NONE;
            switch (datatype) {
              case BRIDGE_DATATYPE_KEYCODE:        type = TOTYPE_KEY;    break;
              case BRIDGE_DATATYPE_POINTINGBUTTON: type = TOTYPE_BUTTON; break;
              default:
                IOLOG_ERROR("Invalid SimultaneousKeyPresses::add unknown datatype\n");
                return;
            }

            if (toType_ == TOTYPE_NONE) {
              toType_ = type;
              switch (datatype) {
                case BRIDGE_DATATYPE_KEYCODE:
                  keytokey_.add(virtualkey_);
                  keytokey_.add(fromFlags_);
                  toKey_raw_ = newval;
                  break;

                case BRIDGE_DATATYPE_POINTINGBUTTON:
                  keytopointingbutton_.add(virtualkey_);
                  keytopointingbutton_.add(fromFlags_);
                  break;

                default:
                  IOLOG_ERROR("Invalid SimultaneousKeyPresses::add unknown datatype\n");
                  return;
              }
            }

            if (toType_ != type) {
              IOLOG_ERROR("Invalid SimultaneousKeyPresses::add\n");
              return;
            }

            switch (datatype) {
              case BRIDGE_DATATYPE_KEYCODE:
                keytokey_.add(KeyCode(newval));
                break;

              case BRIDGE_DATATYPE_POINTINGBUTTON:
                keytopointingbutton_.add(PointingButton(newval));
                break;

              default:
                IOLOG_ERROR("Invalid SimultaneousKeyPresses::add unknown datatype\n");
                return;
            }
          }

        finish:
          ++index_;

          break;
        }

        case BRIDGE_DATATYPE_FLAGS:
        {
          if (index_ < 2) {
            IOLOG_ERROR("Invalid SimultaneousKeyPresses::add\n");
            return;
          } else if (toType_ == TOTYPE_NONE) {
            fromFlags_ = newval;
          } else {
            switch (toType_) {
              case TOTYPE_KEY:
                keytokey_.add(Flags(newval));
                break;

              case TOTYPE_BUTTON:
                keytopointingbutton_.add(Flags(newval));
                break;

              default:
                IOLOG_ERROR("Invalid SimultaneousKeyPresses::add unknown datatype\n");
                return;
            }
          }
          break;
        }

        case BRIDGE_DATATYPE_OPTION:
        {
          if (Option::SIMULTANEOUSKEYPRESSES_RAW == newval) {
            if (toType_ != TOTYPE_KEY) {
              IOLOG_ERROR("Invalid SimultaneousKeyPresses::add\n");
            } else {
              isToRaw_ = true;
            }
          } else if (Option::SIMULTANEOUSKEYPRESSES_STRICT_KEY_ORDER == newval) {
            isStrictKeyOrder_ = true;

          } else if (Option::USE_SEPARATOR == newval) {
            isUseSeparator_ = true;

          } else if (Option::SEPARATOR == newval) {
            isFromInfoFull_ = true;

          } else if (Option::NOREPEAT == newval ||
                     Option::KEYTOKEY_BEFORE_KEYDOWN == newval ||
                     Option::KEYTOKEY_AFTER_KEYUP == newval) {
            if (toType_ != TOTYPE_KEY) {
              IOLOG_ERROR("Invalid SimultaneousKeyPresses::add\n");
            } else {
              keytokey_.add(Option(newval));
            }

          } else {
            IOLOG_ERROR("SimultaneousKeyPresses::add unknown option:%d\n", newval);
          }
          break;
        }

        default:
          IOLOG_ERROR("SimultaneousKeyPresses::add invalid datatype:%d\n", datatype);
          break;
      }
    }

    bool
    SimultaneousKeyPresses::FromInfo::isTarget(bool& isKeyDown, const EventInputQueue::Item& item) const
    {
      switch (type_) {
        case FROMTYPE_KEY:
        {
          if (item.params.type != ParamsUnion::KEYBOARD) return false;

          Params_KeyboardEventCallBack* params = item.params.params.params_KeyboardEventCallBack;
          if (! params) return false;

          if (params->key != key_) return false;

          isKeyDown = params->ex_iskeydown;
          return true;
        }

        case FROMTYPE_BUTTON:
        {
          if (item.params.type != ParamsUnion::RELATIVE_POINTER) return false;

          Params_RelativePointerEventCallback* params = item.params.params.params_RelativePointerEventCallback;
          if (! params) return false;

          if (params->ex_button != button_) return false;

          isKeyDown = params->ex_isbuttondown;
          return true;
        }
      }

      return false;
    }

    bool
    SimultaneousKeyPresses::FromInfo::isTargetKeyDown(const EventInputQueue::Item& item) const
    {
      bool isKeyDown = false;
      if (! isTarget(isKeyDown, item)) return false;
      return isKeyDown;
    }

    bool
    SimultaneousKeyPresses::FromInfo::isTargetKeyUp(const EventInputQueue::Item& item) const
    {
      bool isKeyDown = false;
      if (! isTarget(isKeyDown, item)) return false;
      return ! isKeyDown;
    }

    bool
    SimultaneousKeyPresses::remap(void)
    {
      if (! EventInputQueue::queue_) return false;

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

      EventInputQueue::Item* front = static_cast<EventInputQueue::Item*>(EventInputQueue::queue_->front());
      if (! front) return false;

      // backup device information.
      DeviceIdentifier deviceIdentifier(front->deviceIdentifier);

      // ------------------------------------------------------------
      // fire KeyUp event if needed.
      for (size_t i = 0; i < fromInfo_.size(); ++i) {
        if (! fromInfo_[i].isActive()) continue;
        if (! fromInfo_[i].isTargetKeyUp(*front)) continue;

        // --------------------
        EventInputQueue::queue_->pop_front();
        fromInfo_[i].deactivate();

        // --------------------
        // if all keys are released, fire KeyUp event.
        bool isAllDeactived = true;
        for (size_t j = 0; j < fromInfo_.size(); ++j) {
          if (fromInfo_[j].isActive()) {
            isAllDeactived = false;
          }
        }
        if (isAllDeactived) {
          push_remapped(false, deviceIdentifier);
        }

        return true;
      }

      // ------------------------------------------------------------
      // handle KeyDown event.
      if (! FlagStatus::makeFlags().isOn(fromFlags_)) return false;

      // --------------------
      // scan items in queue_.
      while (downKeys_.size() < fromInfo_.size()) {
        downKeys_.push_back(NULL);
      }
      // Then, downKeys_.size() >= fromInfo_.size()

      for (size_t i = 0; i < fromInfo_.size(); ++i) {
        downKeys_[i].item = NULL;
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
          if (fromInfo_[i].isTargetKeyDown(*front)) {
            downKeys_[i].item = front;
            break;
          } else if (fromInfo_[i].isTargetKeyUp(*front)) {
            return false;
          }
        }

        // ----------------------------------------
        bool isAllKeysDown = true;
        for (size_t i = 0; i < fromInfo_.size(); ++i) {
          if (! downKeys_[i].item) {
            isAllKeysDown = false;
          } else {
            // Checking strict key order.
            //
            // If isStrictKeyOrder_ == true,
            // we must not handle the following state as SimultaneousKeyPresses.
            //
            // - downKeys_[0] == NULL
            // - downKeys_[1] != NULL
            //
            if (! isAllKeysDown && isStrictKeyOrder_) {
              return false;
            }
          }
        }

        if (isAllKeysDown) {
          for (size_t i = 0; i < fromInfo_.size(); ++i) {
            fromInfo_[i].activate();
            EventInputQueue::queue_->erase(downKeys_[i].item);
          }
          push_remapped(true, deviceIdentifier);
          return true;
        }

        // ----------------------------------------
        front = static_cast<EventInputQueue::Item*>(front->getnext());
        if (! front) return false;
      }

      return false;
    }

    void
    SimultaneousKeyPresses::push_remapped(bool isKeyDown, const DeviceIdentifier& deviceIdentifier)
    {
      EventType eventType = isKeyDown ? EventType::DOWN : EventType::UP;

      KeyCode key = virtualkey_;
      if (isToRaw_) {
        key = toKey_raw_;
      }
      if (key == KeyCode::VK_NONE) return;

      Flags flags(0);
      if (isKeyDown) {
        flags.add(key.getModifierFlag());
        flags.stripNONE();
      }

      // ----------------------------------------
      Params_KeyboardEventCallBack::auto_ptr ptr(Params_KeyboardEventCallBack::alloc(eventType, flags, key, CommonData::getcurrent_keyboardType(), false));
      if (! ptr) return;

      Params_KeyboardEventCallBack& params = *ptr;
      bool retainFlagStatusTemporaryCount = false;
      bool push_back = false;
      EventInputQueue::enqueue_(params, retainFlagStatusTemporaryCount, deviceIdentifier, push_back);
    }

    bool
    SimultaneousKeyPresses::remap(RemapParams& remapParams)
    {
      if (toType_ == TOTYPE_KEY) {
        return keytokey_.remap(remapParams);
      } else {
        return keytopointingbutton_.remap(remapParams);
      }
    }
  }
}
