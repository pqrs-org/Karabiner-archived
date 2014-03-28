#include "Config.hpp"
#include "EventInputQueue.hpp"
#include "EventWatcher.hpp"
#include "IOLogWrapper.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  List* EventWatcher::list_;

  void
  EventWatcher::initialize(void)
  {
    list_ = new List();
  }

  void
  EventWatcher::terminate(void)
  {
    if (list_) {
      delete list_;
    }
  }

  void
  EventWatcher::reset(void)
  {
    if (! list_) return;

    list_->clear();
  }

  void
  EventWatcher::on(void)
  {
    if (! list_) return;

    IOLOG_DEVEL("EventWatcher::on (list_->size:%d)\n", static_cast<int>(list_->size()));

    for (Item* p = static_cast<Item*>(list_->front()); p; p = static_cast<Item*>(p->getnext())) {
      p->on();
    }
  }

  void
  EventWatcher::undo(void)
  {
    if (! list_) return;

    for (Item* p = static_cast<Item*>(list_->front()); p; p = static_cast<Item*>(p->getnext())) {
      p->undo();
    }
  }

  void
  EventWatcher::set(bool& b)
  {
    if (! list_) return;

    b = false;
    list_->push_back(new Item(b));
  }

  void
  EventWatcher::unset(bool& b)
  {
    if (! list_) return;

    Item* p = static_cast<Item*>(list_->front());
    for (;;) {
      if (! p) break;

      if (p->isSameAddress(b)) {
        p = static_cast<Item*>(list_->erase_and_delete(p));
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
