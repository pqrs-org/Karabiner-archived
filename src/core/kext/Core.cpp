#include "Core.hpp"
#include "Config.hpp"
#include "RemapUtil.hpp"
#include "Client.hpp"
#include "util/ListHookedKeyboard.hpp"
#include "util/ListHookedConsumer.hpp"
#include "util/ListHookedPointing.hpp"
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

      TimerWrapper timer_repeat_keyboard;
      TimerWrapper timer_repeat_keyboard_extra;
      TimerWrapper timer_repeat_consumer;

      class KeyboardRepeatInfo {
      public:
        KeyboardRepeatInfo(void) { kbd = NULL; }
        const IOHIKeyboard* kbd;
        Params_KeyboardEventCallBack params;
      } keyboardRepeatInfo;

      class KeyboardRepeatInfo_extra {
      public:
        KeyboardRepeatInfo_extra(void) { kbd = NULL; }
        IOHIKeyboard* kbd;
        Params_KeyboardEventCallBack params;
        ExtraRepeatFunc::ExtraRepeatFunc func;
        unsigned int counter;
      } keyboardRepeatInfo_extra;

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
      setRepeat_keyboard(const IOHIKeyboard* kbd, const Params_KeyboardEventCallBack& params)
      {
        IOLockWrapper::ScopedLock lk(timer_repeat_keyboard.getlock());

        if (params.eventType == KeyEvent::DOWN) {
          keyboardRepeatInfo.kbd = kbd;
          keyboardRepeatInfo.params = params;

          IOReturn result = timer_repeat_keyboard.setTimeoutMS(config.get_repeat_initial_wait());
          if (result != kIOReturnSuccess) {
            IOLog("[KeyRemap4MacBook ERROR] setTimeoutMS failed\n");
          }
          if (config.debug_devel) {
            IOLog("KeyRemap4MacBook -Info- setRepeat_keyboard key:%d\n", params.key);
          }

        } else if (params.eventType == KeyEvent::MODIFY || keyboardRepeatInfo.params.key == params.key) {
          if (config.debug_devel) {
            IOLog("KeyRemap4MacBook -Info- cancel keyrepeat\n");
          }
          timer_repeat_keyboard.cancelTimeout();
        }
      }

      void
      setRepeat_keyboard_extra(IOHIKeyboard* kbd, const Params_KeyboardEventCallBack& params, ExtraRepeatFunc::ExtraRepeatFunc func, unsigned int flags)
      {
        IOLockWrapper::ScopedLock lk(timer_repeat_keyboard_extra.getlock());

        if (func) {
          keyboardRepeatInfo_extra.kbd = kbd;
          keyboardRepeatInfo_extra.func = func;
          keyboardRepeatInfo_extra.counter = 0;
          keyboardRepeatInfo_extra.params = params;
          keyboardRepeatInfo_extra.params.flags = flags;

          timer_repeat_keyboard_extra.setTimeoutMS(config.get_keyoverlaidmodifier_initial_wait());

        } else {
          timer_repeat_keyboard_extra.cancelTimeout();
          keyboardRepeatInfo_extra.func = NULL;
        }
      }

      void
      cancelRepeat(void)
      {
        if (config.debug_devel) {
          IOLog("KeyRemap4MacBook -Info- cancelRepeat\n");
        }

        {
          IOLockWrapper::ScopedLock lk(timer_repeat_keyboard.getlock());
          timer_repeat_keyboard.cancelTimeout();
        }
        {
          IOLockWrapper::ScopedLock lk(timer_repeat_keyboard_extra.getlock());
          timer_repeat_keyboard_extra.cancelTimeout();
        }
      }

      void
      doRepeat_keyboard(OSObject* owner, IOTimerEventSource* sender)
      {
        IOLockWrapper::ScopedLock lk(timer_repeat_keyboard.getlock());

        HookedKeyboard* p = ListHookedKeyboard::instance().get(keyboardRepeatInfo.kbd);
        if (! p) return;

        KeyboardEventCallback callback = p->getOrig_keyboardEventAction();
        if (! callback) {
          IOLog("KeyRemap4MacBook -Info- doRepeat_keyboard: callback == NULL (== don't remap xxx is enabled). \n");
          return;
        }

        RemapUtil::fireKey(callback, keyboardRepeatInfo.params);
        timer_repeat_keyboard.setTimeoutMS(config.get_repeat_wait());
      }

      void
      doRepeat_keyboard_extra(OSObject* owner, IOTimerEventSource* sender)
      {
        IOLockWrapper::ScopedLock lk(timer_repeat_keyboard_extra.getlock());

        HookedKeyboard* p = ListHookedKeyboard::instance().get(keyboardRepeatInfo_extra.kbd);
        if (! p) return;

        KeyboardEventCallback callback = p->getOrig_keyboardEventAction();
        if (! callback) {
          IOLog("KeyRemap4MacBook -Info- doRepeat_keyboard_extra: callback == NULL (== don't remap xxx is enabled). \n");
          return;
        }

        keyboardRepeatInfo_extra.func(callback,
                                      keyboardRepeatInfo_extra.params.target,
                                      keyboardRepeatInfo_extra.params.flags,
                                      keyboardRepeatInfo_extra.params.keyboardType,
                                      keyboardRepeatInfo_extra.params.ts,
                                      keyboardRepeatInfo_extra.params.sender,
                                      keyboardRepeatInfo_extra.params.refcon);
        ++(keyboardRepeatInfo_extra.counter);

        timer_repeat_keyboard_extra.setTimeoutMS(config.get_repeat_wait());
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
      }
    }

    void
    initialize(void)
    {
      sysctl_register();
      ClickWatcher::reset();
      PressDownKeys::initialize();
      FlagStatus::initialize();
      KeyRemap4MacBook_client::initialize();
    }

    void
    terminate(void)
    {
      KeyRemap4MacBook_client::terminate();
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

        timer_repeat_keyboard.initialize(workLoop, NULL, doRepeat_keyboard);
        timer_repeat_keyboard_extra.initialize(workLoop, NULL, doRepeat_keyboard_extra);
      }
    }

    void
    stop(void)
    {
      timer_refresh.terminate();
      ListHookedKeyboard::instance().terminate();
      ListHookedConsumer::instance().terminate();
      ListHookedPointing::instance().terminate();

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
      if (params.repeat) {
        keyboardRepeatInfo.params.ts = params.ts;
        keyboardRepeatInfo_extra.params.ts = params.ts;
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
      ListFireExtraKey::reset();
      listFireConsumerKey.reset();
      listFireRelativePointer.reset();

      bool isremapped = false;
      unsigned int ex_extraRepeatFlags = 0;
      ExtraRepeatFunc::ExtraRepeatFunc ex_extraRepeatFunc = NULL;
      KeyCode::KeyCode ex_repeatKeyCode = KeyCode::NONE;
      unsigned int ex_repeatFlags = 0;

      KeyCode::normalizeKey(params.key, params.flags, params.keyboardType);

      RemapParams remapParams = {
        params,
        isremapped,
        KeyRemap4MacBook_bridge::GetWorkspaceData::Reply(),
        ex_extraRepeatFunc,
        ex_extraRepeatFlags,
        ex_repeatKeyCode,
        ex_repeatFlags,
        keyboardRepeatInfo_extra.counter,
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
          listFireRelativePointer.fire(hp->getOrig_relativePointerEventAction(), hp->getOrig_relativePointerEventTarget(), hp->get(), params.ts);
        }
      }

      // consumer emulation
      if (! listFireConsumerKey.isEmpty()) {
        HookedConsumer* hc = ListHookedConsumer::instance().get();
        if (hc) {
          listFireConsumerKey.fire(hc->getOrig_keyboardSpecialEventAction(), hc->getOrig_keyboardSpecialEventTarget(), params.ts, params.sender, params.refcon);
        }
      }

      RemapUtil::fireKey(p->getOrig_keyboardEventAction(), params);
      ListFireExtraKey::fire(p->getOrig_keyboardEventAction(), params);

      setRepeat_keyboard(kbd, params);
      setRepeat_keyboard_extra(kbd, params, ex_extraRepeatFunc, ex_extraRepeatFlags);

      if (NumHeldDownKeys::iszero()) {
        NumHeldDownKeys::reset();
        cancelRepeat();
        FlagStatus::reset();
        params.flags = FlagStatus::makeFlags(params.key);
        RemapUtil::fireModifiers(p->getOrig_keyboardEventAction(), params);
        PressDownKeys::clear(p->getOrig_keyboardEventAction(), params.target, params.ts, params.sender, params.refcon);
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

      ListFireExtraKey::reset();
      KeyCode::KeyCode ex_remapKeyCode = KeyCode::NONE;
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
      unsigned int keyboardType = KeyboardType::MACBOOK;

      if (hk) {
        Params_KeyboardEventCallBack callbackparams = {
          hk->getOrig_keyboardEventTarget(), params.eventType, params.flags, ex_remapKeyCode,
          0, 0, 0, 0,
          keyboardType, false, params.ts, hk->get(), NULL,
        };

        if (ex_remapKeyCode != KeyCode::NONE) {
          RemapUtil::fireKey(hk->getOrig_keyboardEventAction(), callbackparams);
          setRepeat_keyboard(hk->get(), callbackparams);
          setRepeat_keyboard_extra(hk->get(), callbackparams, NULL, 0);
        }
        ListFireExtraKey::fire(hk->getOrig_keyboardEventAction(), callbackparams);
      }

      RemapUtil::fireConsumer(p->getOrig_keyboardSpecialEventAction(), params);
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
      unsigned int flags = FlagStatus::makeFlags(KeyCode::NONE);

      bool ex_dropEvent = false;
      RemapPointingParams_relative remapParams = {
        params, ex_dropEvent,
      };
      remap_pointing_relative_core(remapParams);

      // ------------------------------------------------------------
      unsigned int newflags = FlagStatus::makeFlags(KeyCode::NONE);
      if (flags != newflags) {
        HookedKeyboard* hk = ListHookedKeyboard::instance().get();
        unsigned int keyboardType = KeyboardType::MACBOOK;
        if (hk) {
          Params_KeyboardEventCallBack callbackparams = {
            hk->getOrig_keyboardEventTarget(), KeyEvent::MODIFY, newflags, KeyCode::NONE,
            0, 0, 0, 0,
            keyboardType, false, params.ts, hk->get(), NULL,
          };
          RemapUtil::fireModifiers(hk->getOrig_keyboardEventAction(), callbackparams);
        }
      }

      // ------------------------------------------------------------
      RelativePointerEventCallback reCallback = p->getOrig_relativePointerEventAction();

      if (! ex_dropEvent) {
        params.apply(reCallback);
      }

      if (! listFireRelativePointer.isEmpty()) {
        listFireRelativePointer.fire(reCallback, params.target, pointing, params.ts);
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

      params.apply(p->getOrig_scrollWheelEventAction());
    }
  }
}
