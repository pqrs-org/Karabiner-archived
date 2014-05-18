#include "KeyCodeModifierFlagPairs.hpp"
#include "FlagStatus.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  KeyCodeModifierFlagPairs::Vector_Pair KeyCodeModifierFlagPairs::pairs_;

  void
  KeyCodeModifierFlagPairs::clearVirtualModifiers(void)
  {
    pairs_.clear();

    // Register normal modifiers.

#define REGISTER_MODIFIER(NAME)                                           \
  pairs_.push_back(Pair(ModifierFlag::NAME,                               \
                        KeyCode::NAME,                                    \
                        KeyCode::VK_LOCK_ ## NAME,                        \
                        KeyCode::VK_LOCK_ ## NAME ## _FORCE_ON,           \
                        KeyCode::VK_LOCK_ ## NAME ## _FORCE_OFF,          \
                        KeyCode::VK_NEGATIVE_LOCK_ ## NAME,               \
                        KeyCode::VK_NEGATIVE_LOCK_ ## NAME ## _FORCE_ON,  \
                        KeyCode::VK_NEGATIVE_LOCK_ ## NAME ## _FORCE_OFF, \
                        KeyCode::VK_STICKY_ ## NAME,                      \
                        KeyCode::VK_STICKY_ ## NAME ## _FORCE_ON,         \
                        KeyCode::VK_STICKY_ ## NAME ## _FORCE_OFF));      \

    REGISTER_MODIFIER(CAPSLOCK);
    REGISTER_MODIFIER(COMMAND_L);
    REGISTER_MODIFIER(COMMAND_R);
    REGISTER_MODIFIER(CONTROL_L);
    REGISTER_MODIFIER(CONTROL_R);
    REGISTER_MODIFIER(FN);
    REGISTER_MODIFIER(OPTION_L);
    REGISTER_MODIFIER(OPTION_R);
    REGISTER_MODIFIER(SHIFT_L);
    REGISTER_MODIFIER(SHIFT_R);
#undef REGISTER_MODIFIER

    FlagStatus::globalFlagStatus().initialize();
  }

  void
  KeyCodeModifierFlagPairs::registerVirtualModifier(ModifierFlag m,
                                                    KeyCode k,
                                                    KeyCode vk_lock,
                                                    KeyCode vk_lock_force_on,
                                                    KeyCode vk_lock_force_off,
                                                    KeyCode vk_negative_lock,
                                                    KeyCode vk_negative_lock_force_on,
                                                    KeyCode vk_negative_lock_force_off,
                                                    KeyCode vk_sticky,
                                                    KeyCode vk_sticky_force_on,
                                                    KeyCode vk_sticky_force_off)
  {
    pairs_.push_back(Pair(m,
                          k,
                          vk_lock,
                          vk_lock_force_on,
                          vk_lock_force_off,
                          vk_negative_lock,
                          vk_negative_lock_force_on,
                          vk_negative_lock_force_off,
                          vk_sticky,
                          vk_sticky_force_on,
                          vk_sticky_force_off));

    FlagStatus::globalFlagStatus().initialize();
  }
}
