#ifndef EVENTWATCHER_HPP
#define EVENTWATCHER_HPP

#include "base.hpp"
#include "List.hpp"

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
    class Item : public List::Item {
    public:
      Item(bool& b) : isAnyEventHappen(b) {}
      virtual ~Item(void) {}

      bool& isAnyEventHappen;
    };
    static List* list_;
    static IOLock* lock_;
  };
}

#endif
