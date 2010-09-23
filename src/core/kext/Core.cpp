#include "base.hpp"
#include "ButtonStatus.hpp"
#include "Client.hpp"
#include "CommonData.hpp"
#include "Config.hpp"
#include "Core.hpp"
#include "RemapClass.hpp"
#include "remap.hpp"
#include "util/CallBackWrapper.hpp"
#include "util/EventInputQueue.hpp"
#include "util/EventOutputQueue.hpp"
#include "util/EventWatcher.hpp"
#include "util/KeyboardRepeat.hpp"
#include "util/ListHookedConsumer.hpp"
#include "util/ListHookedKeyboard.hpp"
#include "util/ListHookedPointing.hpp"
#include "util/NumHeldDownKeys.hpp"
#include "util/PressDownKeys.hpp"
#include "util/TimerWrapper.hpp"
#include "RemapFunc/HoldingKeyToKey.hpp"
#include "RemapFunc/KeyOverlaidModifier.hpp"
#include "RemapFunc/PointingRelativeToScroll.hpp"
#include "VirtualKey.hpp"

#include <sys/errno.h>
#include <IOKit/IOWorkLoop.h>
#include <IOKit/IOTimerEventSource.h>

namespace org_pqrs_KeyRemap4MacBook {
  namespace Core {
    namespace {
      IOWorkLoop* workLoop = NULL;
      TimerWrapper timer_refresh;

      // ------------------------------------------------------------
      enum {
        REFRESH_DEVICE_INTERVAL = 3000,
      };

      void
      refreshHookedDevice(OSObject* owner, IOTimerEventSource* sender)
      {
        IOLockWrapper::ScopedLock lk(timer_refresh.getlock());

        ListHookedKeyboard::instance().refresh_callback();
        ListHookedConsumer::instance().refresh_callback();
        ListHookedPointing::instance().refresh_callback();

        timer_refresh.setTimeoutMS(REFRESH_DEVICE_INTERVAL);
      }
    }

    void
    start(void)
    {
      CommonData::initialize();
      EventWatcher::initialize();
      PressDownKeys::initialize();
      FlagStatus::initialize();
      ButtonStatus::initialize();
      KeyRemap4MacBook_client::initialize();

      ListHookedKeyboard::instance().initialize();
      ListHookedConsumer::instance().initialize();
      ListHookedPointing::instance().initialize();

      workLoop = IOWorkLoop::workLoop();
      if (! workLoop) {
        IOLOG_ERROR("IOWorkLoop::workLoop failed\n");
      } else {
        timer_refresh.initialize(workLoop, NULL, refreshHookedDevice);
        timer_refresh.setTimeoutMS(REFRESH_DEVICE_INTERVAL);

        RemapClassManager::initialize(*workLoop);
        KeyboardRepeat::initialize(*workLoop);
        EventInputQueue::initialize(*workLoop);
        VirtualKey::initialize(*workLoop);
        EventOutputQueue::initialize(*workLoop);
        RemapFunc::HoldingKeyToKey::static_initialize(*workLoop);
        RemapFunc::KeyOverlaidModifier::static_initialize(*workLoop);
        RemapFunc::PointingRelativeToScroll::static_initialize(*workLoop);
        ListHookedKeyboard::static_initialize(*workLoop);
      }

      sysctl_register();
    }

    void
    stop(void)
    {
      sysctl_unregister();

      timer_refresh.terminate();
      ListHookedKeyboard::instance().terminate();
      ListHookedConsumer::instance().terminate();
      ListHookedPointing::instance().terminate();
      RemapClassManager::terminate();
      KeyboardRepeat::terminate();
      EventInputQueue::terminate();
      VirtualKey::terminate();
      EventOutputQueue::terminate();
      RemapFunc::HoldingKeyToKey::static_terminate();
      RemapFunc::KeyOverlaidModifier::static_terminate();
      RemapFunc::PointingRelativeToScroll::static_terminate();
      ListHookedKeyboard::static_terminate();

      if (workLoop) {
        workLoop->release();
        workLoop = NULL;
      }

      KeyRemap4MacBook_client::terminate();
      EventWatcher::terminate();
      PressDownKeys::terminate();
      CommonData::terminate();
    }

    // ======================================================================
    bool
    notifierfunc_hookKeyboard(void* target, void* refCon, IOService* newService, IONotifier* notifier)
    {
      IOLOG_INFO("notifierfunc_hookKeyboard newService:%p\n", newService);

      IOHIDevice* device = OSDynamicCast(IOHIKeyboard, newService);
      if (! device) return false;

      ListHookedKeyboard::instance().push_back(new ListHookedKeyboard::Item(device));
      ListHookedConsumer::instance().push_back(new ListHookedConsumer::Item(device));
      return true;
    }

    bool
    notifierfunc_unhookKeyboard(void* target, void* refCon, IOService* newService, IONotifier* notifier)
    {
      IOLOG_INFO("notifierfunc_unhookKeyboard newService:%p\n", newService);

      IOHIDevice* device = OSDynamicCast(IOHIKeyboard, newService);
      if (! device) return false;

      ListHookedKeyboard::instance().erase(device);
      ListHookedConsumer::instance().erase(device);
      return true;
    }

    bool
    notifierfunc_hookPointing(void* target, void* refCon, IOService* newService, IONotifier* notifier)
    {
      IOLOG_INFO("notifierfunc_hookPointing newService:%p\n", newService);

      IOHIDevice* device = OSDynamicCast(IOHIPointing, newService);
      if (! device) return false;

      ListHookedPointing::instance().push_back(new ListHookedPointing::Item(device));
      return true;
    }

    bool
    notifierfunc_unhookPointing(void* target, void* refCon, IOService* newService, IONotifier* notifier)
    {
      IOLOG_INFO("notifierfunc_unhookPointing newService:%p\n", newService);

      IOHIDevice* device = OSDynamicCast(IOHIPointing, newService);
      if (! device) return false;

      ListHookedPointing::instance().erase(device);
      return true;
    }

    // ======================================================================
    void
    remap_KeyboardEventCallback(Params_KeyboardEventCallBack& params)
    {
      params.log();

      // ------------------------------------------------------------
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
        EventOutputQueue::FireModifiers::fire(FlagStatus::makeFlags());
        EventOutputQueue::FireRelativePointer::fire();
        PressDownKeys::clear();
      }
    }

    void
    remap_KeyboardSpecialEventCallback(Params_KeyboardSpecialEventCallback& params)
    {
      params.log();

      RemapConsumerParams remapParams(params);

      // ------------------------------------------------------------
      RemapClassManager::remap_consumer(remapParams);

      // ----------------------------------------
      if (! remapParams.isremapped) {
        Params_KeyboardSpecialEventCallback::auto_ptr ptr(Params_KeyboardSpecialEventCallback::alloc(params.eventType, FlagStatus::makeFlags(), params.key,
                                                                                                     params.flavor, params.guid, false));
        if (ptr) {
          KeyboardRepeat::set(*ptr);
          EventOutputQueue::FireConsumer::fire(*ptr);
        }
      }
    }

    void
    remap_RelativePointerEventCallback(Params_RelativePointerEventCallback& params)
    {
      params.log();

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
      params.log();
      EventOutputQueue::FireScrollWheel::fire(params);
    }
  }
}
