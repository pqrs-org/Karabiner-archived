#include <IOKit/IOLib.h>

#include "DropKey.hpp"
#include "EventOutputQueue.hpp"
#include "IOLogWrapper.hpp"

namespace org_pqrs_Karabiner {
namespace RemapFunc {
void DropKey::add(AddDataType datatype, AddValue newval) {
  switch (datatype) {
  case BRIDGE_DATATYPE_MODIFIERFLAG: {
    ModifierFlag modifierFlag(newval);
    if (modifierFlag != ModifierFlag::ZERO &&
        modifierFlag != ModifierFlag::NONE) {
      fromModifierFlags_.push_back(ModifierFlag(datatype, newval));
    }
    break;
  }

  case BRIDGE_DATATYPE_MODIFIERFLAGS_END:
    // ignore
    break;

  case BRIDGE_DATATYPE_OPTION: {
    Option option(newval);
    if (Option::DROPKEY_DROP_KEY == option) {
      dropKey_ = true;
    } else if (Option::DROPKEY_DROP_CONSUMERKEY == option) {
      dropConsumerKey_ = true;
    } else if (Option::DROPKEY_DROP_POINTINGBUTTON == option) {
      dropPointingButton_ = true;
    } else {
      IOLOG_ERROR("DropKey::add unknown option:%u\n", static_cast<unsigned int>(newval));
    }
    break;
  }

  default:
    IOLOG_ERROR("DropKey::add invalid datatype:%u\n", static_cast<unsigned int>(datatype));
    break;
  }
}

bool DropKey::remap(RemapParams& remapParams) {
  if (fromModifierFlags_.empty()) {
    IOLOG_WARN("Ignore __DropKey__ with no ModifierFlag.\n");

    modifierMatched_ = false;
    eventOutputQueueSerialNumber_ = 0;
    return false;
  } else {
    modifierMatched_ = FlagStatus::globalFlagStatus().isOn(fromModifierFlags_);
    eventOutputQueueSerialNumber_ = EventOutputQueue::getLastPushedSerialNumber() + 1;
    return true;
  }
}

void DropKey::settle(RemapParams& remapParams) {
  if (eventOutputQueueSerialNumber_ == 0) {
    return;
  }

  while (true) {
    auto p = EventOutputQueue::getItem(eventOutputQueueSerialNumber_);
    if (!p) {
      break;
    }
    ++eventOutputQueueSerialNumber_;

    // Do not drop any modifier flags.
    if ((p->getParamsBase()).isModifier()) continue;

    {
      auto params = (p->getParamsBase()).get_Params_KeyboardEventCallBack();
      if (params && dropKey_) {
        dropKey(*p);
      }
    }
    {
      auto params = (p->getParamsBase()).get_Params_KeyboardSpecialEventCallback();
      if (params && dropConsumerKey_) {
        dropKey(*p);
      }
    }
    {
      auto params = (p->getParamsBase()).get_Params_RelativePointerEventCallback();
      if (params && dropPointingButton_) {
        if (modifierMatched_ &&
            !(params->buttons).isNONE()) {
          remapParams.isremapped = true;
          return;
        }
      }
    }
  }
  eventOutputQueueSerialNumber_ = 0;
}

void DropKey::dropKey(EventOutputQueue::Item& item) {
  bool iskeydown = false;
  if (item.getParamsBase().iskeydown(iskeydown)) {
    if (iskeydown) {
      if (FlagStatus::globalFlagStatus().isOn(fromModifierFlags_)) {
        dropped_.push_back(new Item(item.getParamsBase()));
        goto drop;
      }
    } else {
      bool found = false;
      Item* p = static_cast<Item*>(dropped_.safe_front());
      while (p) {
        if ((p->fromEvent).isTargetEvent(item.getParamsBase())) {
          found = true;
          p = static_cast<Item*>(dropped_.erase_and_delete(p));
        } else {
          p = static_cast<Item*>(p->getnext());
        }
      }
      if (found) {
        goto drop;
      }
    }
  }

  return;

drop:
  item.cancel();
}
}
}
