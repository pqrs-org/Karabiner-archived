#ifndef PRESSDOWNKEYS_HPP
#define PRESSDOWNKEYS_HPP

#include "base.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  // ----------------------------------------------------------------------
  // store RemapUtil::fireKey results
  namespace PressDownKeys {
    void initialize(void);
    void add(unsigned int _key, unsigned int _keyboardType);
    void remove(unsigned int _key, unsigned int _keyboardType);
    void clear(KeyboardEventCallback callback, OSObject *target, AbsoluteTime ts, OSObject *sender, void *refcon);
  }
}

#endif
