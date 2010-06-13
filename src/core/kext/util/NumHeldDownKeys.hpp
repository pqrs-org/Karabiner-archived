#ifndef NUMHELDDOWNKEYS_HPP
#define NUMHELDDOWNKEYS_HPP

#include "KeyCode.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  class NumHeldDownKeys {
  public:
    static void set(EventType eventType, KeyCode key, Flags flags);
    static void set(EventType eventType);
    static void set(int diff);
    static void reset(void);
    static bool iszero(void);

  private:
    static void log(void);

    static int num_;
  };
}

#endif
