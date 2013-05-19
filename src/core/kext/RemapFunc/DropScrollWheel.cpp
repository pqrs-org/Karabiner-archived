#include "DropScrollWheel.hpp"
#include "EventOutputQueue.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace RemapFunc {
    DropScrollWheel::DropScrollWheel(void) : dropHorizontalScroll_(false)
    {}

    DropScrollWheel::~DropScrollWheel(void)
    {}

    void
    DropScrollWheel::add(unsigned int datatype, unsigned int newval)
    {
      switch (datatype) {
        case BRIDGE_DATATYPE_OPTION:
        {
          if (Option::DROPSCROLLWHEEL_DROP_HORIZONTAL_SCROLL == newval) {
            dropHorizontalScroll_ = true;
          } else {
            IOLOG_ERROR("DropScrollWheel::add unknown option:%d\n", newval);
          }
          break;
        }

        default:
          IOLOG_ERROR("DropScrollWheel::add invalid datatype:%d\n", datatype);
          break;
      }
    }

    bool
    DropScrollWheel::remap(RemapPointingParams_scroll& remapParams)
    {
      if (remapParams.isremapped) return false;
      remapParams.isremapped = true;

      if (dropHorizontalScroll_) {
        // Overwrite horizontal values with 0.
        // We should not drop events which vertical values are 0
        // because it might cause a vertical scroll stuck in some apps (Adobe Photoshop, Illustrator).

        Params_ScrollWheelEventCallback::auto_ptr ptr(Params_ScrollWheelEventCallback::alloc(remapParams.params.deltaAxis1,
                                                                                             0,
                                                                                             0,
                                                                                             remapParams.params.fixedDelta1,
                                                                                             0,
                                                                                             0,
                                                                                             remapParams.params.pointDelta1,
                                                                                             0,
                                                                                             0,
                                                                                             remapParams.params.options));
        if (ptr) {
          EventOutputQueue::FireScrollWheel::fire(*ptr);
        }
      }

      return true;
    }
  }
}
