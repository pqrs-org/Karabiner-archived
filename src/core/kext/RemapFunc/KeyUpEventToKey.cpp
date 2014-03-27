#include "KeyUpEventToKey.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace RemapFunc {
    void
    KeyUpEventToKey::add(unsigned int datatype, unsigned int newval)
    {
      keytokey_.add(datatype, newval);
    }

    bool
    KeyUpEventToKey::remap(RemapParams& remapParams)
    {
      return false;
    }
  }
}
