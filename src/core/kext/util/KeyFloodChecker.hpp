#ifndef KEYFLOODCHECKER_HPP
#define KEYFLOODCHECKER_HPP

#include "IntervalChecker.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  class KeyFloodChecker {
  public:
    void add(void);
    bool check(uint32_t millisec, int allowdnum);

  private:
    enum {
      MAXNUM = 16,
    };
    struct Item {
      bool active;
      IntervalChecker ic;
    };
    Item item[MAXNUM];
  };
}

#endif
