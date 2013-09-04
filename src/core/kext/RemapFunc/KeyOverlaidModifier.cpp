#include "Config.hpp"
#include "KeyOverlaidModifier.hpp"
#include "../VirtualKey/VK_LAZY.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace RemapFunc {
    KeyOverlaidModifier::KeyOverlaidModifier(void) :
      isUseSeparator_(false),
      index_is_holding_(true),
      index_(0),
      fromKeyFlag_(ModifierFlag::NONE)
    {
      dppkeytokey_.setPeriodMS(DependingPressingPeriodKeyToKey::PeriodMS::Mode::KEY_OVERLAID_MODIFIER);
    }

    KeyOverlaidModifier::~KeyOverlaidModifier(void)
    {}

    void
    KeyOverlaidModifier::add(unsigned int datatype, unsigned int newval)
    {
      switch (datatype) {
        case BRIDGE_DATATYPE_KEYCODE:
        {
          switch (index_) {
            case 0:
              dppkeytokey_.add(DependingPressingPeriodKeyToKey::KeyToKeyType::FROM, datatype, newval);
              dppkeytokey_.add(DependingPressingPeriodKeyToKey::KeyToKeyType::SHORT_PERIOD,             KeyCode::VK_PSEUDO_KEY);
              dppkeytokey_.add(DependingPressingPeriodKeyToKey::KeyToKeyType::LONG_PERIOD,              KeyCode::VK_PSEUDO_KEY);
              dppkeytokey_.add(DependingPressingPeriodKeyToKey::KeyToKeyType::LONG_LONG_PERIOD,         KeyCode::VK_PSEUDO_KEY);
              dppkeytokey_.add(DependingPressingPeriodKeyToKey::KeyToKeyType::PRESSING_TARGET_KEY_ONLY, KeyCode::VK_PSEUDO_KEY);
              fromKeyFlag_ = KeyCode(newval).getModifierFlag();
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

        case BRIDGE_DATATYPE_FLAGS:
        {
          switch (index_) {
            case 0:
              IOLOG_ERROR("Invalid KeyOverlaidModifier::add\n");
              break;

            case 1:
              dppkeytokey_.add(DependingPressingPeriodKeyToKey::KeyToKeyType::FROM,                     datatype, newval);
              {
                Flags flags(newval);
                if (fromKeyFlag_ != ModifierFlag::NONE) {
                  flags.remove(fromKeyFlag_);
                }
                dppkeytokey_.add(DependingPressingPeriodKeyToKey::KeyToKeyType::SHORT_PERIOD,             flags);
                dppkeytokey_.add(DependingPressingPeriodKeyToKey::KeyToKeyType::LONG_PERIOD,              flags);
                dppkeytokey_.add(DependingPressingPeriodKeyToKey::KeyToKeyType::LONG_LONG_PERIOD,         flags);
                dppkeytokey_.add(DependingPressingPeriodKeyToKey::KeyToKeyType::PRESSING_TARGET_KEY_ONLY, flags);
              }
              break;

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
          if (Option::KEYOVERLAIDMODIFIER_REPEAT == newval) {
            dppkeytokey_.setPeriodMS(DependingPressingPeriodKeyToKey::PeriodMS::Mode::KEY_OVERLAID_MODIFIER_WITH_REPEAT);
          } else if (Option::USE_SEPARATOR == newval) {
            isUseSeparator_ = true;
          } else if (Option::SEPARATOR == newval) {
            if (index_ >= 2) {
              index_is_holding_ = false;
            }
          } else if (Option::NOREPEAT == newval ||
                     Option::KEYTOKEY_BEFORE_KEYDOWN == newval ||
                     Option::KEYTOKEY_AFTER_KEYUP == newval) {
            dppkeytokey_.add(DependingPressingPeriodKeyToKey::KeyToKeyType::LONG_PERIOD, datatype, newval);
          } else {
            IOLOG_ERROR("KeyOverlaidModifier::add unknown option:%d\n", newval);
          }
          break;
        }

        default:
          IOLOG_ERROR("KeyOverlaidModifier::add invalid datatype:%d\n", datatype);
          break;
      }
    }

    bool KeyOverlaidModifier::remap(RemapParams& remapParams)                  { return dppkeytokey_.remap(remapParams); }
    bool KeyOverlaidModifier::remap(RemapConsumerParams& remapParams)          { return dppkeytokey_.remap(remapParams); }
    bool KeyOverlaidModifier::remap(RemapPointingParams_relative& remapParams) { return dppkeytokey_.remap(remapParams); }
    bool KeyOverlaidModifier::remap(RemapPointingParams_scroll& remapParams)   { return dppkeytokey_.remap(remapParams); }
  }
}
