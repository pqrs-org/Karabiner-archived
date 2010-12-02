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
              fromInfo1_.set(KeyCode(newval));
              break;

            case 2:
              fromInfo2_.set(KeyCode(newval));
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
              fromInfo1_.set(PointingButton(newval));
              break;

            case 2:
              fromInfo2_.set(PointingButton(newval));
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
    SimultaneousKeyPresses::FromInfo::isTarget(bool& isKeyDown, const EventInputQueue::Item& item)
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

    void
    SimultaneousKeyPresses::remap(void)
    {
      // ----------------------------------------
      // we change queue_->front() only.
      //
      if (! EventInputQueue::queue_) return;

      EventInputQueue::Item* front = static_cast<EventInputQueue::Item*>(EventInputQueue::queue_->front());
      if (! front) return;

      // ----------------------------------------
      // fire KeyUp event if needed.
      bool isKeyDown = false;
      if (fromInfo1_.isActive() && fromInfo1_.isTarget(isKeyDown, *front) && ! isKeyDown) {
        EventInputQueue::queue_->pop_front();
        fromInfo1_.deactivate();
        push_remapped(false);
        return;
      }
      if (fromInfo2_.isActive() && fromInfo2_.isTarget(isKeyDown, *front) && ! isKeyDown) {
        EventInputQueue::queue_->pop_front();
        fromInfo2_.deactivate();
        return;
      }

      // ----------------------------------------
      if (! FlagStatus::makeFlags().isOn(fromFlags_)) return;

      FromInfo* frontFromInfo = NULL;
      FromInfo* pairedFromInfo = NULL;

      /*  */ if (fromInfo1_.isTarget(isKeyDown, *front)) {
        frontFromInfo  = &fromInfo1_;
        pairedFromInfo = &fromInfo2_;
      } else if (fromInfo2_.isTarget(isKeyDown, *front)) {
        frontFromInfo  = &fromInfo2_;
        pairedFromInfo = &fromInfo1_;
      }

      if (! frontFromInfo || ! pairedFromInfo) return;
      if (! isKeyDown) return;

      // ----------------------------------------
      for (;;) {
        front = static_cast<EventInputQueue::Item*>(front->getnext());
        if (! front) return;

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
        if (frontFromInfo->isTarget(isKeyDown, *front)) return;

        if (pairedFromInfo->isTarget(isKeyDown, *front)) {
          if (! isKeyDown) return;

          // SimultaneousKeyPresses!
          EventInputQueue::queue_->erase(front);
          break;
        }
      }

      fromInfo1_.activate();
      fromInfo2_.activate();

      EventInputQueue::queue_->pop_front();
      push_remapped(true);
    }

    void
    SimultaneousKeyPresses::push_remapped(bool isKeyDown)
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
      EventInputQueue::enqueue_(params, false);
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
