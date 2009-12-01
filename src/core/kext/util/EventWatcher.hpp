#ifndef EVENTWATCHER_HPP
#define EVENTWATCHER_HPP

#include "base.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace EventWatcher {
    void reset(void);

    void on(void);

    void set(bool* b);
    void unset(bool* b);
  }
}

#endif
