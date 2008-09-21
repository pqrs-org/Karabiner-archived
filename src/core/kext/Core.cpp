#include "Core.hpp"
#include "Config.hpp"
#include "RemapUtil.hpp"
#include "Client.hpp"
#include "util/ListHookedKeyboard.hpp"
#include "util/ListHookedPointing.hpp"
#include "util/TimerWrapper.hpp"
#include "util/NumHeldDownKeys.hpp"
#include "remap.hpp"

#include <IOKit/IOWorkLoop.h>
#include <IOKit/IOTimerEventSource.h>

namespace org_pqrs_KeyRemap4MacBook {
  namespace Core {
    namespace {
      IOWorkLoop *workLoop = NULL;
      TimerWrapper timer_refresh;

      TimerWrapper timer_repeat_keyboard;
      TimerWrapper timer_repeat_keyboard_extra;
      TimerWrapper timer_repeat_consumer;

      class KeyboardRepeatInfo {
      public:
        KeyboardRepeatInfo(void) { kbd = NULL; }
        const IOHIKeyboard *kbd;
        Params_KeyboardEventCallBack params;
      } keyboardRepeatInfo;

      class KeyboardExtraRepeatInfo {
      public:
        KeyboardExtraRepeatInfo(void) { kbd = NULL; }
        IOHIKeyboard *kbd;
        Params_KeyboardEventCallBack params;
        org_pqrs_KeyRemap4MacBook::ExtraRepeatFunc::ExtraRepeatFunc func;
        unsigned int counter;
      } keyboardExtraRepeatInfo;

      IOHIKeyboard *last_keyboard = NULL;
      IOHIKeyboard *last_consumer = NULL;
      IOHIPointing *last_pointing = NULL;
      Params_KeyboardEventCallBack params_lastKeyboard;
      Params_KeyboardSpecialEventCallback params_lastConsumer;
      Params_RelativePointerEventCallback params_lastRelativePointer;
      Params_ScrollWheelEventCallback params_lastScrollWheel;

      // ------------------------------------------------------------
      enum {
        REFRESH_DEVICE_INTERVAL = 1000,
      };

      void
      refreshHookedDevice(OSObject *owner, IOTimerEventSource *sender)
      {
        ListHookedKeyboard::refresh();
        ListHookedPointing::refresh();

        if (sender) sender->setTimeoutMS(REFRESH_DEVICE_INTERVAL);
      }

      // ----------------------------------------
      void
      cancelAllKeyRepeat(void)
      {
        timer_repeat_keyboard.cancelTimeout();
        timer_repeat_keyboard_extra.cancelTimeout();
      }

      // ----------------------------------------
      void
      setRepeat_keyboard(const IOHIKeyboard *kbd, const Params_KeyboardEventCallBack &params)
      {
        if (params.eventType == KeyEvent::DOWN) {
          keyboardRepeatInfo.kbd = kbd;
          keyboardRepeatInfo.params = params;

          timer_repeat_keyboard.setTimeoutMS(config.get_repeat_initial_wait());

        } else if (params.eventType == KeyEvent::MODIFY || keyboardRepeatInfo.params.key == params.key) {
          timer_repeat_keyboard.cancelTimeout();
        }
      }

      void
      doRepeat_keyboard(OSObject *owner, IOTimerEventSource *sender)
      {
        ListHookedKeyboard::Item *p = ListHookedKeyboard::get(keyboardRepeatInfo.kbd);
        if (! p) return;

        RemapUtil::fireKey(p->getOrig_keyboardEventAction(), keyboardRepeatInfo.params);
        sender->setTimeoutMS(config.get_repeat_wait());
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
      sysctl_unregister();
    }

    void
    start(void)
    {
      workLoop = IOWorkLoop::workLoop();
      if (workLoop) {
        timer_refresh.initialize(workLoop, NULL, refreshHookedDevice);
        timer_refresh.setTimeoutMS(REFRESH_DEVICE_INTERVAL);

        timer_repeat_keyboard.initialize(workLoop, NULL, doRepeat_keyboard);
      }
    }

    void
    stop(void)
    {
      timer_refresh.terminate();
      ListHookedKeyboard::terminate();
      ListHookedPointing::terminate();

      if (workLoop) {
        workLoop->release();
        workLoop = NULL;
      }
    }

    // ======================================================================
    bool
    notifierfunc_hookKeyboard(void *target, void *refCon, IOService *newService)
    {
      IOLog("KeyRemap4MacBook notifierfunc_hookKeyboard\n");

      IOHIKeyboard *kbd = OSDynamicCast(IOHIKeyboard, newService);
      return ListHookedKeyboard::append(kbd);
    }

    bool
    notifierfunc_unhookKeyboard(void *target, void *refCon, IOService *newService)
    {
      IOLog("KeyRemap4MacBook notifierfunc_unhookKeyboard\n");

      IOHIKeyboard *kbd = OSDynamicCast(IOHIKeyboard, newService);
      return ListHookedKeyboard::terminate(kbd);
    }

    bool
    notifierfunc_hookPointing(void *target, void *refCon, IOService *newService)
    {
      IOLog("KeyRemap4MacBook notifierfunc_hookPointing\n");

      IOHIPointing *pointing = OSDynamicCast(IOHIPointing, newService);
      return ListHookedPointing::append(pointing);
    }

    bool
    notifierfunc_unhookPointing(void *target, void *refCon, IOService *newService)
    {
      IOLog("KeyRemap4MacBook notifierfunc_unhookPointing\n");

      IOHIPointing *pointing = OSDynamicCast(IOHIPointing, newService);
      return ListHookedPointing::terminate(pointing);
    }

    // ======================================================================
    void
    remap_KeyboardEventCallback(Params_KeyboardEventCallBack *params)
    {
      if (! params) return;

      IOHIKeyboard *kbd = OSDynamicCast(IOHIKeyboard, params->sender);
      if (! kbd) return;

      ListHookedKeyboard::Item *p = ListHookedKeyboard::get(kbd);
      if (! p) return;

      // ------------------------------------------------------------
      last_keyboard = kbd;
      params_lastKeyboard = *params;

      // ------------------------------------------------------------
      // Because the key repeat generates it by oneself, I throw it away.
      if (params->repeat) {
        keyboardRepeatInfo.params.ts = params->ts;
        keyboardExtraRepeatInfo.params.ts = params->ts;
        return;
      }

      params->log();

      // ------------------------------------------------------------
      if (config.general_capslock_led_hack) {
        int led = kbd->getLEDStatus();
        if (led == 0) {
          kbd->setAlphaLockFeedback(true);
        }
      }

      // ------------------------------------------------------------
      listFireExtraKey.reset();
      listFireConsumerKey.reset();
      listFireRelativePointer.reset();

      unsigned int ex_extraRepeatFlags = 0;
      ExtraRepeatFunc::ExtraRepeatFunc ex_extraRepeatFunc = NULL;

      RemapUtil::normalizeKeyBeforeRemap(params);

      RemapParams remapParams = {
        params,
        params->key,
        KeyRemap4MacBook_bridge::ActiveApplicationInfo::UNKNOWN,
        &ex_extraRepeatFunc,
        &ex_extraRepeatFlags,
        keyboardExtraRepeatInfo.counter,
      };
      NumHeldDownKeys::set(remapParams);

      bool skip = false;
      if (RemapUtil::isInternalKeyboard(params->keyboardType)) {
        if (config.general_dont_remap_internal) skip = true;
      } else {
        if (config.general_dont_remap_external) skip = true;
      }
      if (! skip) {
        KeyRemap4MacBook_bridge::ActiveApplicationInfo::Reply activeApplicationInfo;
        int error = KeyRemap4MacBook_client::sendmsg(KeyRemap4MacBook_bridge::REQUEST_ACTIVE_APPLICATION_INFO, NULL, 0, &activeApplicationInfo, sizeof(activeApplicationInfo));
        if (error == 0) {
          remapParams.appType = activeApplicationInfo.type;
        }
        remap_core(remapParams);
      }

      // pointing emulation
      if (! listFireRelativePointer.isEmpty()) {
        ListHookedPointing::Item *hp = ListHookedPointing::get();
        if (hp) {
          listFireRelativePointer.fire(hp->getOrig_relativePointerEventAction(), hp->getOrig_relativePointerEventTarget(), hp->get(), params->ts);
        }
      }

      // consumer emulation
      if (! listFireConsumerKey.isEmpty()) {
        ListHookedKeyboard::Item *hc = ListHookedKeyboard::getConsumer();
        if (hc) {
          listFireConsumerKey.fire(hc->getOrig_keyboardSpecialEventAction(), hc->getOrig_keyboardSpecialEventTarget(), params->ts, params->sender, params->refcon);
        }
      }

      RemapUtil::fireKey(p->getOrig_keyboardEventAction(), *params);
      listFireExtraKey.fire(p->getOrig_keyboardEventAction(), *params);

      setRepeat_keyboard(kbd, *params);
#if 0
      p->setExtraRepeatInfo(ex_extraRepeatFunc, ex_extraRepeatFlags, params->keyboardType, params->ts, params->target, params->refcon);
#endif

      if (NumHeldDownKeys::iszero()) {
        NumHeldDownKeys::reset();
        FlagStatus::reset();
        params->flags = 0;
        RemapUtil::fireModifiers(p->getOrig_keyboardEventAction(), *params);
        PressDownKeys::clear(p->getOrig_keyboardEventAction(), params->target, params->ts, params->sender, params->refcon);
      }
    }

    void
    remap_KeyboardSpecialEventCallback(Params_KeyboardSpecialEventCallback *params)
    {
      if (! params) return;

      IOHIKeyboard *kbd = OSDynamicCast(IOHIKeyboard, params->sender);
      if (! kbd) return;

      ListHookedKeyboard::Item *p = ListHookedKeyboard::get(kbd);
      if (! p) return;

      // ------------------------------------------------------------
      last_consumer = kbd;
      params_lastConsumer = *params;

      // ------------------------------------------------------------
      params->log();

      listFireExtraKey.reset();
      KeyCode::KeyCode ex_remapKeyCode = KeyCode::NONE;
      RemapConsumerParams remapParams = {
        params, &ex_remapKeyCode,
      };
      NumHeldDownKeys::set(remapParams);
      remap_consumer(remapParams);

      // ----------------------------------------
      ListHookedKeyboard::Item *hk = ListHookedKeyboard::get();
      unsigned int keyboardType = KeyboardType::MACBOOK;

      if (hk) {
        Params_KeyboardEventCallBack callbackparams = {
          hk->getOrig_keyboardEventTarget(), params->eventType, params->flags, ex_remapKeyCode,
          0, 0, 0, 0,
          keyboardType, false, params->ts, hk->get(), NULL,
        };

        if (ex_remapKeyCode != KeyCode::NONE) {
          callbackparams.apply(hk->getOrig_keyboardEventAction());
          setRepeat_keyboard(hk->get(), callbackparams);
        }
        listFireExtraKey.fire(hk->getOrig_keyboardEventAction(), callbackparams);
      }

      RemapUtil::fireConsumer(p->getOrig_keyboardSpecialEventAction(), *params);
    }

    void
    remap_RelativePointerEventCallback(Params_RelativePointerEventCallback *params)
    {
      if (! params) return;

      IOHIPointing *pointing = OSDynamicCast(IOHIPointing, params->sender);
      if (! pointing) return;

      ListHookedPointing::Item *p = ListHookedPointing::get(pointing);
      if (! p) return;

      // ------------------------------------------------------------
      last_pointing = pointing;
      params_lastRelativePointer = *params;

      // ------------------------------------------------------------
      params->log();

      listFireRelativePointer.reset();

      if (params->buttons) {
        cancelAllKeyRepeat();
      }

      bool ex_dropEvent = false;
      RemapPointingParams_relative remapParams = {
        params, &ex_dropEvent,
      };
      remap_pointing_relative_core(remapParams);

      RelativePointerEventCallback reCallback = p->getOrig_relativePointerEventAction();

      if (! ex_dropEvent) {
        params->apply(reCallback);
      }

      if (! listFireRelativePointer.isEmpty()) {
        listFireRelativePointer.fire(reCallback, params->target, pointing, params->ts);
      }

      if (firePointingScroll.isEnable()) {
        firePointingScroll.fire(p->getOrig_scrollWheelEventAction(), p->getOrig_scrollWheelEventTarget(), pointing, params->ts);
      }
    }

    void
    remap_ScrollWheelEventCallback(Params_ScrollWheelEventCallback *params)
    {
      if (! params) return;

      IOHIPointing *pointing = OSDynamicCast(IOHIPointing, params->sender);
      if (! pointing) return;

      ListHookedPointing::Item *p = ListHookedPointing::get(pointing);
      if (! p) return;

      // ------------------------------------------------------------
      last_pointing = pointing;
      params_lastScrollWheel = *params;

      // ------------------------------------------------------------
      params->log();

      cancelAllKeyRepeat();
      params->apply(p->getOrig_scrollWheelEventAction());
    }
  }
}
