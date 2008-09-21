#ifndef NUMHELDDOWNKEYS_HPP
#define NUMHELDDOWNKEYS_HPP

#include "remap.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace NumHeldDownKeys {
    void set(const RemapParams &remapParams);
    void set(const RemapConsumerParams &remapParams);
    void reset(void);
    bool iszero(void);
  }
}

#endif
