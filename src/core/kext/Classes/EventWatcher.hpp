#ifndef EVENTWATCHER_HPP
#define EVENTWATCHER_HPP

#include "List.hpp"
#include "EventInputQueue.hpp"

namespace org_pqrs_Karabiner {
  class EventWatcher {
  private:
    class Item;

  public:
    class Target {
    public:
      Target(void)
      {
        item = new Item();
        if (item) {
          list_.push_back(item);
        }
      }

      ~Target(void)
      {
        if (item) {
          list_.erase_and_delete(item);
        }
      }

      void observe(void)
      {
        if (item) {
          item->observe();
        }
      }
      void unobserve(void)
      {
        if (item) {
          item->unobserve();
        }
      }
      bool isAnyEventHappen(void) const
      {
        if (! item) return false;
        return item->isAnyEventHappen();
      }

    private:
      Item* item;
    };

    static void initialize(void);
    static void terminate(void);

    static void reset(void);
    static void on(void);
    static void undo(void);

  private:
    class Item : public List::Item {
    public:
      Item(void) :
        observed_(false),
        cancelableEventInputQueueSerialNumber_(0),
        isAnyEventHappen_(false)
      {}
      virtual ~Item(void) {}

      void on(void);
      void undo(void);

      bool observed(void) const { return observed_; }
      void observe(void)
      {
        observed_ = true;
        isAnyEventHappen_ = false;
      }
      void unobserve(void) { observed_ = false; }

      bool isAnyEventHappen(void) const { return isAnyEventHappen_; }

    private:
      bool observed_;
      uint64_t cancelableEventInputQueueSerialNumber_;
      bool isAnyEventHappen_;
    };

    static List list_;
  };
}

#endif
