#include "EventWatcher.hpp"
#include "RemapUtil.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace EventWatcher {
    enum {
      MAXNUM = 32,
    };
    bool* item[MAXNUM];

    void
    reset(void)
    {
      for (int i = 0; i < MAXNUM; ++i) {
        item[i] = NULL;
      }
    }

    void
    click(void)
    {
      for (int i = 0; i < MAXNUM; ++i) {
        if (item[i]) {
          *(item[i]) = true;
        }
      }
    }

    void
    set(bool* b)
    {
      if (b == NULL) return;

      *b = false;
      for (int i = 0; i < MAXNUM; ++i) {
        if (item[i] == NULL) {
          item[i] = b;
        }
      }
    }

    void
    unset(bool* b)
    {
      if (b == NULL) return;

      for (int i = 0; i < MAXNUM; ++i) {
        if (item[i] == b) {
          item[i] = NULL;
        }
      }
    }
  }
}
