#include "CommonData.hpp"
#include "EventOutputQueue.hpp"
#include "KeyboardRepeat.hpp"
#include "ToEvent.hpp"
#include "VirtualKey.hpp"

namespace org_pqrs_Karabiner {
bool ToEvent::isEventLikeModifier(void) const {
  switch (type_) {
  case Type::NONE:
    return false;
  case Type::KEY:
    return VirtualKey::isKeyLikeModifier(key_);
  case Type::CONSUMER_KEY:
    return false;
  case Type::POINTING_BUTTON:
    return true;
  }
}

void ToEvent::fire(EventType eventType, Flags flags, AutogenId autogenId, PhysicalEventType physicalEventType,
                   bool add_to_keyrepeat, int delayUntilRepeat, int keyRepeat) {
  switch (type_) {
  case Type::NONE:
    break;

  case Type::KEY: {
    Params_KeyboardEventCallBack params(eventType,
                                        flags,
                                        key_,
                                        CommonData::getcurrent_keyboardType(),
                                        false);
    EventOutputQueue::FireKey::fire(params, autogenId, physicalEventType);
    if (add_to_keyrepeat) {
      KeyboardRepeat::set(params, autogenId, delayUntilRepeat, keyRepeat);
    }
    break;
  }

  case Type::CONSUMER_KEY: {
    Params_KeyboardSpecialEventCallback params(eventType,
                                               flags,
                                               consumer_,
                                               false);
    EventOutputQueue::FireConsumer::fire(params, autogenId, physicalEventType);
    if (add_to_keyrepeat) {
      KeyboardRepeat::set(params, autogenId, delayUntilRepeat, keyRepeat);
    }
    break;
  }

  case Type::POINTING_BUTTON: {
    if (eventType == EventType::DOWN) {
      ButtonStatus::increase(button_);
    } else {
      ButtonStatus::decrease(button_);
    }

    EventOutputQueue::FireRelativePointer::fire(autogenId, physicalEventType, ButtonStatus::makeButtons());

    if (add_to_keyrepeat) {
      // Do nothing
    }
    break;
  }
  }
}

void ToEvent::fire_downup(AutogenId autogenId, PhysicalEventType physicalEventType, bool add_to_keyrepeat) const {
  switch (type_) {
  case Type::NONE:
    break;

  case Type::KEY: {
    KeyboardType keyboardType = CommonData::getcurrent_keyboardType();
    ModifierFlag f = key_.getModifierFlag();

#define CALL_FIREKEY_FIRE(params)                                        \
  EventOutputQueue::FireKey::fire(params, autogenId, physicalEventType); \
  if (add_to_keyrepeat) {                                                \
    KeyboardRepeat::primitive_add(params);                               \
  }

    if (f != ModifierFlag::ZERO) {
      {
        FlagStatus::globalFlagStatus().increase(f);
        Flags flags = FlagStatus::globalFlagStatus().makeFlags();
        Params_KeyboardEventCallBack params(EventType::MODIFY, flags, key_, keyboardType, false);
        CALL_FIREKEY_FIRE(params);
      }
      {
        FlagStatus::globalFlagStatus().decrease(f);
        Flags flags = FlagStatus::globalFlagStatus().makeFlags();
        Params_KeyboardEventCallBack params(EventType::MODIFY, flags, key_, keyboardType, false);
        CALL_FIREKEY_FIRE(params);
      }
    } else {
      Flags flags = FlagStatus::globalFlagStatus().makeFlags();
      {
        Params_KeyboardEventCallBack params(EventType::DOWN, flags, key_, keyboardType, false);
        CALL_FIREKEY_FIRE(params);
      }
      {
        Params_KeyboardEventCallBack params(EventType::UP, flags, key_, keyboardType, false);
        CALL_FIREKEY_FIRE(params);
      }
    }

#undef CALL_FIREKEY_FIRE

    break;
  }

  case Type::CONSUMER_KEY: {
    Flags flags = FlagStatus::globalFlagStatus().makeFlags();

#define CALL_FIRECONSUMER_FIRE(params)                                        \
  EventOutputQueue::FireConsumer::fire(params, autogenId, physicalEventType); \
  if (add_to_keyrepeat) {                                                     \
    KeyboardRepeat::primitive_add(params);                                    \
  }

    {
      Params_KeyboardSpecialEventCallback params(EventType::DOWN, flags, consumer_, false);
      CALL_FIRECONSUMER_FIRE(params);
    }
    {
      Params_KeyboardSpecialEventCallback params(EventType::UP, flags, consumer_, false);
      CALL_FIRECONSUMER_FIRE(params);
    }

#undef CALL_FIRECONSUMER_FIRE

    break;
  }

  case Type::POINTING_BUTTON: {
    for (int i = 0; i < 2; ++i) {
      if (i == 0) {
        ButtonStatus::increase(button_);
      } else {
        ButtonStatus::decrease(button_);
      }

      Buttons buttons(ButtonStatus::makeButtons());
      EventOutputQueue::FireRelativePointer::fire(autogenId, physicalEventType, buttons);
      if (add_to_keyrepeat) {
        KeyboardRepeat::primitive_add(buttons);
      }
    }
    break;
  }
  }
}
}
