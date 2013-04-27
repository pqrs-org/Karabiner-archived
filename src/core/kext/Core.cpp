#include "base.hpp"
#include "ButtonStatus.hpp"
#include "CommonData.hpp"
#include "Config.hpp"
#include "Core.hpp"
#include "RemapClass.hpp"
#include "remap.hpp"
#include "util/CallBackWrapper.hpp"
#include "util/EventInputQueue.hpp"
#include "util/EventOutputQueue.hpp"
#include "util/EventWatcher.hpp"
#include "util/GlobalLock.hpp"
#include "util/KeyboardRepeat.hpp"
#include "util/ListHookedConsumer.hpp"
#include "util/ListHookedKeyboard.hpp"
#include "util/ListHookedPointing.hpp"
#include "util/NumHeldDownKeys.hpp"
#include "util/PressDownKeys.hpp"
#include "util/TimerWrapper.hpp"
#include "RemapFunc/common/DependingPressingPeriodKeyToKey.hpp"
#include "RemapFunc/PointingRelativeToScroll.hpp"
#include "VirtualKey.hpp"

#include <sys/errno.h>
#include <IOKit/IOWorkLoop.h>
#include <IOKit/IOTimerEventSource.h>

namespace org_pqrs_KeyRemap4MacBook {
  namespace Core {
    namespace {
      IOWorkLoop* workLoop = NULL;
    }

    void
    start(void)
    {
      GlobalLock::initialize();
      CommonData::initialize();
      EventWatcher::initialize();
      PressDownKeys::initialize();
      FlagStatus::initialize();
      ButtonStatus::initialize();

      ListHookedDevice::initializeAll();

      workLoop = IOWorkLoop::workLoop();
      if (! workLoop) {
        IOLOG_ERROR("IOWorkLoop::workLoop failed\n");
      } else {
        KeyboardRepeat::initialize(*workLoop);
        EventInputQueue::initialize(*workLoop);
        VirtualKey::initialize(*workLoop);
        EventOutputQueue::initialize(*workLoop);
        RemapFunc::DependingPressingPeriodKeyToKey::static_initialize(*workLoop);
        RemapFunc::PointingRelativeToScroll::static_initialize(*workLoop);
        ListHookedKeyboard::static_initialize(*workLoop);
        RemapClassManager::initialize(*workLoop);
      }

      Config::sysctl_register();
    }

    void
    stop(void)
    {
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
      RemapFunc::DependingPressingPeriodKeyToKey::static_terminate();
      RemapFunc::PointingRelativeToScroll::static_terminate();
      ListHookedKeyboard::static_terminate();

      if (workLoop) {
        workLoop->release();
        workLoop = NULL;
      }

      EventWatcher::terminate();
      PressDownKeys::terminate();

      CommonData::terminate();
    }

    // ======================================================================
    bool
    IOHIKeyboard_gIOMatchedNotification_callback(void* target, void* refCon, IOService* newService, IONotifier* notifier)
    {
      GlobalLock::ScopedLock lk;
      if (! lk) return false;

      IOLOG_DEBUG("%s newService:%p\n", __FUNCTION__, newService);

      IOHIDevice* device = OSDynamicCast(IOHIKeyboard, newService);
      if (! device) return false;

      ListHookedKeyboard::instance().push_back(new ListHookedKeyboard::Item(device));
      ListHookedConsumer::instance().push_back(new ListHookedConsumer::Item(device));
      return true;
    }

    bool
    IOHIKeyboard_gIOTerminatedNotification_callback(void* target, void* refCon, IOService* newService, IONotifier* notifier)
    {
      GlobalLock::ScopedLock lk;
      if (! lk) return false;

      IOLOG_DEBUG("%s newService:%p\n", __FUNCTION__, newService);

      IOHIDevice* device = OSDynamicCast(IOHIKeyboard, newService);
      if (! device) return false;

      ListHookedKeyboard::instance().erase(device);
      ListHookedConsumer::instance().erase(device);
      return true;
    }

    bool
    IOHIPointing_gIOMatchedNotification_callback(void* target, void* refCon, IOService* newService, IONotifier* notifier)
    {
      GlobalLock::ScopedLock lk;
      if (! lk) return false;

      IOLOG_DEBUG("%s newService:%p\n", __FUNCTION__, newService);

      IOHIDevice* device = OSDynamicCast(IOHIPointing, newService);
      if (! device) return false;

      ListHookedPointing::instance().push_back(new ListHookedPointing::Item(device));
      return true;
    }

    bool
    IOHIPointing_gIOTerminatedNotification_callback(void* target, void* refCon, IOService* newService, IONotifier* notifier)
    {
      GlobalLock::ScopedLock lk;
      if (! lk) return false;

      IOLOG_DEBUG("%s newService:%p\n", __FUNCTION__, newService);

      IOHIDevice* device = OSDynamicCast(IOHIPointing, newService);
      if (! device) return false;

      ListHookedPointing::instance().erase(device);
      return true;
    }

    // ======================================================================
    void
    remap_KeyboardEventCallback(Params_KeyboardEventCallBack& params)
    {
      RemapParams remapParams(params);

      // ------------------------------------------------------------
      FlagStatus::set(params.key, params.flags);

      RemapClassManager::remap_key(remapParams);

      // ------------------------------------------------------------
      if (! remapParams.isremapped) {
        Params_KeyboardEventCallBack::auto_ptr ptr(Params_KeyboardEventCallBack::alloc(params.eventType, FlagStatus::makeFlags(), params.key,
                                                                                       params.charCode, params.charSet, params.origCharCode, params.origCharSet,
                                                                                       params.keyboardType, false));
        if (ptr) {
          KeyboardRepeat::set(*ptr);
          EventOutputQueue::FireKey::fire(*ptr);
        }
      }

      if (NumHeldDownKeys::iszero()) {
        NumHeldDownKeys::reset();
        KeyboardRepeat::cancel();
        EventWatcher::reset();
        FlagStatus::reset();
        ButtonStatus::reset();
        VirtualKey::reset();
        EventOutputQueue::FireModifiers::fire(FlagStatus::makeFlags());
        EventOutputQueue::FireRelativePointer::fire();
        PressDownKeys::clear();
      }

      RemapFunc::PointingRelativeToScroll::cancelScroll();
    }

    void
    remap_KeyboardSpecialEventCallback(Params_KeyboardSpecialEventCallback& params)
    {
      RemapConsumerParams remapParams(params);

      // ------------------------------------------------------------
      RemapClassManager::remap_consumer(remapParams);

      if (params.key == ConsumerKeyCode::POWER) {
        CommonData::set_isPowerKeyChanged(remapParams.isremapped);
        if (params.ex_eventOrigin == EventOrigin::USERSPACE) {
          // We need to send a power key event even if it is changed.
          remapParams.isremapped = false;
        }
      }

      // ----------------------------------------
      if (! remapParams.isremapped) {
        Params_KeyboardSpecialEventCallback::auto_ptr ptr(Params_KeyboardSpecialEventCallback::alloc(params.eventType, FlagStatus::makeFlags(), params.key,
                                                                                                     params.flavor, params.guid, false));
        if (ptr) {
          KeyboardRepeat::set(*ptr);
          EventOutputQueue::FireConsumer::fire(*ptr, true);
        }
      }

      RemapFunc::PointingRelativeToScroll::cancelScroll();
    }

    void
    remap_RelativePointerEventCallback(Params_RelativePointerEventCallback& params)
    {
      RemapPointingParams_relative remapParams(params);

      ButtonStatus::set(params.ex_button, params.ex_isbuttondown);

      RemapClassManager::remap_pointing(remapParams);

      // ------------------------------------------------------------
      if (! remapParams.isremapped) {
        EventOutputQueue::FireRelativePointer::fire(ButtonStatus::makeButtons(), params.dx, params.dy);
      }
    }

    void
    remap_ScrollWheelEventCallback(Params_ScrollWheelEventCallback& params)
    {
      RemapPointingParams_scroll remapParams(params);

      RemapClassManager::remap_pointing_scroll(remapParams);

      if (! remapParams.isremapped) {
        EventOutputQueue::FireScrollWheel::fire(params);
        RemapFunc::PointingRelativeToScroll::cancelScroll();
      }
    }
  }
}
