#include "Core.hpp"
#include "Config.hpp"
#include "RemapUtil.hpp"
#include "Client.hpp"
#include "util/ListHookedKeyboard.hpp"
#include "util/ListHookedConsumer.hpp"
#include "util/ListHookedPointing.hpp"
#include "util/KeyboardRepeat.hpp"
#include "util/TimerWrapper.hpp"
#include "util/NumHeldDownKeys.hpp"
#include "remap.hpp"
#include "bridge.hpp"

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
    }

    void
    terminate(void)
    {
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
      IOHIKeyboard* kbd = OSDynamicCast(IOHIKeyboard, params.sender);
      if (! kbd) return;

      HookedKeyboard* p = ListHookedKeyboard::instance().get(kbd);
      if (! p) return;

      // ------------------------------------------------------------
      // Because the key repeat generates it by oneself, I throw it away.
      KeyboardRepeat::setTS(params.ts);
      if (params.repeat) {
        return;
      }

      params.log();

      // ------------------------------------------------------------
      if (config.general_capslock_led_hack) {
        int led = kbd->getLEDStatus();
        if (led == 0) {
          kbd->setAlphaLockFeedback(true);
        }
      }

      // ------------------------------------------------------------
      listFireConsumerKey.reset();
      listFireRelativePointer.reset();

      bool isremapped = false;
      KeyCode::KeyCode ex_repeatKeyCode = KeyCode::VK_NONE;
      unsigned int ex_repeatFlags = 0;

      KeyCode::normalizeKey(params.key, params.flags, params.keyboardType);

      RemapParams remapParams = {
        params,
        isremapped,
        KeyRemap4MacBook_bridge::GetWorkspaceData::Reply(),
        ex_repeatKeyCode,
        ex_repeatFlags,
        0,
      };
      NumHeldDownKeys::set(remapParams);

      // ------------------------------------------------------------
      getWorkspaceData(remapParams.workspacedata);

      // ------------------------------------------------------------
      remap_core(remapParams);

      // ------------------------------------------------------------
      // pointing emulation
      if (! listFireRelativePointer.isEmpty()) {
        HookedPointing* hp = ListHookedPointing::instance().get();
        if (hp) {
          listFireRelativePointer.fire(hp->getOrig_relativePointerEventTarget(), hp->get(), params.ts);
        }
      }

      // consumer emulation
      if (! listFireConsumerKey.isEmpty()) {
        HookedConsumer* hc = ListHookedConsumer::instance().get();
        if (hc) {
          listFireConsumerKey.fire(hc->getOrig_keyboardSpecialEventTarget(), params.ts, params.sender, params.refcon);
        }
      }

      if (! remapParams.isremapped) {
        RemapUtil::fireKey(params, remapParams.workspacedata);
        KeyboardRepeat::set(params);
      }

      if (NumHeldDownKeys::iszero()) {
        NumHeldDownKeys::reset();
        cancelRepeat();
        EventWatcher::reset();
        FlagStatus::reset();
        params.flags = FlagStatus::makeFlags(params.key);
        RemapUtil::fireModifiers(params);
        PressDownKeys::clear(params.target, params.ts, params.sender, params.refcon);
      }
    }

    void
    remap_KeyboardSpecialEventCallback(Params_KeyboardSpecialEventCallback& params)
    {
      IOHIKeyboard* kbd = OSDynamicCast(IOHIKeyboard, params.sender);
      if (! kbd) return;

      HookedConsumer* p = ListHookedConsumer::instance().get(kbd);
      if (! p) return;

      // ------------------------------------------------------------
      params.log();

      KeyCode::KeyCode ex_remapKeyCode = KeyCode::VK_NONE;
      RemapConsumerParams remapParams = {
        params,
        KeyRemap4MacBook_bridge::GetWorkspaceData::Reply(),
        ex_remapKeyCode,
      };
      NumHeldDownKeys::set(remapParams);

      // ------------------------------------------------------------
      getWorkspaceData(remapParams.workspacedata);

      // ------------------------------------------------------------
      remap_consumer(remapParams);

      // ----------------------------------------
      HookedKeyboard* hk = ListHookedKeyboard::instance().get();
      if (hk) {
        Params_KeyboardEventCallBack callbackparams = {
          hk->getOrig_keyboardEventTarget(), params.eventType, params.flags, ex_remapKeyCode,
          0, 0, 0, 0,
          KeyboardType::MACBOOK, false, params.ts, hk->get(), NULL,
        };

        if (ex_remapKeyCode != KeyCode::VK_NONE) {
          RemapUtil::fireKey(callbackparams, remapParams.workspacedata);
          KeyboardRepeat::set(callbackparams);
        }
      }

      RemapUtil::fireConsumer(params);
    }

    void
    remap_RelativePointerEventCallback(Params_RelativePointerEventCallback& params)
    {
      IOHIPointing* pointing = OSDynamicCast(IOHIPointing, params.sender);
      if (! pointing) return;

      HookedPointing* p = ListHookedPointing::instance().get(pointing);
      if (! p) return;

      // ------------------------------------------------------------
      params.log();

      listFireRelativePointer.reset();
      unsigned int flags = FlagStatus::makeFlags(KeyCode::VK_NONE);

      bool ex_dropEvent = false;
      RemapPointingParams_relative remapParams = {
        params, ex_dropEvent,
      };
      remap_pointing_relative_core(remapParams);

      // ------------------------------------------------------------
      unsigned int newflags = FlagStatus::makeFlags(KeyCode::VK_NONE);
      if (flags != newflags) {
        HookedKeyboard* hk = ListHookedKeyboard::instance().get();
        if (hk) {
          Params_KeyboardEventCallBack callbackparams = {
            hk->getOrig_keyboardEventTarget(), KeyEvent::MODIFY, newflags, KeyCode::VK_NONE,
            0, 0, 0, 0,
            KeyboardType::MACBOOK, false, params.ts, hk->get(), NULL,
          };
          RemapUtil::fireModifiers(callbackparams);
        }
      }

      // ------------------------------------------------------------
      if (! ex_dropEvent) {
        params.apply();
      }

      if (! listFireRelativePointer.isEmpty()) {
        listFireRelativePointer.fire(params.target, pointing, params.ts);
      }

      if (firePointingScroll.isEnable()) {
        firePointingScroll.fire(p->getOrig_scrollWheelEventAction(), p->getOrig_scrollWheelEventTarget(), pointing, params.ts);
      }
    }

    void
    remap_ScrollWheelEventCallback(Params_ScrollWheelEventCallback& params)
    {
      IOHIPointing* pointing = OSDynamicCast(IOHIPointing, params.sender);
      if (! pointing) return;

      HookedPointing* p = ListHookedPointing::instance().get(pointing);
      if (! p) return;

      // ------------------------------------------------------------
      params.log();
      params.apply();
    }
  }
}
