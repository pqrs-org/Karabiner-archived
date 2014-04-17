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

    static void initialize(void) {
      clearVirtualModifiers();
    }

    static void clearVirtualModifiers(void);
    static void registerVirtualModifier(KeyCode k, ModifierFlag m);

    static const Vector_Pair& getPairs(void) { return pairs_; }

    static KeyCode getKeyCode(ModifierFlag m) {
      for (size_t i = 0; i < pairs_.size(); ++i) {
        if (pairs_[i].getModifierFlag() == m) {
          return pairs_[i].getKeyCode();
        }
      }
      return KeyCode::VK_NONE;
    }

    static ModifierFlag getModifierFlag(KeyCode k) {
      for (size_t i = 0; i < pairs_.size(); ++i) {
        if (pairs_[i].getKeyCode() == k) {
          return pairs_[i].getModifierFlag();
        }
      }
      return ModifierFlag::ZERO;
    }

  private:
    static Vector_Pair pairs_;
  };
}

#endif
