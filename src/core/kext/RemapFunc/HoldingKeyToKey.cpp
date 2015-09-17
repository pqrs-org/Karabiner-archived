#include "diagnostic_macros.hpp"

BEGIN_IOKIT_INCLUDE;
#include <IOKit/IOLib.h>
END_IOKIT_INCLUDE;

#include "HoldingKeyToKey.hpp"
#include "IOLogWrapper.hpp"

namespace org_pqrs_Karabiner {
namespace RemapFunc {

void HoldingKeyToKey::add(AddDataType datatype, AddValue newval) {
  switch (datatype) {
  case BRIDGE_DATATYPE_KEYCODE:
  case BRIDGE_DATATYPE_CONSUMERKEYCODE:
  case BRIDGE_DATATYPE_POINTINGBUTTON: {
    switch (index_) {
    case 0:
      fromEvent_ = FromEvent(datatype, newval);
      dppkeytokey_.setFromEvent(fromEvent_);
      break;

    default:
      if (!isUseSeparator_ &&
          datatype == BRIDGE_DATATYPE_KEYCODE &&
          KeyCode::VK_NONE == KeyCode(newval) &&
          indexType_ == INDEX_IS_NORMAL) {
        indexType_ = INDEX_IS_HOLDING;
      } else {
        addToDependingPressingPeriodKeyToKey(datatype, newval);
      }
      break;
    }
    ++index_;

    break;
  }

  case BRIDGE_DATATYPE_MODIFIERFLAG:
  case BRIDGE_DATATYPE_MODIFIERFLAGS_END:
  case BRIDGE_DATATYPE_OPTION:
  case BRIDGE_DATATYPE_DELAYUNTILREPEAT:
  case BRIDGE_DATATYPE_KEYREPEAT: {
    switch (index_) {
    case 0:
      if (datatype == BRIDGE_DATATYPE_OPTION && Option::USE_SEPARATOR == Option(newval)) {
        isUseSeparator_ = true;
      } else {
        IOLOG_ERROR("Invalid HoldingKeyToKey::add\n");
      }
      break;

    case 1: {
      if (datatype == BRIDGE_DATATYPE_MODIFIERFLAG ||
          datatype == BRIDGE_DATATYPE_MODIFIERFLAGS_END) {
        dppkeytokey_.addFromModifierFlags(datatype, newval);
      }
      break;
    }

    default:
      if (datatype == BRIDGE_DATATYPE_OPTION && Option::SEPARATOR == Option(newval)) {
        if (index_ >= 2 &&
            indexType_ == INDEX_IS_NORMAL) {
          indexType_ = INDEX_IS_HOLDING;
        }
      } else if (datatype == BRIDGE_DATATYPE_OPTION && Option::KEYTOKEY_BEFORE_KEYDOWN == Option(newval)) {
        indexType_ = INDEX_IS_KEYTOKEY_BEFORE_KEYDOWN;
        dppkeytokey_.addBeforeAfterKeys(datatype, newval);
      } else if (datatype == BRIDGE_DATATYPE_OPTION && Option::KEYTOKEY_AFTER_KEYUP == Option(newval)) {
        indexType_ = INDEX_IS_KEYTOKEY_AFTER_KEYUP;
        dppkeytokey_.addBeforeAfterKeys(datatype, newval);
      } else {
        addToDependingPressingPeriodKeyToKey(datatype, newval);
      }
      break;
    }
    break;
  }

  case BRIDGE_DATATYPE_THRESHOLDMILLISECOND:
    dppkeytokey_.overwritePeriodMS(DependingPressingPeriodKeyToKey::PeriodMS::Mode::HOLDING_KEY_TO_KEY,
                                   DependingPressingPeriodKeyToKey::PeriodMS::Type::SHORT_PERIOD,
                                   newval);
    break;

  default:
    IOLOG_ERROR("HoldingKeyToKey::add invalid datatype:%u\n", static_cast<unsigned int>(datatype));
    break;
  }
}

void HoldingKeyToKey::addToDependingPressingPeriodKeyToKey(AddDataType datatype, AddValue newval) {
  switch (indexType_) {
  case INDEX_IS_NORMAL:
    dppkeytokey_.add(DependingPressingPeriodKeyToKey::KeyToKeyType::SHORT_PERIOD,
                     datatype, newval);
    break;
  case INDEX_IS_HOLDING:
    dppkeytokey_.add(DependingPressingPeriodKeyToKey::KeyToKeyType::LONG_PERIOD,
                     datatype, newval);
    break;
  case INDEX_IS_KEYTOKEY_BEFORE_KEYDOWN:
  case INDEX_IS_KEYTOKEY_AFTER_KEYUP:
    dppkeytokey_.addBeforeAfterKeys(datatype, newval);
    break;
  }
}
}
}
