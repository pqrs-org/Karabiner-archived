#include <IOKit/IOLib.h>

#include "ConsumerToConsumer.hpp"
#include "EventOutputQueue.hpp"
#include "IOLogWrapper.hpp"
#include "KeyboardRepeat.hpp"
#include "ListHookedConsumer.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace RemapFunc {
    ConsumerToConsumer::ConsumerToConsumer(void) : index_(0), keyboardRepeatID_(-1), isRepeatEnabled_(true)
    {}

    ConsumerToConsumer::~ConsumerToConsumer(void)
    {}

    void
    ConsumerToConsumer::add(unsigned int datatype, unsigned int newval)
    {
      switch (datatype) {
        case BRIDGE_DATATYPE_CONSUMERKEYCODE:
        {
          switch (index_) {
            case 0:
              fromKey_.key = newval;
              break;
            default:
              toKeys_.push_back(PairConsumerKeyFlags(newval));
              break;
          }
          ++index_;

          break;
        }

        case BRIDGE_DATATYPE_FLAGS:
        {
          switch (index_) {
            case 0:
              IOLOG_ERROR("Invalid ConsumerToConsumer::add\n");
              break;
            case 1:
              fromKey_.flags = Flags(newval);
              break;
            default:
              if (! toKeys_.empty()) {
                toKeys_.back().flags = Flags(newval);
              }
              break;
          }
          break;
        }

        case BRIDGE_DATATYPE_OPTION:
        {
          if (Option::NOREPEAT == newval) {
            isRepeatEnabled_ = false;
          } else {
            IOLOG_ERROR("ConsumerToConsumer::add unknown option:%d\n", newval);
          }
          break;
        }

        default:
          IOLOG_ERROR("ConsumerToConsumer::add invalid datatype:%d\n", datatype);
          break;
      }
    }

    bool
    ConsumerToConsumer::remap(RemapConsumerParams& remapParams)
    {
      if (remapParams.isremapped) return false;
      if (! fromkeychecker_.isFromKey(remapParams.params.ex_iskeydown, remapParams.params.key, FlagStatus::makeFlags(), fromKey_.key, fromKey_.flags)) return false;
      remapParams.isremapped = true;

      // ------------------------------------------------------------
      // NumLock Hack
      // If we change NumLock key, we need to call IOHIKeyboard::setNumLock(false).
      // Unless call setNumLock, internal NumLock status of IOHIKeyboard is still active.
      // And NumLock retains working status.
      if (fromKey_.key == ConsumerKeyCode::NUMLOCK) {
        bool tonumlock = false;
        for (size_t i = 0; i < toKeys_.size(); ++i) {
          if (toKeys_[i].key == ConsumerKeyCode::NUMLOCK) {
            tonumlock = true;
          }
        }
        if (! tonumlock) {
          ListHookedConsumer::instance().disableNumLock();
        }
      }

      // ----------------------------------------
      FlagStatus::temporary_decrease(fromKey_.flags);

      switch (toKeys_.size()) {
        case 0:
          break;

        case 1:
        {
          FlagStatus::temporary_increase(toKeys_[0].flags);
          Params_KeyboardSpecialEventCallback::auto_ptr ptr(Params_KeyboardSpecialEventCallback::alloc(remapParams.params.eventType,
                                                                                                       FlagStatus::makeFlags(),
                                                                                                       toKeys_[0].key,
                                                                                                       false));
          if (! ptr) return false;
          Params_KeyboardSpecialEventCallback& params = *ptr;

          if (remapParams.params.ex_iskeydown && ! isRepeatEnabled_) {
            KeyboardRepeat::cancel();
          } else {
            KeyboardRepeat::set(params);
          }
          EventOutputQueue::FireConsumer::fire(params);
          break;
        }

        default:
          if (remapParams.params.ex_iskeydown) {
            KeyboardRepeat::cancel();

            for (size_t i = 0; i < toKeys_.size(); ++i) {
              FlagStatus::temporary_increase(toKeys_[i].flags);

              ConsumerKeyCode key = toKeys_[i].key;
              Flags flags = FlagStatus::makeFlags();
              {
                Params_KeyboardSpecialEventCallback::auto_ptr ptr(Params_KeyboardSpecialEventCallback::alloc(EventType::DOWN, flags, key, false));
                if (! ptr) return false;
                EventOutputQueue::FireConsumer::fire(*ptr);
              }
              {
                Params_KeyboardSpecialEventCallback::auto_ptr ptr(Params_KeyboardSpecialEventCallback::alloc(EventType::UP, flags, key, false));
                if (! ptr) return false;
                EventOutputQueue::FireConsumer::fire(*ptr);
              }

              KeyboardRepeat::primitive_add(EventType::DOWN, flags, key);
              KeyboardRepeat::primitive_add(EventType::UP,   flags, key);

              FlagStatus::temporary_decrease(toKeys_[i].flags);
            }

            if (isRepeatEnabled_) {
              keyboardRepeatID_ = KeyboardRepeat::primitive_start(Config::get_repeat_consumer_initial_wait(),
                                                                  Config::get_repeat_consumer_wait());
            } else {
              keyboardRepeatID_ = -1;
            }

          } else {
            if (KeyboardRepeat::getID() == keyboardRepeatID_) {
              KeyboardRepeat::cancel();
            }
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
