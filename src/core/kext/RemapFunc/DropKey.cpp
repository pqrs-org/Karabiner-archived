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
    dropTargetAutogenId_ = AutogenId(0);
    return false;
  } else {
    modifierMatched_ = FlagStatus::globalFlagStatus().isOn(fromModifierFlags_);
    dropTargetAutogenId_ = AutogenId(EventOutputQueue::getLastPushedSerialNumber());
    return true;
  }
}

void DropKey::cancelEventOutputQueueItems(void) {
  if (dropTargetAutogenId_ == 0) {
    return;
  }

  for (EventOutputQueue::Item* p = static_cast<EventOutputQueue::Item*>(EventOutputQueue::getQueue().safe_front());
       p;
       p = static_cast<EventOutputQueue::Item*>(p->getnext())) {
    if (EventInputQueue::currentSerialNumber() != p->getEventInputQueueSerialNumber()) {
      continue;
    }
    if (dropTargetAutogenId_ >= p->getAutogenId()) {
      continue;
    }

    auto& paramsBase = p->getParamsBase();

    // Do not drop any modifier flags.
    if (paramsBase.isModifier()) continue;

    {
      auto params = paramsBase.get_Params_KeyboardEventCallBack();
      if (params && dropKey_) {
        dropKey(*p);
      }
    }
    {
      auto params = paramsBase.get_Params_KeyboardSpecialEventCallback();
      if (params && dropConsumerKey_) {
        dropKey(*p);
      }
    }
    {
      auto params = paramsBase.get_Params_RelativePointerEventCallback();
      if (params && dropPointingButton_) {
        if (modifierMatched_ &&
            !(params->buttons).isNONE()) {
          p->cancel();
          return;
        }
      }
    }
  }
  dropTargetAutogenId_ = AutogenId(0);
}

void DropKey::dropKey(EventOutputQueue::Item& item) {
  bool iskeydown = false;
  if (item.getParamsBase().iskeydown(iskeydown)) {
    if (iskeydown) {
      if (modifierMatched_) {
        dropped_.push_back(new Item(item.getParamsBase()));
        item.cancel();
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
        item.cancel();
      }
    }
  }
}
}
}
