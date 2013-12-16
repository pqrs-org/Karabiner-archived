#ifndef FROMEVENT_HPP
#define FROMEVENT_HPP

#include "KeyCode.hpp"
#include "ParamsUnion.hpp"
#include "Vector.hpp"

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

    // Return whether pressing state is changed.
    bool changePressingState(const ParamsUnion& paramsUnion, Flags currentFlags, Flags fromFlags);
    bool isPressing(void) const { return isPressing_; }
    void unsetPressingState(void) { isPressing_ = false; }

    // Primitive functions:
    // These functions do not treat Flags.
    // Use changePressingState in general.
    bool isTargetDownEvent(const ParamsUnion& paramsUnion) const;
    bool isTargetUpEvent(const ParamsUnion& paramsUnion) const;

    // Get ModifierFlag from KeyCode.
    ModifierFlag getModifierFlag(void) const {
      if (type_ != Type::KEY) return ModifierFlag::NONE;
      return key_.getModifierFlag();
    }

  private:
    bool isTargetEvent(bool& isDown, const ParamsUnion& paramsUnion) const;

    bool isPressing_;

    // Do not store Flags in FromEvent because SimultaneousKeyPresses uses multiple FromEvents.

    Type::Value type_;
    KeyCode key_;
    ConsumerKeyCode consumer_;
    PointingButton button_;
  };

  DECLARE_VECTOR(FromEvent);
}

#endif
