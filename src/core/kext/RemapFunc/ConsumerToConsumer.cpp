#include "EventOutput.hpp"
#include "ConsumerToConsumer.hpp"
#include "KeyboardRepeat.hpp"
#include "ListHookedKeyboard.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace RemapFunc {
    void
    ConsumerToConsumer::initialize(void)
    {
      toKeys_ = new Vector_PairConsumerKeyFlags();
    }

    void
    ConsumerToConsumer::terminate(void)
    {
      if (toKeys_) {
        delete toKeys_;
        toKeys_ = NULL;
      }
    }

    void
    ConsumerToConsumer::add(ConsumerKeyCode newval)
    {
      if (! toKeys_) return;

      switch (index_) {
        case 0:
          fromKey_.key = newval;
          break;
        default:
          toKeys_->push_back(PairConsumerKeyFlags(newval));
          break;
      }
      ++index_;
    }

    void
    ConsumerToConsumer::add(Flags newval)
    {
      if (! toKeys_) return;

      switch (index_) {
        case 0:
          IOLOG_ERROR("Invalid ConsumerToConsumer::add\n");
          break;
        case 1:
          fromKey_.flags = newval;
          break;
        default:
          toKeys_->back().flags = newval;
          break;
      }
    }

    bool
    ConsumerToConsumer::remap(RemapConsumerParams& remapParams)
    {
      if (! toKeys_) return false;

      // ------------------------------------------------------------
      // NumLock Hack
      // If we change NumLock key, we need to call IOHIKeyboard::setNumLock(false).
      // Unless call setNumLock, internal NumLock status of IOHIKeyboard is still active.
      // And NumLock retains working status.
      if (fromKey_.key == ConsumerKeyCode::NUMLOCK) {
        bool tonumlock = false;
        for (size_t i = 0; i < toKeys_->size(); ++i) {
          if ((*toKeys_)[i].key == ConsumerKeyCode::NUMLOCK) {
            tonumlock = true;
          }
        }
        if (! tonumlock) {
          HookedKeyboard* hk = ListHookedKeyboard::instance().get();
          if (hk) {
            IOHIKeyboard* kbd = hk->get();
            if (kbd) {
              if (kbd->numLock()) {
                kbd->setNumLock(false);
              }
            }
          }
        }
      }

      // ------------------------------------------------------------
      if (remapParams.isremapped) return false;
      if (! fromkeychecker_.isFromKey(remapParams.params, fromKey_.key, fromKey_.flags)) return false;
      remapParams.isremapped = true;

      // ----------------------------------------
      FlagStatus::temporary_decrease(fromKey_.flags);
      KeyboardRepeat::cancel();

      switch (toKeys_->size()) {
        case 0:
          break;

        case 1:
        {
          FlagStatus::temporary_increase((*toKeys_)[0].flags);
          Params_KeyboardSpecialEventCallback::auto_ptr ptr(Params_KeyboardSpecialEventCallback::alloc(remapParams.params.eventType,
                                                                                                       FlagStatus::makeFlags(),
                                                                                                       (*toKeys_)[0].key,
                                                                                                       false));
          if (! ptr) return false;
          Params_KeyboardSpecialEventCallback& params = *ptr;

          KeyboardRepeat::set(params);
          EventOutput::FireConsumer::fire(params);
          break;
        }

        default:
          if (remapParams.params.eventType == EventType::DOWN) {
            for (size_t i = 0; i < toKeys_->size(); ++i) {
              FlagStatus::temporary_increase((*toKeys_)[i].flags);
              Params_KeyboardSpecialEventCallback::auto_ptr ptr(Params_KeyboardSpecialEventCallback::alloc(EventType::DOWN,
                                                                                                           FlagStatus::makeFlags(),
                                                                                                           (*toKeys_)[i].key,
                                                                                                           false));
              if (! ptr) return false;
              Params_KeyboardSpecialEventCallback& params = *ptr;

              EventOutput::FireConsumer::fire(params);
              params.eventType = EventType::UP;
              EventOutput::FireConsumer::fire(params);

              KeyboardRepeat::primitive_add(EventType::DOWN, params.flags, params.key);
              KeyboardRepeat::primitive_add(EventType::UP,   params.flags, params.key);

              FlagStatus::temporary_decrease((*toKeys_)[i].flags);
            }

            KeyboardRepeat::primitive_start();
          }

          break;
      }

      return true;
    }

    bool
    ConsumerToConsumer::call_remap_with_VK_PSEUDO_KEY(EventType eventType)
    {
      Params_KeyboardSpecialEventCallback::auto_ptr ptr(Params_KeyboardSpecialEventCallback::alloc(eventType,
                                                                                                   FlagStatus::makeFlags(),
                                                                                                   ConsumerKeyCode::VK_PSEUDO_KEY,
                                                                                                   false));
      if (! ptr) return false;
      Params_KeyboardSpecialEventCallback& params = *ptr;

      RemapConsumerParams rp(params);
      return remap(rp);
    }
  }
}
