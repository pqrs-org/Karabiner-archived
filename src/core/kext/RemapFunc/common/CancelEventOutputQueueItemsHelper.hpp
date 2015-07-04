#ifndef CANCELEVENTOUTPUTQUEUEITEMSHELPER_HPP
#define CANCELEVENTOUTPUTQUEUEITEMSHELPER_HPP

#include "EventOutputQueue.hpp"
#include "RemapFuncBase.hpp"

namespace org_pqrs_Karabiner {
namespace RemapFunc {
class CancelEventOutputQueueItemsHelper final {
public:
  void registerSerialNumber(void) {
    auto sn = EventInputQueue::currentSerialNumber();
    if (targetSerialNumbers_.is_include(sn)) {
      return;
    }

    // unregister old values
    for (size_t i = 0; i < targetSerialNumbers_.size(); ++i) {
      if (targetSerialNumbers_[i] != SerialNumber(0)) {
        enum {
          THRESHOLD = 10,
        };
        if (targetSerialNumbers_[i] + THRESHOLD < sn) {
          targetSerialNumbers_[i].set(SerialNumber(0));
        }
      }
    }

    for (size_t i = 0; i < targetSerialNumbers_.size(); ++i) {
      if (targetSerialNumbers_[i] == SerialNumber(0)) {
        targetSerialNumbers_[i].set(sn);
        return;
      }
    }
    targetSerialNumbers_.push_back(sn);
  }

  bool isTarget(const EventOutputQueue::Item& item) {
    return targetSerialNumbers_.is_include(item.getEventInputQueueSerialNumber());
  }

private:
  EventInputQueue::Vector_SerialNumber targetSerialNumbers_;
};
}
}

#endif
