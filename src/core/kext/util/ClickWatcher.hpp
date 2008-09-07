#ifndef CLICKWATCHER_HPP
#define CLICKWATCHER_HPP

#include "base.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  class ClickWatcher {
  public:
    enum {
      MAXNUM = 8,
    };
    void reset(void);
    void click(void);

    void set(bool *b);
    void unset(bool *b);

  private:
    bool *watchlist[MAXNUM];
  };
}

#endif
