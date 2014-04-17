#include "KeyCodeModifierFlagPairs.hpp"
#include "FlagStatus.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  KeyCodeModifierFlagPairs::Vector_Pair KeyCodeModifierFlagPairs::pairs_;

  void
  KeyCodeModifierFlagPairs::clearVirtualModifiers(void)
  {
    pairs_.clear();

    // Register normal modifiers.
    pairs_.push_back(Pair(KeyCode::CAPSLOCK,           ModifierFlag::CAPSLOCK));
    pairs_.push_back(Pair(KeyCode::SHIFT_L,            ModifierFlag::SHIFT_L));
    pairs_.push_back(Pair(KeyCode::SHIFT_R,            ModifierFlag::SHIFT_R));
    pairs_.push_back(Pair(KeyCode::CONTROL_L,          ModifierFlag::CONTROL_L));
    pairs_.push_back(Pair(KeyCode::CONTROL_R,          ModifierFlag::CONTROL_R));
    pairs_.push_back(Pair(KeyCode::OPTION_L,           ModifierFlag::OPTION_L));
    pairs_.push_back(Pair(KeyCode::OPTION_R,           ModifierFlag::OPTION_R));
    pairs_.push_back(Pair(KeyCode::COMMAND_L,          ModifierFlag::COMMAND_L));
    pairs_.push_back(Pair(KeyCode::COMMAND_R,          ModifierFlag::COMMAND_R));
    pairs_.push_back(Pair(KeyCode::FN,                 ModifierFlag::FN));

    pairs_.push_back(Pair(KeyCode::VK_MODIFIER_EXTRA1, ModifierFlag::EXTRA1));
    pairs_.push_back(Pair(KeyCode::VK_MODIFIER_EXTRA2, ModifierFlag::EXTRA2));
    pairs_.push_back(Pair(KeyCode::VK_MODIFIER_EXTRA3, ModifierFlag::EXTRA3));
    pairs_.push_back(Pair(KeyCode::VK_MODIFIER_EXTRA4, ModifierFlag::EXTRA4));
    pairs_.push_back(Pair(KeyCode::VK_MODIFIER_EXTRA5, ModifierFlag::EXTRA5));

    FlagStatus::globalFlagStatus().initialize();
  }

  void
  KeyCodeModifierFlagPairs::registerVirtualModifier(KeyCode k, ModifierFlag m)
  {
    pairs_.push_back(Pair(k, m));

    FlagStatus::globalFlagStatus().initialize();
  }
}
