#include "ModifierName.hpp"

namespace org_pqrs_Karabiner {
ModifierName::Vector_Item ModifierName::items_;

void ModifierName::clearVirtualModifiers(void) {
  items_.clear();

  // Register normal modifiers.
  items_.push_back(Item(ModifierFlag::CAPSLOCK, "caps"));
  items_.push_back(Item(ModifierFlag::SHIFT_L, "shift"));
  items_.push_back(Item(ModifierFlag::SHIFT_R, "shift"));
  items_.push_back(Item(ModifierFlag::CONTROL_L, "control"));
  items_.push_back(Item(ModifierFlag::CONTROL_R, "control"));
  items_.push_back(Item(ModifierFlag::OPTION_L, "option"));
  items_.push_back(Item(ModifierFlag::OPTION_R, "option"));
  items_.push_back(Item(ModifierFlag::COMMAND_L, "command"));
  items_.push_back(Item(ModifierFlag::COMMAND_R, "command"));
  items_.push_back(Item(ModifierFlag::FN, "fn"));
}

const char*
ModifierName::getName(ModifierFlag modifierFlag) {
  for (size_t i = 0; i < items_.size(); ++i) {
    if (modifierFlag == items_[i].getModifierFlag()) {
      return items_[i].getName();
    }
  }
  return nullptr;
}
}
