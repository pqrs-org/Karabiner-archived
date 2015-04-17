#include "Config.hpp"
#include "EventInputQueue.hpp"
#include "EventWatcher.hpp"
#include "IOLogWrapper.hpp"

namespace org_pqrs_Karabiner {
List EventWatcher::list_;

void EventWatcher::initialize(void) {}

void EventWatcher::terminate(void) {
  list_.clear();
}

void EventWatcher::reset(void) {
  for (Item* p = static_cast<Item*>(list_.safe_front()); p; p = static_cast<Item*>(p->getnext())) {
    p->unobserve();
  }
}

void EventWatcher::on(void) {
  IOLOG_DEVEL("EventWatcher::on (list_.size:%d)\n", static_cast<int>(list_.size()));

  for (Item* p = static_cast<Item*>(list_.safe_front()); p; p = static_cast<Item*>(p->getnext())) {
    if (p->observed()) {
      p->on();
    }
  }
}

void EventWatcher::Item::on(void) {
  isAnyEventHappen_ = true;
}
}
