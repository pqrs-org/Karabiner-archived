#include "diagnostic_macros.hpp"

BEGIN_IOKIT_INCLUDE;
#include <IOKit/IOLib.h>
END_IOKIT_INCLUDE;

#include <sys/errno.h>

#include "ButtonStatus.hpp"
#include "CommonData.hpp"
#include "Config.hpp"
#include "Core.hpp"
#include "EventInputQueue.hpp"
#include "EventOutputQueue.hpp"
#include "EventWatcher.hpp"
#include "FromEvent.hpp"
#include "GlobalLock.hpp"
#include "IOLogWrapper.hpp"
#include "KeyCodeModifierFlagPairs.hpp"
#include "KeyboardRepeat.hpp"
#include "ListHookedConsumer.hpp"
#include "ListHookedKeyboard.hpp"
#include "ListHookedPointing.hpp"
#include "ModifierName.hpp"
#include "Params.hpp"
#include "PressDownKeys.hpp"
#include "PressingPhysicalKeys.hpp"
#include "RemapClass.hpp"
#include "RemapFunc/KeyToKey.hpp"
#include "RemapFunc/PointingRelativeToScroll.hpp"
#include "RemapFunc/common/DependingPressingPeriodKeyToKey.hpp"
#include "SimultaneousKeyPresses.hpp"
#include "TimerWrapper.hpp"
#include "VirtualKey.hpp"

namespace org_pqrs_Karabiner {
namespace Core {
namespace {
IOWorkLoop* workLoop = nullptr;
}

void start(void) {
  GlobalLock::initialize();
  CommonData::initialize();
  KeyCodeModifierFlagPairs::initialize();
  ModifierName::initialize();
  EventWatcher::initialize();
  PressDownKeys::initialize();
  ButtonStatus::initialize();

  workLoop = IOWorkLoop::workLoop();
  if (!workLoop) {
    IOLOG_ERROR("IOWorkLoop::workLoop failed\n");
  } else {
    ListHookedDevice::initializeAll(*workLoop);

    KeyboardRepeat::initialize(*workLoop);
    EventInputQueue::initialize(*workLoop);
    VirtualKey::initialize(*workLoop);
    EventOutputQueue::initialize(*workLoop);
    RemapFunc::KeyToKey::static_initialize(*workLoop);
    RemapFunc::DependingPressingPeriodKeyToKey::static_initialize(*workLoop);
    RemapFunc::PointingRelativeToScroll::static_initialize(*workLoop);
    ListHookedKeyboard::static_initialize(*workLoop);
    RemapClassManager::initialize(*workLoop);
  }

  Config::sysctl_register();
}

void stop(void) {
  // Destroy global lock.
  // Then, all callbacks and hooked functions become inactive.
  GlobalLock::terminate();

  // ------------------------------------------------------------
  ListHookedDevice::terminateAll();

  // ------------------------------------------------------------
  // call terminate
  Config::sysctl_unregister();

  RemapClassManager::terminate();
  KeyboardRepeat::terminate();
  EventInputQueue::terminate();
  VirtualKey::terminate();
  EventOutputQueue::terminate();
  RemapFunc::KeyToKey::static_terminate();
  RemapFunc::DependingPressingPeriodKeyToKey::static_terminate();
  RemapFunc::PointingRelativeToScroll::static_terminate();
  ListHookedKeyboard::static_terminate();

  if (workLoop) {
    workLoop->release();
    workLoop = nullptr;
  }

  EventWatcher::terminate();
  PressDownKeys::terminate();

  CommonData::terminate();
}

// ======================================================================
bool IOHIKeyboard_gIOMatchedNotification_callback(void* target, void* refCon, IOService* newService, IONotifier* notifier) {
  GlobalLock::ScopedLock lk;
  if (!lk) return false;

  IOLOG_DEBUG("%s newService:%p\n", __FUNCTION__, newService);

  IOHIDevice* device = OSDynamicCast(IOHIKeyboard, newService);
  if (!device) return false;

  ListHookedKeyboard::instance().push_back(new ListHookedKeyboard::Item(device));
  ListHookedConsumer::instance().push_back(new ListHookedConsumer::Item(device));
  return true;
}

bool IOHIKeyboard_gIOTerminatedNotification_callback(void* target, void* refCon, IOService* newService, IONotifier* notifier) {
  GlobalLock::ScopedLock lk;
  if (!lk) return false;

  IOLOG_DEBUG("%s newService:%p\n", __FUNCTION__, newService);

  IOHIDevice* device = OSDynamicCast(IOHIKeyboard, newService);
  if (!device) return false;

  ListHookedKeyboard::instance().erase(device);
  ListHookedConsumer::instance().erase(device);
  return true;
}

bool IOHIPointing_gIOMatchedNotification_callback(void* target, void* refCon, IOService* newService, IONotifier* notifier) {
  GlobalLock::ScopedLock lk;
  if (!lk) return false;

  IOLOG_DEBUG("%s newService:%p\n", __FUNCTION__, newService);

  IOHIDevice* device = OSDynamicCast(IOHIPointing, newService);
  if (!device) return false;

  ListHookedPointing::instance().push_back(new ListHookedPointing::Item(device));
  return true;
}

bool IOHIPointing_gIOTerminatedNotification_callback(void* target, void* refCon, IOService* newService, IONotifier* notifier) {
  GlobalLock::ScopedLock lk;
  if (!lk) return false;

  IOLOG_DEBUG("%s newService:%p\n", __FUNCTION__, newService);

  IOHIDevice* device = OSDynamicCast(IOHIPointing, newService);
  if (!device) return false;

  ListHookedPointing::instance().erase(device);
  return true;
}

// ======================================================================
void remap_KeyboardEventCallback(const Params_Base& paramsBase) {
  auto params = paramsBase.get_Params_KeyboardEventCallBack();
  if (!params) return;

  PhysicalEventType physicalEventType = PhysicalEventType::UP;
  if (params->ex_iskeydown) {
    physicalEventType = PhysicalEventType::DOWN;
  }
  RemapParams remapParams(*params, physicalEventType);

  // Call `prepare` before FlagStatus::set.
  //
  // For example, with this autogen:
  //
  //    <autogen>
  //      __HoldingKeyToKey__
  //      KeyCode::I,
  //      __{ KeyCode::A }__
  //      __{ KeyCode::B }__
  //    </autogen>
  //
  // Hitting `i` and `shift` immediately (`i` down, `shift` down, `i` up, `shift` up),
  // `a` and `shift` should be sent (`a` down, `shift` down, `a` up, `shift` up).
  //
  // If we call prepare after FlagStatus::set,
  // `shift-a` will be sent. It's not intended.

  RemapClassManager::prepare(remapParams);

  FlagStatus::globalFlagStatus().set(params->key, params->flags);

  RemapClassManager::remap(remapParams);

  // ------------------------------------------------------------
  if (!remapParams.isremapped) {
    Params_KeyboardEventCallBack p(params->eventType,
                                   FlagStatus::globalFlagStatus().makeFlags(),
                                   params->key,
                                   params->charCode,
                                   params->charSet,
                                   params->origCharCode,
                                   params->origCharSet,
                                   params->keyboardType,
                                   false);
    KeyboardRepeat::set(p, AutogenId::maxValue());
    EventOutputQueue::FireKey::fire(p, AutogenId::maxValue(), physicalEventType);
  }

  RemapFunc::PointingRelativeToScroll::cancelScroll();
}

void remap_KeyboardSpecialEventCallback(const Params_Base& paramsBase) {
  auto params = paramsBase.get_Params_KeyboardSpecialEventCallback();
  if (!params) return;

  PhysicalEventType physicalEventType = PhysicalEventType::UP;
  if (params->ex_iskeydown) {
    physicalEventType = PhysicalEventType::DOWN;
  }
  RemapParams remapParams(*params, physicalEventType);
  RemapClassManager::prepare(remapParams);
  RemapClassManager::remap(remapParams);

  // ----------------------------------------
  if (!remapParams.isremapped) {
    Params_KeyboardSpecialEventCallback p(params->eventType,
                                          FlagStatus::globalFlagStatus().makeFlags(),
                                          params->key,
                                          params->flavor,
                                          params->guid,
                                          false);
    KeyboardRepeat::set(p, AutogenId::maxValue());
    EventOutputQueue::FireConsumer::fire(p, AutogenId::maxValue(), physicalEventType);
  }

  RemapFunc::PointingRelativeToScroll::cancelScroll();
}

void remap_RelativePointerEventCallback(const Params_Base& paramsBase) {
  auto params = paramsBase.get_Params_RelativePointerEventCallback();
  if (!params) return;

  ButtonStatus::set(params->ex_button, params->ex_isbuttondown);

  PhysicalEventType physicalEventType = PhysicalEventType::UP;
  if (params->ex_button != PointingButton::NONE &&
      params->ex_isbuttondown) {
    physicalEventType = PhysicalEventType::DOWN;
  }
  RemapParams remapParams(*params, physicalEventType);
  RemapClassManager::prepare(remapParams);
  RemapClassManager::remap(remapParams);

  // ------------------------------------------------------------
  if (!remapParams.isremapped) {
    EventOutputQueue::FireRelativePointer::fire(AutogenId::maxValue(), physicalEventType, ButtonStatus::makeButtons(), params->dx, params->dy);
  }
}

void remap_ScrollWheelEventCallback(const Params_Base& paramsBase) {
  auto params = paramsBase.get_Params_ScrollWheelEventCallback();
  if (!params) return;

  PhysicalEventType physicalEventType = PhysicalEventType::DOWN;
  RemapParams remapParams(*params, physicalEventType);
  RemapClassManager::prepare(remapParams);
  RemapClassManager::remap(remapParams);

  if (!remapParams.isremapped) {
    EventOutputQueue::FireScrollWheel::fire(*params, AutogenId::maxValue(), physicalEventType);
    RemapFunc::PointingRelativeToScroll::cancelScroll();
  }
}
}
}
