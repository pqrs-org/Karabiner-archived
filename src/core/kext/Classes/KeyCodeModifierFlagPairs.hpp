#ifndef KEYCODEMODIFIERFLAGPAIRS_HPP
#define KEYCODEMODIFIERFLAGPAIRS_HPP

#include "KeyCode.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  class KeyCodeModifierFlagPairs {
  public:
    class Pair {
    public:
      Pair(void) {}
      Pair(KeyCode k, ModifierFlag m) : keyCode_(k), modifierFlag_(m) {}
      KeyCode getKeyCode(void) const { return keyCode_; }
      ModifierFlag getModifierFlag(void) const { return modifierFlag_; }

    private:
      KeyCode keyCode_;
      ModifierFlag modifierFlag_;
    };
    DECLARE_VECTOR(Pair);

    static void clearVirtualModifiers(void) {
      pairs_.clear();

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
    }

    static void registerVirtualModifier(KeyCode k, ModifierFlag m) {
      pairs_.push_back(Pair(k, m));
    }

  private:
    static Vector_Pair pairs_;
  };
}

#endif
