#include "CommonData.hpp"
#include "Config.hpp"
#include "EventOutputQueue.hpp"
#include "FlagStatus.hpp"
#include "VirtualKey.hpp"
#include "VirtualKey/VK_CONFIG.hpp"
#include "VirtualKey/VK_CONSUMERKEY.hpp"
#include "VirtualKey/VK_DEFINED_IN_USERSPACE.hpp"
#include "VirtualKey/VK_IOHIDPOSTEVENT.hpp"
#include "VirtualKey/VK_LAZY.hpp"
#include "VirtualKey/VK_LOCK.hpp"
#include "VirtualKey/VK_MOUSEKEY.hpp"
#include "VirtualKey/VK_STICKY.hpp"
#include "VirtualKey/VK_PARTIAL.hpp"
#include "VirtualKey/VK_WAIT.hpp"
#include "VirtualKey/VK_JIS_BACKSLASH.hpp"
#include "VirtualKey/VK_JIS_TEMPORARY.hpp"
#include "VirtualKey/VK_JIS_TOGGLE_EISUU_KANA.hpp"
#include "VirtualKey/VK_JIS_YEN.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  void
  VirtualKey::initialize(IOWorkLoop& workloop)
  {
    VirtualKey::VK_MOUSEKEY::initialize(workloop);
    VirtualKey::VK_JIS_TEMPORARY::initialize(workloop);

    VirtualKey::VK_CONFIG::initialize();
    VirtualKey::VK_DEFINED_IN_USERSPACE::initialize();
    VirtualKey::VK_PARTIAL::initialize();
  }

  void
  VirtualKey::terminate(void)
  {
    VirtualKey::VK_MOUSEKEY::terminate();
    VirtualKey::VK_JIS_TEMPORARY::terminate();

    VirtualKey::VK_CONFIG::terminate();
    VirtualKey::VK_DEFINED_IN_USERSPACE::terminate();
    VirtualKey::VK_PARTIAL::terminate();
  }

  void
  VirtualKey::reset(void)
  {
    VirtualKey::VK_MOUSEKEY::reset();
  }

  bool
  VirtualKey::handle(const Params_KeyboardEventCallBack& params)
  {
    if (VirtualKey::VK_CONFIG::handle(params))                { return true; }
    if (VirtualKey::VK_CONSUMERKEY::handle(params))           { return true; }
    if (VirtualKey::VK_DEFINED_IN_USERSPACE::handle(params))  { return true; }
    if (VirtualKey::VK_IOHIDPOSTEVENT::handle(params))        { return true; }
    if (VirtualKey::VK_LAZY::handle(params))                  { return true; }
    if (VirtualKey::VK_LOCK::handle(params))                  { return true; }
    if (VirtualKey::VK_MOUSEKEY::handle(params))              { return true; }
    if (VirtualKey::VK_PARTIAL::handle(params))               { return true; }
    if (VirtualKey::VK_STICKY::handle(params))                { return true; }
    if (VirtualKey::VK_WAIT::handle(params))                  { return true; }
    if (VirtualKey::VK_JIS_BACKSLASH::handle(params))         { return true; }
    if (VirtualKey::VK_JIS_TEMPORARY::handle(params))         { return true; }
    if (VirtualKey::VK_JIS_TOGGLE_EISUU_KANA::handle(params)) { return true; }
    if (VirtualKey::VK_JIS_YEN::handle(params))               { return true; }
    return false;
  }

  bool
  VirtualKey::handleAfterEnqueued(const Params_KeyboardEventCallBack& params)
  {
    if (VirtualKey::VK_DEFINED_IN_USERSPACE::handleAfterEnqueued(params)) { return true; }
    if (VirtualKey::VK_IOHIDPOSTEVENT::handleAfterEnqueued(params))       { return true; }
    return false;
  }

  bool
  VirtualKey::isKeyLikeModifier(KeyCode keycode)
  {
    if (VirtualKey::VK_CONFIG::is_VK_CONFIG_SYNC_KEYDOWNUP(keycode)) return true;
    if (VirtualKey::VK_LAZY::getModifierFlag(keycode) != ModifierFlag::ZERO) return true;
    if (VirtualKey::VK_MOUSEKEY::isKeyLikeModifier(keycode)) return true;
    return false;
  }
}
