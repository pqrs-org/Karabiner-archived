#include <IOKit/IOLib.h>

#include "CallBackWrapper.hpp"
#include "CommonData.hpp"
#include "Config.hpp"
#include "FlagStatus.hpp"
#include "FromKeyChecker.hpp"
#include "IOLogWrapper.hpp"
#include "KeyCode.hpp"
#include "ListHookedConsumer.hpp"
#include "ListHookedKeyboard.hpp"
#include "ListHookedPointing.hpp"
#include "RemapClass.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  void
  Params_KeyboardEventCallBack::log(bool isCaught, EventType eventType, Flags flags, KeyCode key, KeyboardType keyboardType, bool repeat)
  {
    IOLOG_DEBUG("KeyboardEventCallback [%7s]: eventType %2d, flags 0x%08x, key 0x%04x, kbdType %3d, repeat = %d\n",
                isCaught ? "caught" : "sending",
                eventType.get(), flags.get(), key.get(), keyboardType.get(), repeat);
  }

  void
  Params_UpdateEventFlagsCallback::log(bool isCaught, Flags flags)
  {
    IOLOG_DEBUG("UpdateEventFlagsCallback [%7s]: flags 0x%08x\n",
                isCaught ? "caught" : "sending",
                flags.get());
  }

  void
  Params_KeyboardSpecialEventCallback::log(bool isCaught, EventType eventType, Flags flags, ConsumerKeyCode key, unsigned int flavor, UInt64 guid, bool repeat)
  {
    IOLOG_DEBUG("KeyboardSpecialEventCallBack [%7s]: eventType %2d, flags 0x%08x, key 0x%04x, flavor %4d, guid %lld, repeat = %d\n",
                isCaught ? "caught" : "sending",
                eventType.get(), flags.get(), key.get(), flavor, guid, repeat);
  }

  void
  Params_RelativePointerEventCallback::log(bool isCaught, Buttons buttons, int dx, int dy)
  {
    IOLOG_DEBUG_POINTING("RelativePointerEventCallBack [%7s]: buttons: 0x%08x, dx: %3d, dy: %3d\n",
                         isCaught ? "caught" : "sending",
                         buttons.get(), dx, dy);
  }

  void
  Params_ScrollWheelEventCallback::log(bool isCaught,
                                       short deltaAxis1,
                                       short deltaAxis2,
                                       short deltaAxis3,
                                       IOFixed fixedDelta1,
                                       IOFixed fixedDelta2,
                                       IOFixed fixedDelta3,
                                       SInt32 pointDelta1,
                                       SInt32 pointDelta2,
                                       SInt32 pointDelta3,
                                       SInt32 options)
  {
#if __x86_64__
    IOLOG_DEBUG_POINTING("ScrollWheelEventCallback [%7s]: deltaAxis(%d,%d,%d), fixedDelta(%d,%d,%d), pointDelta(%d,%d,%d), options: %d\n",
                         isCaught ? "caught" : "sending",
                         deltaAxis1, deltaAxis2, deltaAxis3,
                         fixedDelta1, fixedDelta2, fixedDelta3,
                         pointDelta1, pointDelta2, pointDelta3,
                         options);
#else
    IOLOG_DEBUG_POINTING("ScrollWheelEventCallback [%7s]: deltaAxis(%d,%d,%d), fixedDelta(%ld,%ld,%ld), pointDelta(%ld,%ld,%ld), options: %ld\n",
                         isCaught ? "caught" : "sending",
                         deltaAxis1, deltaAxis2, deltaAxis3,
                         fixedDelta1, fixedDelta2, fixedDelta3,
                         pointDelta1, pointDelta2, pointDelta3,
                         options);
#endif
  }
}
