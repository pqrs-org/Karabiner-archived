#include "CallBackWrapper.hpp"
#include "Config.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  void
  Params_KeyboardEventCallBack::log(const char *message) const
  {
    if (! config.debug) return;

    printf("KeyRemap4MacBook KeyboardEventCallback [%s]: eventType %d, flags 0x%x, key %d, kbdType %d\n",
           message, eventType, flags, key, keyboardType);
  }

  void
  Params_KeyboardSpecialEventCallback::log(const char *message) const
  {
    if (! config.debug) return;

    printf("KeyRemap4MacBook KeyboardSpecialEventCallBack [%s]: eventType %d, flags 0x%x, key %d, flavor %d, guid %d\n",
           message, eventType, flags, key, flavor, guid);
  }

  void
  Params_RelativePointerEventCallback::log(const char *message) const
  {
    if (! config.debug_pointing) return;

    printf("KeyRemap4MacBook RelativePointerEventCallBack [%s]: buttons: %d, dx: %d, dy: %d, ts: 0x%x\n",
           message, buttons, dx, dy, ts);
  }

  void
  Params_ScrollWheelEventCallback::log(const char *message) const
  {
    if (! config.debug_pointing) return;

    printf("KeyRemap4MacBook ScrollWheelEventCallback [%s]: deltaAxis(%d, %d, %d), fixedDelta(%d, %d, %d), pointDelta(%d,%d,%d), options: %d\n",
           message,
           deltaAxis1, deltaAxis2, deltaAxis3,
           fixedDelta1, fixedDelta2, fixedDelta3,
           pointDelta1, pointDelta2, pointDelta3,
           options);
  }

  // ----------------------------------------------------------------------
  void
  Params_KeyboardEventCallBack::apply(KeyboardEventCallback callback)
  {
    if (! callback) return;

    log("sending");
    callback(target, eventType, flags, key, charCode, charSet, origCharCode, origCharSet,
             keyboardType, repeat, ts, sender, refcon);
  }

  void
  Params_KeyboardSpecialEventCallback::apply(KeyboardSpecialEventCallback callback)
  {
    if (! callback) return;

    log("sending");
    callback(target, eventType, flags, key, flavor, guid, repeat, ts, sender, refcon);
  }

  void
  Params_RelativePointerEventCallback::apply(RelativePointerEventCallback callback)
  {
    if (! callback) return;

    log("sending");
    callback(target, buttons, dx, dy, ts, sender, refcon);
  }

  void
  Params_ScrollWheelEventCallback::apply(ScrollWheelEventCallback callback)
  {
    if (! callback) return;

    log("sending");
    callback(target,
             deltaAxis1, deltaAxis2, deltaAxis3,
             fixedDelta1, fixedDelta2, fixedDelta3,
             pointDelta1, pointDelta2, pointDelta3,
             options, ts, sender, refcon);
  }
}
