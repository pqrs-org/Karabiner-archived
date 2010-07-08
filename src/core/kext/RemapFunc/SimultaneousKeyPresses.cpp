#include "CommonData.hpp"
#include "EventInputQueue.hpp"
#include "SimultaneousKeyPresses.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace RemapFunc {
    void
    SimultaneousKeyPresses::initialize(void)
    {
      keytokey_.initialize();
      keytopointingbutton_.initialize();

      toType_ = TOTYPE_NONE;
    }

    void
    SimultaneousKeyPresses::terminate(void)
    {
      keytokey_.terminate();
      keytopointingbutton_.terminate();
    }

    void
    SimultaneousKeyPresses::add(KeyCode newval)
    {
      switch (index_) {
        case 0:
          virtualkey_ = newval;
          break;

        case 1:
          fromInfo1_.set(newval);
          break;

        case 2:
          fromInfo2_.set(newval);
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

          keytokey_.add(newval);
          break;
      }
      ++index_;
    }

    void
    SimultaneousKeyPresses::add(PointingButton newval)
    {
      switch (index_) {
        case 0:
          IOLOG_ERROR("Invalid SimultaneousKeyPresses::add\n");
          break;

        case 1:
          fromInfo1_.set(newval);
          break;

        case 2:
          fromInfo2_.set(newval);
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

          keytopointingbutton_.add(newval);
          break;
      }
      ++index_;
    }

    void
    SimultaneousKeyPresses::add(Flags newval)
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
              keytokey_.add(newval);
              break;

            case TOTYPE_BUTTON:
              keytopointingbutton_.add(newval);
              break;
          }
          break;
      }
    }

    void
    SimultaneousKeyPresses::add(SimultaneousKeyPresses::Option newval)
    {
      switch (newval) {
        case OPTION_RAW:
          if (toType_ != TOTYPE_KEY) {
            IOLOG_ERROR("Invalid SimultaneousKeyPresses::add\n");
          } else {
            isToRaw_ = true;
          }
          break;
      }
    }

    EventInputQueue::Item*
    SimultaneousKeyPresses::FromInfo::findLastItem(bool& isKeyDown, bool isIncludeDropped)
    {
      if (! EventInputQueue::queue_) return NULL;

      for (EventInputQueue::Item* p = static_cast<EventInputQueue::Item*>(EventInputQueue::queue_->back()); p; p = static_cast<EventInputQueue::Item*>(p->getprev())) {
        if (! isIncludeDropped && p->dropped) continue;

        switch (type_) {
          case FROMTYPE_KEY:
          {
            if ((p->params).type != ParamsUnion::KEYBOARD) continue;

            Params_KeyboardEventCallBack* params = (p->params).params.params_KeyboardEventCallBack;
            if (! params) continue;

            if (params->key != key_) continue;

            isKeyDown = (params->eventType).isKeyDownOrModifierDown(params->key, params->flags);
            return p;
          }

          case FROMTYPE_BUTTON:
          {
            if ((p->params).type != ParamsUnion::RELATIVE_POINTER) continue;

            Params_RelativePointerEventCallback* params = (p->params).params.params_RelativePointerEventCallback;
            if (! params) continue;

            if (params->ex_justPressed.isOn(button_)) {
              isKeyDown = true;
              return p;
            }
            if (params->ex_justReleased.isOn(button_)) {
              isKeyDown = false;
              return p;
            }
            break;
          }
        }
      }

      return NULL;
    }

    bool
    SimultaneousKeyPresses::FromInfo::restore(bool isFireUp)
    {
      // restore KeyUp/ButtonUp Event (fromInfo1_)
      // drop    KeyUp/ButtonUp Event (fromInfo2_)

      // We consider "Shift_L+Shift_R to Space".
      // When we press keys by the following order.
      //
      // (1) Shift_L Down
      // (2) Shift_L Up
      // (3) Shift_L Down
      // (4) Shift_R Down
      //
      // Fire Space at (3)+(4), and we need to change/drop Shift_L Up, Shift_R Up.
      // But, we need to change these Up keys after (3),(4). Do not change (2).
      // So, we change keys from tail.
      if (! active_) return false;

      // ------------------------------------------------------------
      bool isKeyDown = false;
      EventInputQueue::Item* item = findLastItem(isKeyDown, true);
      if (! item) return false;
      if (item->dropped) return false;

      if (isKeyDown) return false;

      item->dropped = true;
      active_ = false;
      return true;
    }

    void
    SimultaneousKeyPresses::remap(void)
    {
      if (fromInfo1_.restore(true)) {
        push_remapped(false);
      }
      fromInfo2_.restore(false);

      // ------------------------------------------------------------
      // find fromKeyCode*_
      //
      // We consider "A+B to C".
      // In this case, we change key in case1, but don't change key in case2.
      // Because A is released in case2.
      //
      // case1) "KeyDown A", "KeyDown B"             (=> change key)
      // case2) "KeyDown A", "KeyUp A", "KeyDown B". (=> don't change key)
      //
      // So, we check keys from tail.
      if (! FlagStatus::makeFlags().isOn(fromFlags_)) return;

      bool isKeyDown1 = false;
      bool isKeyDown2 = false;
      EventInputQueue::Item* item1 = fromInfo1_.findLastItem(isKeyDown1, false);
      EventInputQueue::Item* item2 = fromInfo2_.findLastItem(isKeyDown2, false);

      if (! item1      || ! item2 ||
          ! isKeyDown1 || ! isKeyDown2) return;

      item1->dropped = true;
      item2->dropped = true;
      fromInfo1_.activate();
      fromInfo2_.activate();

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
      EventInputQueue::enqueue_(params);
    }

    bool
    SimultaneousKeyPresses::handlevirtualkey(const Params_KeyboardEventCallBack& params)
    {
      RemapParams rp(params);
      if (toType_ == TOTYPE_KEY) {
        return keytokey_.remap(rp);
      } else {
        return keytopointingbutton_.remap(rp);
      }
    }
  }
}
