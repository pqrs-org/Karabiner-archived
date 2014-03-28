#ifndef EVENTWATCHER_HPP
#define EVENTWATCHER_HPP

#include "List.hpp"
#include "EventInputQueue.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  class EventWatcher {
  public:
    static void initialize(void);
    static void terminate(void);

    static void reset(void);
    static void on(void);
    static void undo(void);

    static void set(bool& b);
    static void unset(bool& b);

  private:
    class Item : public List::Item {
    public:
      Item(bool& b) : isAnyEventHappen_(b) {}
      virtual ~Item(void) {}

      void on(void);
      void undo(void);

      bool isSameAddress(bool& b) const { return &isAnyEventHappen_ == &b; }

    private:
      uint64_t cancelableEventInputQueueSerialNumber_;
      bool& isAnyEventHappen_;
    };
    static List* list_;
  };
}

#endif
