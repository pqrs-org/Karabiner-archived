#include "base.hpp"
#include "Client.hpp"
#include "CommonData.hpp"
#include "Config.hpp"
#include "Core.hpp"
#include "RemapUtil.hpp"
#include "bridge.hpp"
#include "remap.hpp"
#include "util/KeyboardRepeat.hpp"
#include "util/ListHookedConsumer.hpp"
#include "util/ListHookedKeyboard.hpp"
#include "util/ListHookedPointing.hpp"
#include "util/NumHeldDownKeys.hpp"
#include "util/TimerWrapper.hpp"

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

        ListHookedKeyboard::instance().refresh();
        ListHookedConsumer::instance().refresh();
        ListHookedPointing::instance().refresh();

        timer_refresh.setTimeoutMS(REFRESH_DEVICE_INTERVAL);
      }

      // ----------------------------------------
      void
      cancelRepeat(void)
      {
        if (config.debug_devel) {
          IOLog("KeyRemap4MacBook -Info- cancelRepeat\n");
        }

        KeyboardRepeat::cancel();
      }

      void
      getWorkspaceData(KeyRemap4MacBook_bridge::GetWorkspaceData::Reply& reply)
      {
        // ------------------------------------------------------------
        // When we press the functional key (ex. F2) with the keyboard of the third vendor,
        // KeyRemap4MacBook_client::sendmsg returns EIO.
        //
        // We use the previous value when the error occurred.
        static KeyRemap4MacBook_bridge::GetWorkspaceData::Reply last = {
          KeyRemap4MacBook_bridge::GetWorkspaceData::UNKNOWN,
          KeyRemap4MacBook_bridge::GetWorkspaceData::INPUTMODE_ROMAN,
          KeyRemap4MacBook_bridge::GetWorkspaceData::INPUTMODE_DETAIL_ROMAN,
        };

        int error = KeyRemap4MacBook_client::sendmsg(KeyRemap4MacBook_bridge::REQUEST_GET_WORKSPACE_DATA, NULL, 0, &reply, sizeof(reply));
        if (config.debug_devel) {
          printf("KeyRemap4MacBook -Info- GetWorkspaceData: %d,%d,%d (error: %d)\n",
                 reply.type, reply.inputmode, reply.inputmodedetail, error);
        }
        if (error == 0) {
          last = reply;
        } else {
          // use last info.
          reply = last;
        }

        // ------------------------------------------------------------
        // Store data to KeyboardRepeat
        KeyboardRepeat::setWorkSpaceData(reply);
      }
    }

    void
    initialize(void)
    {
      sysctl_register();
      EventWatcher::initialize();
      PressDownKeys::initialize();
      FlagStatus::initialize();
      KeyRemap4MacBook_client::initialize();
      CommonData::initialize();
      refresh_remapfunc();
    }

    void
    terminate(void)
    {
      CommonData::terminate();
      KeyRemap4MacBook_client::terminate();
      EventWatcher::terminate();
      sysctl_unregister();
    }

    void
    start(void)
    {
      ListHookedKeyboard::instance().initialize();
      ListHookedConsumer::instance().initialize();
      ListHookedPointing::instance().initialize();

      workLoop = IOWorkLoop::workLoop();
      if (! workLoop) {
        IOLog("[KeyRemap4MacBook ERROR] IOWorkLoop::workLoop failed\n");
      } else {
        timer_refresh.initialize(workLoop, NULL, refreshHookedDevice);
        timer_refresh.setTimeoutMS(REFRESH_DEVICE_INTERVAL);

        KeyboardRepeat::initialize(*workLoop);
      }
    }

    void
    stop(void)
    {
      timer_refresh.terminate();
      ListHookedKeyboard::instance().terminate();
      ListHookedConsumer::instance().terminate();
      ListHookedPointing::instance().terminate();
      KeyboardRepeat::terminate();

      if (workLoop) {
        workLoop->release();
        workLoop = NULL;
      }
    }

    // ======================================================================
    bool
    notifierfunc_hookKeyboard(void* target, void* refCon, IOService* newService, IONotifier* notifier)
    {
      IOLog("KeyRemap4MacBook notifierfunc_hookKeyboard\n");

      IOHIKeyboard* kbd = OSDynamicCast(IOHIKeyboard, newService);
      ListHookedKeyboard::instance().append(kbd);
      ListHookedConsumer::instance().append(kbd);
      return true;
    }

    bool
    notifierfunc_unhookKeyboard(void* target, void* refCon, IOService* newService, IONotifier* notifier)
    {
      IOLog("KeyRemap4MacBook notifierfunc_unhookKeyboard\n");

      IOHIKeyboard* kbd = OSDynamicCast(IOHIKeyboard, newService);
      ListHookedKeyboard::instance().terminate(kbd);
      ListHookedConsumer::instance().terminate(kbd);
      return true;
    }

    bool
    notifierfunc_hookPointing(void* target, void* refCon, IOService* newService, IONotifier* notifier)
    {
      IOLog("KeyRemap4MacBook notifierfunc_hookPointing\n");

      IOHIPointing* pointing = OSDynamicCast(IOHIPointing, newService);
      return ListHookedPointing::instance().append(pointing);
    }

    bool
    notifierfunc_unhookPointing(void* target, void* refCon, IOService* newService, IONotifier* notifier)
    {
      IOLog("KeyRemap4MacBook notifierfunc_unhookPointing\n");

      IOHIPointing* pointing = OSDynamicCast(IOHIPointing, newService);
      return ListHookedPointing::instance().terminate(pointing);
    }

    // ======================================================================
    void
    remap_KeyboardEventCallback(Params_KeyboardEventCallBack& params)
    {
      params.log();

      // ------------------------------------------------------------
      params.key.normalizeKey(params.flags, params.keyboardType);

      KeyRemap4MacBook_bridge::GetWorkspaceData::Reply workspacedata;
      getWorkspaceData(workspacedata);

      RemapParams remapParams(params, workspacedata);
      NumHeldDownKeys::set(remapParams);

      // ------------------------------------------------------------
      remap_core(remapParams);

      // ------------------------------------------------------------
      if (! remapParams.isremapped) {
        params.flags = FlagStatus::makeFlags();
        KeyboardRepeat::set(params);
        RemapUtil::fireKey(params, remapParams.workspacedata);
      }

      if (NumHeldDownKeys::iszero()) {
        NumHeldDownKeys::reset();
        cancelRepeat();
        EventWatcher::reset();
        FlagStatus::reset();
        FireModifiers::fire(FlagStatus::makeFlags());
        PressDownKeys::clear();
      }
    }

    void
    remap_KeyboardSpecialEventCallback(Params_KeyboardSpecialEventCallback& params)
    {
      params.log();

      KeyRemap4MacBook_bridge::GetWorkspaceData::Reply workspacedata;
      getWorkspaceData(workspacedata);

      RemapConsumerParams remapParams(params, workspacedata);
      NumHeldDownKeys::set(remapParams);

      // ------------------------------------------------------------
      remap_consumer(remapParams);

      // ----------------------------------------
      if (! remapParams.isremapped) {
        params.flags = FlagStatus::makeFlags();
        RemapUtil::fireConsumer(params);
      }
    }

    void
    remap_RelativePointerEventCallback(Params_RelativePointerEventCallback& params)
    {
      params.log();

      RemapPointingParams_relative remapParams(params);
      remap_pointing_relative_core(remapParams);

      // ------------------------------------------------------------
      if (! remapParams.isremapped) {
        params.buttons.add(RemapUtil::getRemappedButtons());
        RemapUtil::fireRelativePointer(params);
      }
    }

    void
    remap_ScrollWheelEventCallback(Params_ScrollWheelEventCallback& params)
    {
      params.log();
      RemapUtil::fireScrollWheel(params);
    }
  }
}
