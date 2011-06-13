#include "base.hpp"
#include "CommonData.hpp"
#include "Config.hpp"
#include "EventOutputQueue.hpp"
#include "FlagStatus.hpp"
#include "VirtualKey.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  void
  VirtualKey::initialize(IOWorkLoop& workloop)
  {
    VirtualKey::VK_MOUSEKEY::initialize(workloop);
    VirtualKey::VK_JIS_TEMPORARY::initialize(workloop);

    VirtualKey::VK_CONFIG::initialize();
  }

  void
  VirtualKey::terminate(void)
  {
    VirtualKey::VK_MOUSEKEY::terminate();
    VirtualKey::VK_JIS_TEMPORARY::terminate();

    VirtualKey::VK_CONFIG::terminate();
  }

  void
  VirtualKey::reset(void)
  {
    VirtualKey::VK_MOUSEKEY::reset();
  }

  bool
  VirtualKey::isKeyLikeModifier(KeyCode keycode)
  {
    if (VirtualKey::VK_CONFIG::is_VK_CONFIG_SYNC_KEYDOWNUP(keycode)) return true;
    if (VirtualKey::VK_LAZY::getModifierFlag(keycode) != ModifierFlag::NONE) return true;
    if (VirtualKey::VK_MOUSEKEY::is_VK_MOUSEKEY(keycode)) return true;
    return false;
  }
}
