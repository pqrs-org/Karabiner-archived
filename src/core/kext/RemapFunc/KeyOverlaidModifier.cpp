#include <IOKit/IOLib.h>

#include "../VirtualKey/VK_LAZY.hpp"
#include "Config.hpp"
#include "IOLogWrapper.hpp"
#include "KeyOverlaidModifier.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace RemapFunc {
    KeyOverlaidModifier::KeyOverlaidModifier(void) :
      isUseSeparator_(false),
      indexType_(INDEX_IS_HOLDING),
      index_(0)
    {
      dppkeytokey_.setPeriodMS(DependingPressingPeriodKeyToKey::PeriodMS::Mode::KEY_OVERLAID_MODIFIER);
    }

    KeyOverlaidModifier::~KeyOverlaidModifier(void)
    {}

    void
    KeyOverlaidModifier::add(AddDataType datatype, AddValue newval)
    {
      switch (datatype) {
        case BRIDGE_DATATYPE_KEYCODE:
        case BRIDGE_DATATYPE_CONSUMERKEYCODE:
        case BRIDGE_DATATYPE_POINTINGBUTTON:
        {
          switch (index_) {
            case 0:
              dppkeytokey_.add(DependingPressingPeriodKeyToKey::KeyToKeyType::FROM, datatype, newval);
              dppkeytokey_.add(DependingPressingPeriodKeyToKey::KeyToKeyType::SHORT_PERIOD,             KeyCode::VK_PSEUDO_KEY);
              dppkeytokey_.add(DependingPressingPeriodKeyToKey::KeyToKeyType::LONG_PERIOD,              KeyCode::VK_PSEUDO_KEY);
              dppkeytokey_.add(DependingPressingPeriodKeyToKey::KeyToKeyType::LONG_LONG_PERIOD,         KeyCode::VK_PSEUDO_KEY);
              dppkeytokey_.add(DependingPressingPeriodKeyToKey::KeyToKeyType::PRESSING_TARGET_KEY_ONLY, KeyCode::VK_PSEUDO_KEY);
              fromEvent_ = FromEvent(datatype, newval);
              break;

            default:
              if (! isUseSeparator_ && index_ >= 2 && indexType_ == INDEX_IS_HOLDING) {
                indexType_ = INDEX_IS_NORMAL;
              }

              if (indexType_ == INDEX_IS_HOLDING) {
                dppkeytokey_.add(DependingPressingPeriodKeyToKey::KeyToKeyType::FROM, KeyCode::VK_NONE);
                dppkeytokey_.add(DependingPressingPeriodKeyToKey::KeyToKeyType::LONG_PERIOD, datatype, newval);
              } else if (indexType_ == INDEX_IS_NORMAL) {
                dppkeytokey_.add(DependingPressingPeriodKeyToKey::KeyToKeyType::SHORT_PERIOD,             datatype, newval);
                dppkeytokey_.add(DependingPressingPeriodKeyToKey::KeyToKeyType::LONG_LONG_PERIOD,         datatype, newval);
                dppkeytokey_.add(DependingPressingPeriodKeyToKey::KeyToKeyType::PRESSING_TARGET_KEY_ONLY, datatype, newval);
              } else if (indexType_ == INDEX_IS_REPEAT_TOKEYS) {
                dppkeytokey_.add(DependingPressingPeriodKeyToKey::KeyToKeyType::LONG_LONG_PERIOD, datatype, newval);
              }
              break;
          }
          ++index_;

          break;
        }

        case BRIDGE_DATATYPE_MODIFIERFLAG:
        case BRIDGE_DATATYPE_MODIFIERFLAGS_END:
        {
          switch (index_) {
            case 0:
              IOLOG_ERROR("Invalid KeyOverlaidModifier::add\n");
              break;

            case 1:
            {
              dppkeytokey_.add(DependingPressingPeriodKeyToKey::KeyToKeyType::FROM, datatype, newval);

              bool skip = false;
              if (datatype == BRIDGE_DATATYPE_MODIFIERFLAG &&
                  fromEvent_.getModifierFlag() == ModifierFlag(newval)) {
                skip = true;
              }

              if (! skip) {
                dppkeytokey_.add(DependingPressingPeriodKeyToKey::KeyToKeyType::SHORT_PERIOD,             datatype, newval);
                dppkeytokey_.add(DependingPressingPeriodKeyToKey::KeyToKeyType::LONG_PERIOD,              datatype, newval);
                dppkeytokey_.add(DependingPressingPeriodKeyToKey::KeyToKeyType::LONG_LONG_PERIOD,         datatype, newval);
                dppkeytokey_.add(DependingPressingPeriodKeyToKey::KeyToKeyType::PRESSING_TARGET_KEY_ONLY, datatype, newval);
              }
              break;
            }

            default:
              if (indexType_ == INDEX_IS_HOLDING) {
                dppkeytokey_.add(DependingPressingPeriodKeyToKey::KeyToKeyType::LONG_PERIOD, datatype, newval);
              } else if (indexType_ == INDEX_IS_NORMAL) {
                dppkeytokey_.add(DependingPressingPeriodKeyToKey::KeyToKeyType::SHORT_PERIOD,             datatype, newval);
                dppkeytokey_.add(DependingPressingPeriodKeyToKey::KeyToKeyType::LONG_LONG_PERIOD,         datatype, newval);
                dppkeytokey_.add(DependingPressingPeriodKeyToKey::KeyToKeyType::PRESSING_TARGET_KEY_ONLY, datatype, newval);
              } else if (indexType_ == INDEX_IS_REPEAT_TOKEYS) {
                dppkeytokey_.add(DependingPressingPeriodKeyToKey::KeyToKeyType::LONG_LONG_PERIOD, datatype, newval);
              }
              break;
          }
          break;
        }

        case BRIDGE_DATATYPE_OPTION:
        {
          Option option(newval);
          if (Option::KEYOVERLAIDMODIFIER_REPEAT == option) {
            dppkeytokey_.setPeriodMS(DependingPressingPeriodKeyToKey::PeriodMS::Mode::KEY_OVERLAID_MODIFIER_WITH_REPEAT);
          } else if (Option::KEYOVERLAIDMODIFIER_REPEAT_TOKEYS == option) {
            indexType_ = INDEX_IS_REPEAT_TOKEYS;
            dppkeytokey_.clearToKeys(DependingPressingPeriodKeyToKey::KeyToKeyType::LONG_LONG_PERIOD);
          } else if (Option::USE_SEPARATOR == option) {
            isUseSeparator_ = true;
          } else if (Option::SEPARATOR == option) {
            if (index_ >= 2 && indexType_ == INDEX_IS_HOLDING) {
              indexType_ = INDEX_IS_NORMAL;
            }
          } else if (Option::NOREPEAT == option ||
                     Option::KEYTOKEY_BEFORE_KEYDOWN == option ||
                     Option::KEYTOKEY_AFTER_KEYUP == option) {
            dppkeytokey_.add(DependingPressingPeriodKeyToKey::KeyToKeyType::LONG_PERIOD, datatype, newval);
          } else {
            IOLOG_ERROR("KeyOverlaidModifier::add unknown option:%u\n", static_cast<unsigned int>(newval));
          }
          break;
        }

        case BRIDGE_DATATYPE_DELAYUNTILREPEAT:
        case BRIDGE_DATATYPE_KEYREPEAT:
        {
          dppkeytokey_.add(DependingPressingPeriodKeyToKey::KeyToKeyType::LONG_PERIOD, datatype, newval);
          break;
        }

        default:
          IOLOG_ERROR("KeyOverlaidModifier::add invalid datatype:%u\n", static_cast<unsigned int>(datatype));
          break;
      }
    }

    bool KeyOverlaidModifier::remap(RemapParams& remapParams) { return dppkeytokey_.remap(remapParams); }
  }
}
