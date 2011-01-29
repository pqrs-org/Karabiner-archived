#include "EventOutputQueue.hpp"
#include "KeyToPointingButton.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace RemapFunc {
    KeyToPointingButton::KeyToPointingButton(void) : index_(0)
    {
      toButtons_ = new Vector_PairPointingButtonFlags();
    }

    KeyToPointingButton::~KeyToPointingButton(void)
    {
      if (toButtons_) {
        delete toButtons_;
      }
    }

    void
    KeyToPointingButton::add(unsigned int datatype, unsigned int newval)
    {
      if (! toButtons_) return;

      switch (datatype) {
        case BRIDGE_DATATYPE_KEYCODE:
        {
          switch (index_) {
            case 0:
              fromKey_.key = newval;
              break;

            default:
              IOLOG_ERROR("Invalid KeyToPointingButton::add\n");
              break;
          }
          ++index_;

          break;
        }

        case BRIDGE_DATATYPE_POINTINGBUTTON:
        {
          switch (index_) {
            case 0:
              IOLOG_ERROR("Invalid KeyToPointingButton::add\n");
              break;

            default:
              toButtons_->push_back(PairPointingButtonFlags(newval));
              break;
          }
          ++index_;

          break;
        }

        case BRIDGE_DATATYPE_FLAGS:
        {
          switch (index_) {
            case 0:
              IOLOG_ERROR("Invalid KeyToPointingButton::add\n");
              break;

            case 1:
              fromKey_.flags = newval;
              break;

            default:
              if (! toButtons_->empty()) {
                (toButtons_->back()).flags = newval;
              }
              break;
          }
          break;
        }

        default:
          IOLOG_ERROR("KeyToPointingButton::add invalid datatype:%d\n", datatype);
          break;
      }
    }

    bool
    KeyToPointingButton::remap(RemapParams& remapParams)
    {
      if (! toButtons_) return false;

      if (remapParams.isremapped) return false;
      if (! fromkeychecker_.isFromKey(remapParams.params.ex_iskeydown, remapParams.params.key, FlagStatus::makeFlags(), fromKey_.key, fromKey_.flags)) return false;
      remapParams.isremapped = true;

      // ------------------------------------------------------------
      // We need to call FlagStatus::decrease before mapping,
      // and call FlagStatus::increase after a mapping.
      //
      // ex. Option_L+Space to Right Button
      // (1) KeyDown Option_L
      //      1. KeyDown Option_L
      // (2) KeyDown Space
      //      2. KeyUp   Option_L
      //      3. ButtonDown Right
      // (3) KeyUp   Space
      //      4. ButtonUp   Right
      //      5. KeyUp   Option_L
      //
      // *** To support Mouse Dragging,
      // *** we need to use FlagStatus::decrease/increase.
      // *** (not temporary_decrease/temporary_increase).

      if (remapParams.params.ex_iskeydown) {
        FlagStatus::decrease(fromKey_.flags | fromKey_.key.getModifierFlag());
      }
      // Do not cancel KeyboardRepeat here.
      // Because the mouse click event must not stop the key-repeat.
      //
      // OmniGraffle has a function which pull a line by mouse click with pressing "C" key.
      // If you stop key-repeat, this function fails.
      switch (toButtons_->size()) {
        case 0:
          break;

        case 1:
          if (remapParams.params.ex_iskeydown) {
            FlagStatus::increase((*toButtons_)[0].flags);
            ButtonStatus::increase((*toButtons_)[0].button);
          } else {
            FlagStatus::decrease((*toButtons_)[0].flags);
            ButtonStatus::decrease((*toButtons_)[0].button);
          }
          EventOutputQueue::FireRelativePointer::fire();
          break;

        case 2:
          if (remapParams.params.ex_iskeydown) {
            for (size_t i = 0; i < toButtons_->size(); ++i) {
              FlagStatus::temporary_increase((*toButtons_)[i].flags);

              ButtonStatus::increase((*toButtons_)[i].button);
              EventOutputQueue::FireRelativePointer::fire(ButtonStatus::makeButtons());
              ButtonStatus::decrease((*toButtons_)[i].button);
              EventOutputQueue::FireRelativePointer::fire(ButtonStatus::makeButtons());

              FlagStatus::temporary_decrease((*toButtons_)[i].flags);
            }
          }
          break;
      }

      if (! remapParams.params.ex_iskeydown) {
        FlagStatus::increase(fromKey_.flags | fromKey_.key.getModifierFlag());
        EventOutputQueue::FireModifiers::fire();
      }

      return true;
    }
  }
}
