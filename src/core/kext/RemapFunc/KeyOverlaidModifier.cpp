#include <IOKit/IOLib.h>

#include "../VirtualKey/VK_LAZY.hpp"
#include "Config.hpp"
#include "IOLogWrapper.hpp"
#include "KeyOverlaidModifier.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace RemapFunc {
    KeyOverlaidModifier::KeyOverlaidModifier(void) :
      isUseSeparator_(false),
      index_is_holding_(true),
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
              if (! isUseSeparator_ && index_ >= 2) {
                index_is_holding_ = false;
              }

              if (index_is_holding_) {
                dppkeytokey_.add(DependingPressingPeriodKeyToKey::KeyToKeyType::FROM, KeyCode::VK_NONE);
                dppkeytokey_.add(DependingPressingPeriodKeyToKey::KeyToKeyType::LONG_PERIOD, datatype, newval);
              } else {
                dppkeytokey_.add(DependingPressingPeriodKeyToKey::KeyToKeyType::SHORT_PERIOD,             datatype, newval);
                dppkeytokey_.add(DependingPressingPeriodKeyToKey::KeyToKeyType::LONG_LONG_PERIOD,         datatype, newval);
                dppkeytokey_.add(DependingPressingPeriodKeyToKey::KeyToKeyType::PRESSING_TARGET_KEY_ONLY, datatype, newval);
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
                  fromEvent_.getModifierFlag() == ModifierFlag(newval) &&
                  fromEvent_.getModifierFlag() != ModifierFlag::NONE) {
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
              if (index_is_holding_) {
                dppkeytokey_.add(DependingPressingPeriodKeyToKey::KeyToKeyType::LONG_PERIOD, datatype, newval);
              } else {
                dppkeytokey_.add(DependingPressingPeriodKeyToKey::KeyToKeyType::SHORT_PERIOD,             datatype, newval);
                dppkeytokey_.add(DependingPressingPeriodKeyToKey::KeyToKeyType::LONG_LONG_PERIOD,         datatype, newval);
                dppkeytokey_.add(DependingPressingPeriodKeyToKey::KeyToKeyType::PRESSING_TARGET_KEY_ONLY, datatype, newval);
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
          } else if (Option::USE_SEPARATOR == option) {
            isUseSeparator_ = true;
          } else if (Option::SEPARATOR == option) {
            if (index_ >= 2) {
              index_is_holding_ = false;
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
