#ifndef KEYCODEMODIFIERFLAGPAIRS_HPP
#define KEYCODEMODIFIERFLAGPAIRS_HPP

#include "KeyCode.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  class KeyCodeModifierFlagPairs {
  public:
    class KeyCodeType {
    public:
      enum Value {
        KEYCODE,
        VK_LOCK,
        VK_LOCK_FORCE_ON,
        VK_LOCK_FORCE_OFF,
        VK_STICKY,
        VK_STICKY_FORCE_ON,
        VK_STICKY_FORCE_OFF,
        __END__,
      };
    };

    class Pair {
    public:
      Pair(void) {}
      Pair(ModifierFlag m,
           KeyCode k,
           KeyCode vk_lock,
           KeyCode vk_lock_force_on,
           KeyCode vk_lock_force_off,
           KeyCode vk_sticky,
           KeyCode vk_sticky_force_on,
           KeyCode vk_sticky_force_off) :
        modifierFlag_(m)
      {
        keyCode_[KeyCodeType::KEYCODE]             = k;
        keyCode_[KeyCodeType::VK_LOCK]             = vk_lock;
        keyCode_[KeyCodeType::VK_LOCK_FORCE_ON]    = vk_lock_force_on;
        keyCode_[KeyCodeType::VK_LOCK_FORCE_OFF]   = vk_lock_force_off;
        keyCode_[KeyCodeType::VK_STICKY]           = vk_sticky;
        keyCode_[KeyCodeType::VK_STICKY_FORCE_ON]  = vk_sticky_force_on;
        keyCode_[KeyCodeType::VK_STICKY_FORCE_OFF] = vk_sticky_force_off;
      }
      ModifierFlag getModifierFlag(void) const { return modifierFlag_; }
      KeyCode getKeyCode(KeyCodeType::Value type) const {
        if (type == KeyCodeType::__END__) {
          return KeyCode::VK_NONE;
        }
        return keyCode_[type];
      }

    private:
      ModifierFlag modifierFlag_;
      KeyCode keyCode_[KeyCodeType::__END__];
    };
    DECLARE_VECTOR(Pair);

    static void initialize(void) {
      clearVirtualModifiers();
    }

    static void clearVirtualModifiers(void);
    static void registerVirtualModifier(ModifierFlag m,
                                        KeyCode k,
                                        KeyCode vk_lock,
                                        KeyCode vk_lock_force_on,
                                        KeyCode vk_lock_force_off,
                                        KeyCode vk_sticky,
                                        KeyCode vk_sticky_force_on,
                                        KeyCode vk_sticky_force_off);

    static const Vector_Pair& getPairs(void) { return pairs_; }

    static KeyCode getKeyCode(ModifierFlag m, KeyCodeType::Value type) {
      for (size_t i = 0; i < pairs_.size(); ++i) {
        if (pairs_[i].getModifierFlag() == m) {
          return pairs_[i].getKeyCode(type);
        }
      }
      return KeyCode::VK_NONE;
    }

    static ModifierFlag getModifierFlag(KeyCode k, KeyCodeType::Value type) {
      for (size_t i = 0; i < pairs_.size(); ++i) {
        if (pairs_[i].getKeyCode(type) == k) {
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
