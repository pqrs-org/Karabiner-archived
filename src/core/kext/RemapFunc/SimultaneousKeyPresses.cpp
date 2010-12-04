#include "CommonData.hpp"
#include "EventInputQueue.hpp"
#include "SimultaneousKeyPresses.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace RemapFunc {
    SimultaneousKeyPresses::SimultaneousKeyPresses(void) : index_(0)
    {
      toType_ = TOTYPE_NONE;
    }

    SimultaneousKeyPresses::~SimultaneousKeyPresses(void)
    {}

    void
    SimultaneousKeyPresses::add(unsigned int datatype, unsigned int newval)
    {
      switch (datatype) {
        case BRIDGE_DATATYPE_KEYCODE:
        {
          switch (index_) {
            case 0:
              virtualkey_ = newval;
              break;

            case 1:
              fromInfo_[0].set(KeyCode(newval));
              break;

            case 2:
              fromInfo_[1].set(KeyCode(newval));
              break;

            case 3:
              // pass-through (== no break)
              keytokey_.add(virtualkey_);
              keytokey_.add(fromFlags_);
              toKey_raw_ = newval;
            default:
              if (toType_ != TOTYPE_NONE && toType_ != TOTYPE_KEY) {
                IOLOG_ERROR("Invalid SimultaneousKeyPresses::add\n");
                break;
              }
              toType_ = TOTYPE_KEY;

              keytokey_.add(KeyCode(newval));
              break;
          }
          ++index_;

          break;
        }

        case BRIDGE_DATATYPE_POINTINGBUTTON:
        {
          switch (index_) {
            case 0:
              IOLOG_ERROR("Invalid SimultaneousKeyPresses::add\n");
              break;

            case 1:
              fromInfo_[0].set(PointingButton(newval));
              break;

            case 2:
              fromInfo_[1].set(PointingButton(newval));
              break;

            case 3:
              // pass-through (== no break)
              keytopointingbutton_.add(virtualkey_);
              keytopointingbutton_.add(fromFlags_);
            default:
              if (toType_ != TOTYPE_NONE && toType_ != TOTYPE_BUTTON) {
                IOLOG_ERROR("Invalid SimultaneousKeyPresses::add\n");
                break;
              }
              toType_ = TOTYPE_BUTTON;

              keytopointingbutton_.add(PointingButton(newval));
              break;
          }
          ++index_;

          break;
        }

        case BRIDGE_DATATYPE_FLAGS:
        {
          switch (index_) {
            case 0:
            case 1:
              IOLOG_ERROR("Invalid SimultaneousKeyPresses::add\n");
              break;

            case 2:
            case 3:
              fromFlags_ = newval;
              break;

            default:
              switch (toType_) {
                case TOTYPE_NONE:
                  IOLOG_ERROR("Invalid SimultaneousKeyPresses::add\n");
                  break;

                case TOTYPE_KEY:
                  keytokey_.add(Flags(newval));
                  break;

                case TOTYPE_BUTTON:
                  keytopointingbutton_.add(Flags(newval));
                  break;
              }
              break;
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
      // ----------------------------------------
      // we change queue_->front() only.
      //
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

      EventInputQueue::Item * front = static_cast<EventInputQueue::Item*>(EventInputQueue::queue_->front());
      if (! front) return false;

      // backup device information.
      DeviceVendor deviceVendor = front->deviceVendor;
      DeviceProduct deviceProduct = front->deviceProduct;

      // ------------------------------------------------------------
      // fire KeyUp event if needed.
      for (size_t i = 0; i < sizeof(fromInfo_) / sizeof(fromInfo_[0]); ++i) {
        if (! fromInfo_[i].isActive()) continue;
        if (! fromInfo_[i].isTargetKeyUp(*front)) continue;

        // --------------------
        EventInputQueue::queue_->pop_front();
        fromInfo_[i].deactivate();

        // --------------------
        // if all keys are released, fire KeyUp event.
        bool isAllDeactived = true;
        for (size_t j = 0; j < sizeof(fromInfo_) / sizeof(fromInfo_[0]); ++j) {
          if (fromInfo_[j].isActive()) {
            isAllDeactived = false;
          }
        }
        if (isAllDeactived) {
          push_remapped(false, deviceVendor, deviceProduct);
        }

        return true;
      }

      // ------------------------------------------------------------
      // handle KeyDown event.
      if (! FlagStatus::makeFlags().isOn(fromFlags_)) return false;

      FromInfo* frontFromInfo = NULL;
      FromInfo* pairedFromInfo = NULL;

      /*  */ if (fromInfo_[0].isTargetKeyDown(*front)) {
        frontFromInfo  = fromInfo_ + 0;
        pairedFromInfo = fromInfo_ + 1;
      } else if (fromInfo_[1].isTargetKeyDown(*front)) {
        frontFromInfo  = fromInfo_ + 1;
        pairedFromInfo = fromInfo_ + 0;
      }

      if (! frontFromInfo || ! pairedFromInfo) return false;

      // --------------------
      // scan items in queue_.
      for (;;) {
        front = static_cast<EventInputQueue::Item*>(front->getnext());
        if (! front) return false;

        // We consider "Shift_L+Shift_R to Space".
        // When we press keys by the following order.
        //
        // (1) Shift_L Down
        // (2) Shift_L Up
        // (3) Shift_R Down
        // (4) Shift_R Up
        //
        // frontFromInfo == Shift_L
        // pairedFromInfo == Shift_R
        //
        // If frontFromInfo is appeared before pairedFromInfo,
        // we must not handle these keys as SimultaneousKeyPresses.
        //
        if (frontFromInfo->isTargetKeyDown(*front)) return false;
        if (frontFromInfo->isTargetKeyUp(*front)) return false;

        if (pairedFromInfo->isTargetKeyDown(*front)) {
          // SimultaneousKeyPresses!
          EventInputQueue::queue_->erase(front);
          break;
        }
      }

      fromInfo_[0].activate();
      fromInfo_[1].activate();

      EventInputQueue::queue_->pop_front();
      push_remapped(true, deviceVendor, deviceProduct);

      return true;
    }

    void
    SimultaneousKeyPresses::push_remapped(bool isKeyDown, DeviceVendor deviceVendor, DeviceProduct deviceProduct)
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
      bool updateWorkspaceData = false;
      bool push_back = false;
      EventInputQueue::enqueue_(params, retainFlagStatusTemporaryCount, updateWorkspaceData, deviceVendor, deviceProduct, push_back);
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
