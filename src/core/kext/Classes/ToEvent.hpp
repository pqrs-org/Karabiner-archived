#ifndef TOEVENT_HPP
#define TOEVENT_HPP

#include "KeyCode.hpp"
#include "Vector.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  class ToEvent {
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

    explicit ToEvent(void)              : type_(Type::NONE)                          {}
    explicit ToEvent(KeyCode v)         : type_(Type::KEY),             key_(v)      {}
    explicit ToEvent(ConsumerKeyCode v) : type_(Type::CONSUMER_KEY),    consumer_(v) {}
    explicit ToEvent(PointingButton v)  : type_(Type::POINTING_BUTTON), button_(v)   {}

    Flags getFlags(void) const { return flags_; }
    void setFlags(Flags v) { flags_ = v; }

    // XXX: remove me
    Type::Value getType(void) const { return type_; }
    KeyCode getKeyCode(void) const { return key_; }

    // Get ModifierFlag from KeyCode.
    ModifierFlag getModifierFlag(void) const {
      if (type_ != Type::KEY) return ModifierFlag::NONE;
      return key_.getModifierFlag();
    }
    bool isEventLikeModifier(void) const;

  private:
    Type::Value type_;
    Flags flags_;
    KeyCode key_;
    ConsumerKeyCode consumer_;
    PointingButton button_;
  };

  DECLARE_VECTOR(ToEvent);
}

#endif
