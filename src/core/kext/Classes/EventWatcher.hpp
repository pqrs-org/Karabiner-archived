#ifndef EVENTWATCHER_HPP
#define EVENTWATCHER_HPP

#include "List.hpp"
#include "EventInputQueue.hpp"

namespace org_pqrs_Karabiner {
class EventWatcher final {
private:
  class Item;

public:
  class Target final {
  public:
    Target(void) {
      item_ = new Item();
      if (item_) {
        list_.push_back(item_);
      }
    }

    ~Target(void) {
      if (item_) {
        list_.erase_and_delete(item_);
      }
    }

    void observe(void) {
      if (item_) {
        item_->observe();
      }
    }
    void unobserve(void) {
      if (item_) {
        item_->unobserve();
      }
    }
    bool isAnyEventHappen(void) const {
      if (!item_) return false;
      return item_->isAnyEventHappen();
    }

  private:
    Item* item_;
  };

  static void initialize(void);
  static void terminate(void);

  static void reset(void);
  static void on(void);

private:
  class Item final : public List::Item {
  public:
    Item(void) : observed_(false),
                 isAnyEventHappen_(false) {}
    virtual ~Item(void) {}

    void on(void);

    bool observed(void) const { return observed_; }
    void observe(void) {
      observed_ = true;
      isAnyEventHappen_ = false;
    }
    void unobserve(void) { observed_ = false; }

    bool isAnyEventHappen(void) const { return isAnyEventHappen_; }

  private:
    bool observed_;
    bool isAnyEventHappen_;
  };

  static List list_;
};
}

#endif
