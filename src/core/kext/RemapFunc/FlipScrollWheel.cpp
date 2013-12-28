#include <IOKit/IOLib.h>

#include "EventOutputQueue.hpp"
#include "FlipScrollWheel.hpp"
#include "IOLogWrapper.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace RemapFunc {
    FlipScrollWheel::FlipScrollWheel(void) : flipHorizontalScroll_(false), flipVerticalScroll_(false)
    {}

    FlipScrollWheel::~FlipScrollWheel(void)
    {}

    void
    FlipScrollWheel::add(unsigned int datatype, unsigned int newval)
    {
      switch (datatype) {
        case BRIDGE_DATATYPE_OPTION:
        {
          Option option(newval);
          /*  */ if (Option::FLIPSCROLLWHEEL_HORIZONTAL == option) {
            flipHorizontalScroll_ = true;
          } else if (Option::FLIPSCROLLWHEEL_VERTICAL == option) {
            flipVerticalScroll_ = true;
          } else {
            IOLOG_ERROR("FlipScrollWheel::add unknown option:%d\n", newval);
          }
          break;
        }

        default:
          IOLOG_ERROR("FlipScrollWheel::add invalid datatype:%d\n", datatype);
          break;
      }
    }

    bool
    FlipScrollWheel::remap(RemapPointingParams_scroll& remapParams)
    {
      if (remapParams.isremapped) return false;
      remapParams.isremapped = true;

      short da1   = remapParams.params.deltaAxis1;
      short da2   = remapParams.params.deltaAxis2;
      short da3   = remapParams.params.deltaAxis3;
      IOFixed fd1 = remapParams.params.fixedDelta1;
      IOFixed fd2 = remapParams.params.fixedDelta2;
      IOFixed fd3 = remapParams.params.fixedDelta3;
      SInt32 pd1  = remapParams.params.pointDelta1;
      SInt32 pd2  = remapParams.params.pointDelta2;
      SInt32 pd3  = remapParams.params.pointDelta3;

      if (flipHorizontalScroll_) {
        da2 = -da2;
        fd2 = -fd2;
        pd2 = -pd2;
      }
      if (flipVerticalScroll_) {
        da1 = -da1;
        fd1 = -fd1;
        pd1 = -pd1;
      }

      Params_ScrollWheelEventCallback::auto_ptr ptr(Params_ScrollWheelEventCallback::alloc(da1, da2, da3,
                                                                                           fd1, fd2, fd3,
                                                                                           pd1, pd2, pd3,
                                                                                           remapParams.params.options));
      if (ptr) {
        EventOutputQueue::FireScrollWheel::fire(*ptr);
      }

      return true;
    }
  }
}
