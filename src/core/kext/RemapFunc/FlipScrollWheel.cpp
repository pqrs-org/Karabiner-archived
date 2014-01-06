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
    FlipScrollWheel::remap(RemapParams& remapParams)
    {
      Params_ScrollWheelEventCallback* params = remapParams.paramsUnion.get_Params_ScrollWheelEventCallback();
      if (! params) return false;

      if (remapParams.isremapped) return false;
      remapParams.isremapped = true;

      short da1   = params->deltaAxis1;
      short da2   = params->deltaAxis2;
      short da3   = params->deltaAxis3;
      IOFixed fd1 = params->fixedDelta1;
      IOFixed fd2 = params->fixedDelta2;
      IOFixed fd3 = params->fixedDelta3;
      SInt32 pd1  = params->pointDelta1;
      SInt32 pd2  = params->pointDelta2;
      SInt32 pd3  = params->pointDelta3;

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
                                                                                           params->options));
      if (ptr) {
        EventOutputQueue::FireScrollWheel::fire(*ptr);
      }

      return true;
    }
  }
}
