#include "CallBackWrapper.hpp"
#include "Config.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  void
  Params_KeyboardEventCallBack::log(void) const
  {
    if (! config.debug) return;

    printf("KeyRemap4MacBook caught KeyboardEventCallback: eventType %d, flags 0x%x, key %d, kbdType %d\n",
           eventType, flags, key, keyboardType);
  }

  void
  Params_KeyboardSpecialEventCallback::log(void) const
  {
    if (! config.debug) return;

    printf("KeyRemap4MacBook caught KeyboardSpecialEventCallBack: eventType %d, flags 0x%x, key %d, flavor %d, guid %d\n",
           eventType, flags, key, flavor, guid);
  }

  void
  Params_RelativePointerEventCallback::log(void) const
  {
    if (! config.debug_pointing) return;

    printf("KeyRemap4MacBook caught RelativePointerEventCallBack: buttons: %d, dx: %d, dy: %d, ts: 0x%x\n",
           buttons, dx, dy, ts);
  }

  void
  Params_ScrollWheelEventCallback::log(void) const
  {
    if (! config.debug_pointing) return;

    printf("KeyRemap4MacBook caught ScrollWheelEventCallback: deltaAxis(%d, %d, %d), fixedDelta(%d, %d, %d), pointDelta(%d,%d,%d), options: %d\n",
           deltaAxis1, deltaAxis2, deltaAxis3,
           fixedDelta1, fixedDelta2, fixedDelta3,
           pointDelta1, pointDelta2, pointDelta3,
           options);
  }
}
