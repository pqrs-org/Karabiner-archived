#include "diagnostic_macros.hpp"

BEGIN_IOKIT_INCLUDE;
#include <IOKit/IOLib.h>
END_IOKIT_INCLUDE;

#include "DropAllKeys.hpp"
#include "EventOutputQueue.hpp"
#include "IOLogWrapper.hpp"

namespace org_pqrs_Karabiner {
namespace RemapFunc {
void DropAllKeys::add(AddDataType datatype, AddValue newval) {
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
    if (Option::DROPALLKEYS_DROP_KEY == option) {
      dropKey_ = true;
    } else if (Option::DROPALLKEYS_DROP_CONSUMERKEY == option) {
      dropConsumerKey_ = true;
    } else if (Option::DROPALLKEYS_DROP_POINTINGBUTTON == option) {
      dropPointingButton_ = true;
    } else {
      IOLOG_ERROR("DropAllKeys::add unknown option:%u\n", static_cast<unsigned int>(newval));
    }
    break;
  }

  default:
    IOLOG_ERROR("DropAllKeys::add invalid datatype:%u\n", static_cast<unsigned int>(datatype));
    break;
  }
}

bool DropAllKeys::remap(RemapParams& remapParams) {
  // We do not drop key events in `remap`.
  // We need to drop key events in `cancelEventOutputQueueItems`.
  //
  // Why:
  // __DropAllKeys__ will be used in mode in order to ignore invalid keys.
  // (eg. enable only hjkl keys in simple vi mode v2.)
  //
  // In this case, we should not drop modifier keys in order to use shift+hjkl keys.
  // Therefore, we do not drop modifier keys.
  // When we enabled "change space to shift", space+hjkl should be shift+hjkl and
  // the shift modifier should not be dropped.
  // If we set `remapParams.remapped` in `remap`,
  // the space key will not be changed to shift key and space+hjkl will be hjkl without modifier.
  //
  // So, we pass key events to other remap functions and drop key events in `cancelEventOutputQueueItems`.

  helper_.registerSerialNumber();

  if (fromModifierFlags_.empty()) {
    IOLOG_WARN("Ignore __DropAllKeys__ with no ModifierFlag.\n");

    modifierMatched_ = false;
    doCancel_ = false;
    return false;
  } else {
    modifierMatched_ = FlagStatus::globalFlagStatus().isOn(fromModifierFlags_);
    doCancel_ = true;
    return true;
  }
}

void DropAllKeys::cancelEventOutputQueueItems(EventOutputQueue::Item& item) {
  if (!helper_.isTarget(item)) {
    return;
  }

  if (!doCancel_) {
    return;
  }

  // Do not cancel events which are pushed before this __DropAllKeys__.
  if (item.getAutogenId() < autogenId_) {
    return;
  }

  auto& paramsBase = item.getParamsBase();

  // Do not drop any modifier flags.
  if (paramsBase.isModifier()) return;

  {
    auto params = paramsBase.get_Params_KeyboardEventCallBack();
    if (params && dropKey_) {
      dropKey(item);
    }
  }
  {
    auto params = paramsBase.get_Params_KeyboardSpecialEventCallback();
    if (params && dropConsumerKey_) {
      dropKey(item);
    }
  }
  {
    auto params = paramsBase.get_Params_RelativePointerEventCallback();
    if (params && dropPointingButton_) {
      if (modifierMatched_ &&
          !(params->buttons).isNONE()) {
        item.cancel();
        return;
      }
    }
  }
}

void DropAllKeys::dropKey(EventOutputQueue::Item& item) {
  bool iskeydown = false;
  if (item.getParamsBase().iskeydown(iskeydown)) {
    if (iskeydown) {
      if (!item.getParamsBase().isRepeat()) {
        if (modifierMatched_) {
          dropped_.push_back(new Item(item.getParamsBase()));
          item.cancel();
        }
      } else {
        bool found = false;
        for (Item* p = static_cast<Item*>(dropped_.safe_front()); p; p = static_cast<Item*>(p->getnext())) {
          if ((p->fromEvent).isTargetEvent(item.getParamsBase())) {
            found = true;
            break;
          }
        }
        if (found) {
          item.cancel();
        }
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
