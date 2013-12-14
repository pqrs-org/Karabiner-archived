#ifndef FROMEVENT_HPP
#define FROMEVENT_HPP

#include "EventInputQueue.hpp"
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

    FromEvent(void)              : isPressing_(false), type_(Type::NONE)                          {}
    FromEvent(KeyCode v)         : isPressing_(false), type_(Type::KEY),             key_(v)      {}
    FromEvent(ConsumerKeyCode v) : isPressing_(false), type_(Type::CONSUMER_KEY),    consumer_(v) {}
    FromEvent(PointingButton v)  : isPressing_(false), type_(Type::POINTING_BUTTON), button_(v)   {}

    bool isTargetDownEvent(const EventInputQueue::Item& item) const;
    bool isTargetUpEvent(const EventInputQueue::Item& item) const;

    // Get ModifierFlag from KeyCode.
    ModifierFlag getModifierFlag(void) const {
      if (type_ != Type::KEY) return ModifierFlag::NONE;
      return key_.getModifierFlag();
    }

  private:
    bool isTargetEvent(bool& isDown, const EventInputQueue::Item& item) const;

    bool isPressing_;

    // Do not store Flags in FromEvent because SimultaneousKeyPresses uses multiple FromEvents.

    Type::Value type_;
    KeyCode key_;
    ConsumerKeyCode consumer_;
    PointingButton button_;
  };
}

#endif
