#include <IOKit/IOLib.h>

#include "DropScrollWheel.hpp"
#include "EventOutputQueue.hpp"
#include "IOLogWrapper.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace RemapFunc {
    DropScrollWheel::DropScrollWheel(void) : dropHorizontalScroll_(false)
    {}

    DropScrollWheel::~DropScrollWheel(void)
    {}

    void
    DropScrollWheel::add(AddDataType datatype, AddValue newval)
    {
      switch (datatype) {
        case BRIDGE_DATATYPE_OPTION:
        {
          Option option(newval);
          if (Option::DROPSCROLLWHEEL_DROP_HORIZONTAL_SCROLL == option) {
            dropHorizontalScroll_ = true;
          } else if (Option::DROPSCROLLWHEEL_DROP_MOMENTUM_SCROLL == option) {
            dropMomentumScroll_ = true;
          } else {
            IOLOG_ERROR("DropScrollWheel::add unknown option:%u\n", static_cast<unsigned int>(newval));
          }
          break;
        }

        default:
          IOLOG_ERROR("DropScrollWheel::add invalid datatype:%u\n", static_cast<unsigned int>(datatype));
          break;
      }
    }

    bool
    DropScrollWheel::remap(RemapParams& remapParams)
    {
      Params_ScrollWheelEventCallback* params = remapParams.paramsUnion.get_Params_ScrollWheelEventCallback();
      if (! params) return false;

      if (remapParams.isremapped) return false;

      if (dropMomentumScroll_) {
        // Drop events which have kScrollTypeMomentumContinue.

        // see IOHIDSystem/IOHIDevicePrivateKeys.h about options.
        const int kScrollTypeMomentumContinue_ = 0x0004;
        if ((params->options & kScrollTypeMomentumContinue_) == 0) {
          return false;
        }
      }

      // ----------------------------------------
      remapParams.isremapped = true;

      if (dropHorizontalScroll_) {
        // Overwrite horizontal values with 0.
        // We should not drop events which vertical values are 0
        // because it might cause a vertical scroll stuck in some apps (Adobe Photoshop, Illustrator).

        Params_ScrollWheelEventCallback::auto_ptr ptr(Params_ScrollWheelEventCallback::alloc(params->deltaAxis1,
                                                                                             0,
                                                                                             0,
                                                                                             params->fixedDelta1,
                                                                                             0,
                                                                                             0,
                                                                                             params->pointDelta1,
                                                                                             0,
                                                                                             0,
                                                                                             params->options));
        if (ptr) {
          EventOutputQueue::FireScrollWheel::fire(*ptr);
        }
      }

      return true;
    }
  }
}
