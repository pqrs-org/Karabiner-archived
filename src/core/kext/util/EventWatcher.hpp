#ifndef EVENTWATCHER_HPP
#define EVENTWATCHER_HPP

#include "base.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  class EventWatcher {
  public:
    static void initialize(void);
    static void terminate(void);

    static void reset(void);
    static void on(void);

    static void set(bool& b);
    static void unset(bool& b);

  private:
    enum {
      MAXNUM = 32,
    };
    static bool* item_[MAXNUM];
    static IOLock* lock_;
  };
}

#endif
