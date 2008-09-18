#include "Core.hpp"
#include "Config.hpp"
#include "RemapUtil.hpp"
#include "Client.hpp"
#include "ListHookedKeyboard.hpp"
#include "ListHookedPointing.hpp"
#include "util/TimerWrapper.hpp"
#include "remap.hpp"

#include <IOKit/IOWorkLoop.h>
#include <IOKit/IOTimerEventSource.h>

namespace org_pqrs_KeyRemap4MacBook {
  namespace Core {
    namespace {
      IOWorkLoop *workLoop = NULL;
      TimerWrapper timer_refresh;

      TimerWrapper timer_repeat;
      Params_KeyboardEventCallBack repeat;

      TimerWrapper timer_extraRepeat;
      struct ExtraRepeatInfo {
        unsigned int flags;
        unsigned int keyboardType;
        AbsoluteTime ts;
        OSObject *target;
        OSObject *sender;
        void *refcon;
        org_pqrs_KeyRemap4MacBook::ExtraRepeatFunc::ExtraRepeatFunc func;
        unsigned int counter;
      } extraRepeat;

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
    }

    void
    initialize(void)
    {
      sysctl_register();
      clickWatcher.reset();
      pressDownKeys.initialize();
      KeyRemap4MacBook_client::initialize();

      workLoop = IOWorkLoop::workLoop();
      if (workLoop) {
        timer_refresh.initialize(workLoop, NULL, refreshHookedDevice);
        timer_refresh.setTimeoutMS(REFRESH_DEVICE_INTERVAL);
      }
    }

    void
    terminate(void)
    {
      timer_refresh.terminate();

      sysctl_unregister();
    }

    void
    start(void)
    {
      const int INTERVAL = 1000;
      timer_refresh.setTimeoutMS(INTERVAL);

    }

    void
    stop(void)
    {
      timer_refresh.terminate();

      if (workLoop) {
        workLoop->release();
        workLoop = NULL;
      }

      ListHookedKeyboard::terminate();
      ListHookedPointing::terminate();
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
      if (params == NULL) return;

      IOHIKeyboard *kbd = OSDynamicCast(IOHIKeyboard, params->sender);
      if (! kbd) return;

      ListHookedKeyboard::Item *p = ListHookedKeyboard::get(kbd);
      if (! p) return;

      // ------------------------------------------------------------
      // Because the key repeat generates it by oneself, I throw it away.
#if 0
      if (repeat) {
        (p->repeat).ts = ts;
        (p->extraRepeat).ts = ts;
        return;
      }
#endif

      if (config.debug) {
        printf("KeyRemap4MacBook caught KeyboardEventCallback: eventType %d, flags 0x%x, key %d, kbdType %d\n",
               params->eventType, params->flags, params->key, params->keyboardType);
      }

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
        extraRepeat.counter,
      };

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

#if 0
      p->setExtraRepeatInfo(ex_extraRepeatFunc, ex_extraRepeatFlags, params->keyboardType, params->ts, params->target, params->refcon);
      p->setRepeatInfo(params);
#endif

      if (allFlagStatus.numHeldDownKeys <= 0) {
        params->flags = 0;
        RemapUtil::fireModifiers(p->getOrig_keyboardEventAction(), *params);
        pressDownKeys.clear(p->getOrig_keyboardEventAction(), params->target, params->ts, params->sender, params->refcon);
      }
    }

    void
    remap_KeyboardSpecialEventCallback(Params_KeyboardSpecialEventCallback *params)
    {
      if (params == NULL) return;

      IOHIKeyboard *kbd = OSDynamicCast(IOHIKeyboard, params->sender);
      if (! kbd) return;

      ListHookedKeyboard::Item *p = ListHookedKeyboard::get(kbd);
      if (! p) return;

      if (config.debug) {
        printf("KeyRemap4MacBook caught keyboardSpecialEventCallBack: eventType %d, flags 0x%x, key %d, flavor %d, guid %d\n",
               params->eventType, params->flags, params->key, params->flavor, params->guid);
      }

      listFireExtraKey.reset();
      KeyCode::KeyCode ex_remapKeyCode = KeyCode::NONE;
      RemapConsumerParams remapParams = {
        params, &ex_remapKeyCode,
      };
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
          RemapUtil::execCallBack_KeyboardEventCallBack(hk->getOrig_keyboardEventAction(), callbackparams);
#if 0
          hk->setRepeatInfo(callbackparams);
#endif
        }
        listFireExtraKey.fire(hk->getOrig_keyboardEventAction(), callbackparams);
      }

      RemapUtil::fireConsumer(p->getOrig_keyboardSpecialEventAction(), *params);
    }
  }
}
