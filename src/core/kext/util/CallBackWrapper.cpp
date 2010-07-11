#include "CallBackWrapper.hpp"
#include "CommonData.hpp"
#include "Config.hpp"
#include "FlagStatus.hpp"
#include "FromKeyChecker.hpp"
#include "KeyCode.hpp"
#include "ListHookedConsumer.hpp"
#include "ListHookedKeyboard.hpp"
#include "ListHookedPointing.hpp"
#include "RemapClass.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  void
  Params_KeyboardEventCallBack::log(const char* message) const
  {
    IOLOG_DEBUG("KeyboardEventCallback [%7s]: eventType %2d, flags 0x%08x, key %4d, kbdType %3d, repeat = %d\n",
                message, eventType.get(), flags.get(), key.get(), keyboardType.get(), repeat);
  }

  void
  Params_UpdateEventFlagsCallback::log(const char* message) const
  {
    IOLOG_DEBUG("UpdateEventFlagsCallback [%7s]: flags 0x%08x\n",
                message, flags.get());
  }

  void
  Params_KeyboardSpecialEventCallback::log(const char* message) const
  {
    IOLOG_DEBUG("KeyboardSpecialEventCallBack [%7s]: eventType %2d, flags 0x%08x, key %4d, flavor %4d, guid %lld, repeat = %d\n",
                message, eventType.get(), flags.get(), key.get(), flavor, guid, repeat);
  }

  void
  Params_RelativePointerEventCallback::log(const char* message) const
  {
    IOLOG_DEBUG_POINTING("RelativePointerEventCallBack [%7s]: buttons: 0x%08x, dx: %3d, dy: %3d\n",
                         message, buttons.get(), dx, dy);
  }

  void
  Params_ScrollWheelEventCallback::log(const char* message) const
  {
#if __x86_64__
    IOLOG_DEBUG_POINTING("ScrollWheelEventCallback [%7s]: deltaAxis(%d,%d,%d), fixedDelta(%d,%d,%d), pointDelta(%d,%d,%d), options: %d\n",
                         message,
                         deltaAxis1, deltaAxis2, deltaAxis3,
                         fixedDelta1, fixedDelta2, fixedDelta3,
                         pointDelta1, pointDelta2, pointDelta3,
                         options);
#else
    IOLOG_DEBUG_POINTING("ScrollWheelEventCallback [%7s]: deltaAxis(%d,%d,%d), fixedDelta(%ld,%ld,%ld), pointDelta(%ld,%ld,%ld), options: %ld\n",
                         message,
                         deltaAxis1, deltaAxis2, deltaAxis3,
                         fixedDelta1, fixedDelta2, fixedDelta3,
                         pointDelta1, pointDelta2, pointDelta3,
                         options);
#endif
  }

  void
  Params_KeyboardSpecialEventCallback::apply(void) const
  {
    if (key >= ConsumerKeyCode::VK__BEGIN__) {
      // Invalid keycode
      IOLOG_ERROR("Params_KeyboardSpecialEventCallback::apply invalid key:%d\n", key.get());
      return;
    }
    if (flags.isVirtualModifiersOn()) {
      IOLOG_ERROR("%s invalid flags:%d\n", __PRETTY_FUNCTION__, flags.get());
      return;
    }

    // ------------------------------------------------------------
    ListHookedConsumer::Item* hc = ListHookedConsumer::instance().get();
    if (! hc) return;

    KeyboardSpecialEventCallback callback = hc->getOrig_keyboardSpecialEventAction();
    if (! callback) return;

    OSObject* target = hc->getOrig_keyboardSpecialEventTarget();
    if (! target) return;

    OSObject* sender = OSDynamicCast(OSObject, hc->get());
    if (! sender) return;

    const AbsoluteTime& ts = CommonData::getcurrent_ts();
    OSObject* refcon = NULL;

    log("sending");
    callback(target, eventType.get(), flags.get(), key.get(),
             flavor, guid, repeat, ts, sender, refcon);

    // --------------------
    if (eventType == EventType::DOWN) {
      FlagStatus::sticky_clear();
    }
  }
}
