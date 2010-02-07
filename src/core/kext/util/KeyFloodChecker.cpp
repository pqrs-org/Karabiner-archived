#include "KeyFloodChecker.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  void
  KeyFloodChecker::add(void)
  {
    for (int i = 0; i < MAXNUM; ++i) {
      if (! item[i].active) {
        item[i].active = true;
        item[i].ic.begin();
        return;
      }
    }
  }

  bool
  KeyFloodChecker::check(uint32_t millisec, int allowdnum) {
    int count = 0;

    for (int i = 0; i < MAXNUM; ++i) {
      if (! item[i].active) continue;

      if (item[i].ic.getmillisec() < millisec) {
        ++count;
      } else {
        // drop
        item[i].active = false;
      }
    }

    return count <= allowdnum;
  }
}
