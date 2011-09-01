#include "StripModifierFromScrollWheel.hpp"
#include "EventOutputQueue.hpp"
#include "PointingRelativeToScroll.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace RemapFunc {
    StripModifierFromScrollWheel::StripModifierFromScrollWheel(void) : flags_(0)
    {}

    StripModifierFromScrollWheel::~StripModifierFromScrollWheel(void)
    {}

    void
    StripModifierFromScrollWheel::add(unsigned int datatype, unsigned int newval)
    {
      switch (datatype) {
        case BRIDGE_DATATYPE_FLAGS:
        {
          flags_ = newval;
          break;
        }

        default:
          IOLOG_ERROR("StripModifierFromScrollWheel::add invalid datatype:%d\n", datatype);
          break;
      }
    }

    bool
    StripModifierFromScrollWheel::remap(RemapPointingParams_scroll& remapParams)
    {
      if (remapParams.isremapped) return false;
      if (! FlagStatus::makeFlags().isOn(flags_)) return false;

      remapParams.isremapped = true;

      FlagStatus::temporary_strip(flags_);

      EventOutputQueue::FireScrollWheel::fire(remapParams.params);
      RemapFunc::PointingRelativeToScroll::cancelScroll();

      return true;
    }
  }
}
