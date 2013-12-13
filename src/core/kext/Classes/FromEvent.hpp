#ifndef FROMEVENT_HPP
#define FROMEVENT_HPP

#include "KeyCode.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  class FromEvent {
  public:
    class Type {
    public:
      enum Value {
        NONE,
        KEY,
        CONSUMER_KEY, // Mute, VolumeIncrement, VolumeDecrement, etcetc.
        POINTING_BUTTON,
      };
    };

    FromEvent(void)              : type_(Type::NONE)                          {}
    FromEvent(KeyCode v)         : type_(Type::KEY),             key_(v)      {}
    FromEvent(ConsumerKeyCode v) : type_(Type::CONSUMER_KEY),    consumer_(v) {}
    FromEvent(PointingButton v)  : type_(Type::POINTING_BUTTON), button_(v)   {}

    bool isTargetDown(const EventInputQueue::Item& item) const;
    bool isTargetUp(const EventInputQueue::Item& item) const;

  private:
    bool isTarget(bool& isDown, const EventInputQueue::Item& item) const;

    // Do not store Flags in FromEvent because SimultaneousKeyPresses uses multiple FromEvents.

    Type::Value type_;
    KeyCode key_;
    ConsumerKeyCode consumer_;
    PointingButton button_;
  };
}

#endif
