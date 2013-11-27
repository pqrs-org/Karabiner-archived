#include <IOKit/IOLib.h>

#include "EventOutputQueue.hpp"
#include "VK_PARTIAL.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  List* VirtualKey::VK_PARTIAL::changedKeyCodes_ = NULL;
  bool VirtualKey::VK_PARTIAL::active_ = false;
  EventType VirtualKey::VK_PARTIAL::eventType_;

  void
  VirtualKey::VK_PARTIAL::initialize(void)
  {
    changedKeyCodes_ = new List();
  }

  void
  VirtualKey::VK_PARTIAL::terminate(void)
  {
    delete changedKeyCodes_;
  }

  bool
  VirtualKey::VK_PARTIAL::handle(const Params_KeyboardEventCallBack& params)
  {
    // ------------------------------------------------------------
    // Process VK_PARTIAL
    if (params.key == KeyCode::VK_PARTIAL_KEYDOWN ||
        params.key == KeyCode::VK_PARTIAL_KEYUP) {
      if (params.ex_iskeydown) {
        active_ = true;

        if (params.key == KeyCode::VK_PARTIAL_KEYDOWN) {
          eventType_ = EventType::DOWN;
        } else {
          eventType_ = EventType::UP;
        }
      }
      return true;
    }

    // ------------------------------------------------------------
    // Process target key.
    if (active_ && params.ex_iskeydown) {
      active_ = false;

      Params_KeyboardEventCallBack::auto_ptr ptr(Params_KeyboardEventCallBack::alloc(eventType_,
                                                                                     params.flags,
                                                                                     params.key,
                                                                                     params.charCode,
                                                                                     params.charSet,
                                                                                     params.origCharCode,
                                                                                     params.origCharSet,
                                                                                     params.keyboardType,
                                                                                     params.repeat));
      if (ptr) {
        EventOutputQueue::FireKey::fire(*ptr);
      }

      // We need to register a key for changedKeyCodes_ after EventOutputQueue::FireKey::fire
      // because changedKeyCodes_ blocks key events.
      if (changedKeyCodes_) {
        changedKeyCodes_->push_back(new Item(params.key));
      }

      return true;
    }

    // ------------------------------------------------------------
    // Ignore processed target key until it will be KeyUp.
    if (changedKeyCodes_) {
      for (Item* p = static_cast<Item*>(changedKeyCodes_->front()); p; p = static_cast<Item*>(p->getnext())) {
        if (p->key == params.key) {
          if (! params.ex_iskeydown) {
            changedKeyCodes_->erase_and_delete(p);
          }
          return true;
        }
      }
    }

    // ------------------------------------------------------------
    return false;
  }
}
