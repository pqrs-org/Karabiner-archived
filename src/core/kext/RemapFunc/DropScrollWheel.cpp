#include "DropScrollWheel.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace RemapFunc {
    DropScrollWheel::DropScrollWheel(void)
    {}

    DropScrollWheel::~DropScrollWheel(void)
    {}

    void
    DropScrollWheel::add(unsigned int datatype, unsigned int newval)
    {}

    bool
    DropScrollWheel::remap(RemapPointingParams_scroll& remapParams)
    {
      remapParams.isremapped = true;
      return true;
    }
  }
}
