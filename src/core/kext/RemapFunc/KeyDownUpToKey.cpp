#include "IOLogWrapper.hpp"
#include "KeyDownUpToKey.hpp"
#include "RemapClass.hpp"

namespace org_pqrs_Karabiner {
namespace RemapFunc {

void KeyDownUpToKey::add(AddDataType datatype, AddValue newval) {
  switch (datatype) {
  case BRIDGE_DATATYPE_KEYCODE:
  case BRIDGE_DATATYPE_CONSUMERKEYCODE:
  case BRIDGE_DATATYPE_POINTINGBUTTON:
    switch (indexType_) {
    case INDEXTYPE_NONE:
      indexType_ = INDEXTYPE_FROM;
      keytokey_from_.add(datatype, newval);
      keytokey_downup_.add(KeyCode::VK_PSEUDO_KEY);
      keytokey_interrupted_.add(KeyCode::VK_PSEUDO_KEY);
      keytokey_interrupted_.add(datatype, newval);
      break;

    case INDEXTYPE_FROM:
    case INDEXTYPE_DOWNUP:
      indexType_ = INDEXTYPE_DOWNUP;
      keytokey_downup_.add(datatype, newval);
      break;

    case INDEXTYPE_INTERRUPTED:
      if (needClearInterruptedToKeys_) {
        needClearInterruptedToKeys_ = false;
        keytokey_interrupted_.clearToKeys();
      }
      keytokey_interrupted_.add(datatype, newval);
      break;
    }
    break;

  case BRIDGE_DATATYPE_MODIFIERFLAG:
  case BRIDGE_DATATYPE_MODIFIERFLAGS_END:
    switch (indexType_) {
    case INDEXTYPE_NONE:
      IOLOG_ERROR("Invalid KeyDownUpToKey::add\n");
      break;

    case INDEXTYPE_FROM:
      keytokey_from_.add(datatype, newval);
      keytokey_downup_.add(datatype, newval);
      keytokey_interrupted_.add(datatype, newval);
      break;

    case INDEXTYPE_DOWNUP:
      keytokey_downup_.add(datatype, newval);
      break;

    case INDEXTYPE_INTERRUPTED:
      keytokey_interrupted_.add(datatype, newval);
      break;
    }
    break;

  case BRIDGE_DATATYPE_OPTION: {
    Option option(newval);
    if (Option::SEPARATOR == option) {
      indexType_ = INDEXTYPE_INTERRUPTED;
      needClearInterruptedToKeys_ = true;
    } else if (Option::USE_SEPARATOR == option) {
      // do nothing
    } else {
      IOLOG_ERROR("KeyDownUpToKey::add unknown option:%u\n", static_cast<unsigned int>(newval));
    }
    break;
  }

  default:
    IOLOG_ERROR("KeyDownUpToKey::add invalid datatype:%u\n", static_cast<unsigned int>(datatype));
    break;
  }
}

void KeyDownUpToKey::prepare(RemapParams& remapParams) {
  bool iskeydown = false;
  if (remapParams.paramsBase.iskeydown(iskeydown)) {
    if (keytokey_from_.remap(remapParams)) {
      keytokey_downup_.call_remap_with_VK_PSEUDO_KEY(EventType::DOWN, remapParams.physicalEventType);
      keytokey_downup_.call_remap_with_VK_PSEUDO_KEY(EventType::UP, remapParams.physicalEventType);
    } else {
      keytokey_interrupted_.call_remap_with_VK_PSEUDO_KEY(EventType::DOWN, remapParams.physicalEventType);
      interrupted_ = true;
    }
    RemapClassManager::unregisterPrepareTargetItem(this);
  }
}

bool KeyDownUpToKey::remap(RemapParams& remapParams) {
  if (!keytokey_from_.remap(remapParams)) return false;

  if (interrupted_) {
    interrupted_ = false;
    keytokey_interrupted_.call_remap_with_VK_PSEUDO_KEY(EventType::UP, remapParams.physicalEventType);
  }

  if (keytokey_from_.isPressing()) {
    RemapClassManager::registerPrepareTargetItem(this);
    interrupted_ = false;
  }

  return true;
}
}
}
