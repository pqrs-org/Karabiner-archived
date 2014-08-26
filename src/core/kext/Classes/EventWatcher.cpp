#include "Config.hpp"
#include "EventInputQueue.hpp"
#include "EventWatcher.hpp"
#include "IOLogWrapper.hpp"

namespace org_pqrs_Karabiner {
  List EventWatcher::list_;

  void
  EventWatcher::initialize(void)
  {
  }

  void
  EventWatcher::terminate(void)
  {
    list_.clear();
  }

  void
  EventWatcher::reset(void)
  {
    list_.clear();
  }

  void
  EventWatcher::on(void)
  {
    IOLOG_DEVEL("EventWatcher::on (list_.size:%d)\n", static_cast<int>(list_.size()));

    for (Item* p = static_cast<Item*>(list_.safe_front()); p; p = static_cast<Item*>(p->getnext())) {
      p->on();
    }
  }

  void
  EventWatcher::undo(void)
  {
    for (Item* p = static_cast<Item*>(list_.safe_front()); p; p = static_cast<Item*>(p->getnext())) {
      p->undo();
    }
  }

  void
  EventWatcher::set(bool& b)
  {
    b = false;
    list_.push_back(new Item(b));
  }

  void
  EventWatcher::unset(bool& b)
  {
    Item* p = static_cast<Item*>(list_.safe_front());
    for (;;) {
      if (! p) break;

      if (p->isSameAddress(b)) {
        p = static_cast<Item*>(list_.erase_and_delete(p));
      } else {
        p = static_cast<Item*>(p->getnext());
      }
    }
  }

  void
  EventWatcher::Item::on(void)
  {
    if (! isAnyEventHappen_) {
      cancelableEventInputQueueSerialNumber_ = EventInputQueue::currentSerialNumber();
    }
    isAnyEventHappen_ = true;
  }

  void
  EventWatcher::Item::undo(void)
  {
    if (cancelableEventInputQueueSerialNumber_ == EventInputQueue::currentSerialNumber()) {
      isAnyEventHappen_ = false;
    }
  }
}
