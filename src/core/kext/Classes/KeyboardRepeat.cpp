#include "CommonData.hpp"
#include "EventOutputQueue.hpp"
#include "IOLogWrapper.hpp"
#include "KeyboardRepeat.hpp"
#include "ListHookedKeyboard.hpp"
#include "RemapClass.hpp"

namespace org_pqrs_Karabiner {
List KeyboardRepeat::queue_;
TimerWrapper KeyboardRepeat::fire_timer_;
int KeyboardRepeat::id_ = 0;
int KeyboardRepeat::keyRepeat_ = 0;
AutogenId KeyboardRepeat::autogenId_(0);

void KeyboardRepeat::initialize(IOWorkLoop& workloop) {
  fire_timer_.initialize(&workloop, nullptr, KeyboardRepeat::fire_timer_callback);
}

void KeyboardRepeat::terminate(void) {
  fire_timer_.terminate();

  queue_.clear();
}

void KeyboardRepeat::cancel(void) {
  fire_timer_.cancelTimeout();

  queue_.clear();

  // Increase id_ at cancel for DependingPressingPeriodKeyToKey to detect keyboardRepeat canceling.
  succID();
}

void KeyboardRepeat::primitive_add(EventType eventType,
                                   Flags flags,
                                   KeyCode key,
                                   KeyboardType keyboardType) {
  if (key == KeyCode::VK_NONE) return;

  // ------------------------------------------------------------
  Params_KeyboardEventCallBack params(eventType,
                                      flags,
                                      key,
                                      keyboardType,
                                      true);
  queue_.push_back(new Item(params));
}

void KeyboardRepeat::primitive_add(EventType eventType,
                                   Flags flags,
                                   ConsumerKeyCode key) {
  if (key == ConsumerKeyCode::VK_NONE) return;

  // ------------------------------------------------------------
  Params_KeyboardSpecialEventCallback params(eventType,
                                             flags,
                                             key,
                                             true);
  queue_.push_back(new Item(params));
}

void KeyboardRepeat::primitive_add(Buttons button) {
  // ------------------------------------------------------------
  Params_RelativePointerEventCallback params(button,
                                             0,
                                             0,
                                             PointingButton::NONE,
                                             false);
  queue_.push_back(new Item(params));
}

int KeyboardRepeat::primitive_start(AutogenId autogenId, int delayUntilRepeat, int keyRepeat) {
  keyRepeat_ = keyRepeat;
  autogenId_ = autogenId;
  fire_timer_.setTimeoutMS(delayUntilRepeat);

  return succID();
}

void KeyboardRepeat::set(EventType eventType,
                         Flags flags,
                         KeyCode key,
                         KeyboardType keyboardType,
                         AutogenId autogenId,
                         int delayUntilRepeat,
                         int keyRepeat) {
  if (key == KeyCode::VK_NONE) return;

  if (eventType == EventType::MODIFY) {
    goto cancel;

  } else if (eventType == EventType::UP) {
    // The repetition of multiple keys is controlled by manual operation.
    // So, we ignore it.
    if (queue_.size() != 1) return;

    // We stop key repeat only when the repeating key is up.
    KeyboardRepeat::Item* p = static_cast<KeyboardRepeat::Item*>(queue_.safe_front());
    if (p) {
      auto params = (p->getParamsBase()).get_Params_KeyboardEventCallBack();
      if (params && key == params->key) {
        goto cancel;
      }
    }

  } else if (eventType == EventType::DOWN) {
    cancel();

    primitive_add(eventType, flags, key, keyboardType);
    primitive_start(autogenId, delayUntilRepeat, keyRepeat);

    IOLOG_DEVEL("KeyboardRepeat::set key:%d flags:0x%x\n", key.get(), flags.get());

  } else {
    goto cancel;
  }

  return;

cancel:
  cancel();
}

void KeyboardRepeat::set(EventType eventType,
                         Flags flags,
                         ConsumerKeyCode key,
                         AutogenId autogenId,
                         int delayUntilRepeat,
                         int keyRepeat) {
  if (key == ConsumerKeyCode::VK_NONE) return;

  if (eventType == EventType::UP) {
    goto cancel;

  } else if (eventType == EventType::DOWN) {
    if (!key.isRepeatable()) {
      goto cancel;
    }

    cancel();

    primitive_add(eventType, flags, key);
    primitive_start(autogenId, delayUntilRepeat, keyRepeat);

    IOLOG_DEVEL("KeyboardRepeat::set consumer key:%d flags:0x%x\n", key.get(), flags.get());

  } else {
    goto cancel;
  }

  return;

cancel:
  cancel();
}

void KeyboardRepeat::fire_timer_callback(OSObject* owner, IOTimerEventSource* sender) {
  IOLOG_DEVEL("KeyboardRepeat::fire queue_.size = %d\n", static_cast<int>(queue_.size()));

  // ----------------------------------------
  for (KeyboardRepeat::Item* p = static_cast<KeyboardRepeat::Item*>(queue_.safe_front()); p; p = static_cast<KeyboardRepeat::Item*>(p->getnext())) {
    {
      {
        auto params = (p->getParamsBase()).get_Params_KeyboardEventCallBack();
        if (params) {
          EventOutputQueue::FireKey::fire(
              Params_KeyboardEventCallBack(
                  params->eventType,
                  params->flags,
                  params->key,
                  params->keyboardType,
                  queue_.size() == 1 ? true : false),
              autogenId_,
              PhysicalEventType::DOWN);
        }
      }

      {
        auto params = (p->getParamsBase()).get_Params_KeyboardSpecialEventCallback();
        if (params) {
          EventOutputQueue::FireConsumer::fire(
              Params_KeyboardSpecialEventCallback(
                  params->eventType,
                  params->flags,
                  params->key,
                  queue_.size() == 1 ? true : false),
              autogenId_,
              PhysicalEventType::DOWN);
        }
      }

      {
        auto params = (p->getParamsBase()).get_Params_RelativePointerEventCallback();
        if (params) {
          EventOutputQueue::FireRelativePointer::fire(autogenId_, PhysicalEventType::DOWN, params->buttons, params->dx, params->dy);
        }
      }
    }
  }

  fire_timer_.setTimeoutMS(keyRepeat_);
}
}
