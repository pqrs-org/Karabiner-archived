#ifndef NUMHELDDOWNKEYS_HPP
#define NUMHELDDOWNKEYS_HPP

#include "remap.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  class NumHeldDownKeys {
  public:
    static void set(const RemapParams& remapParams);
    static void set(const RemapConsumerParams& remapParams);
    static void set(int diff);
    static void reset(void);
    static bool iszero(void);

  private:
    static void log(void);

    static int num_;
  };
}

#endif
