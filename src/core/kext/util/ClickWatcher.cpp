#include "ClickWatcher.hpp"
#include "RemapUtil.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  void
  ClickWatcher::reset(void)
  {
    for (int i = 0; i < MAXNUM; ++i) {
      watchlist[i] = NULL;
    }
  }

  void
  ClickWatcher::click(void)
  {
    for (int i = 0; i < MAXNUM; ++i) {
      if (watchlist[i]) {
        *(watchlist[i]) = true;
      }
    }
  }

  void
  ClickWatcher::set(bool *b)
  {
    if (b == NULL) return;

    *b = false;
    for (int i = 0; i < MAXNUM; ++i) {
      if (watchlist[i] == NULL) {
        watchlist[i] = b;
      }
    }
  }

  void
  ClickWatcher::unset(bool *b)
  {
    if (b == NULL) return;

    for (int i = 0; i < MAXNUM; ++i) {
      if (watchlist[i] == b) {
        watchlist[i] = NULL;
      }
    }
  }
}
