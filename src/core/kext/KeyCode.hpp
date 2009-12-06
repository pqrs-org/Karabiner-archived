#ifndef KEYCODE_HPP
#define KEYCODE_HPP

namespace org_pqrs_KeyRemap4MacBook {
  class KeyCode;
  class Flags;
  class Buttons;

  // ======================================================================
  class EventType {
  public:
    EventType(unsigned int v = 0) : value_(v) {}
    unsigned int get(void) const { return value_; }
    bool operator==(const EventType& other) const { return value_ == other.get(); }
    bool operator!=(const EventType& other) const { return ! (*this == other); }

    bool isKeyDownOrModifierDown(const KeyCode& key, const Flags& flags) const;

#include "keycode/output/include.EventType.hpp"

  private:
    unsigned int value_;
  };

  // ======================================================================
  class KeyboardType {
  public:
    KeyboardType(unsigned int v = 0) : value_(v) {}
    unsigned int get(void) const { return value_; }
    bool operator==(const KeyboardType& other) const { return value_ == other.get(); }
    bool operator!=(const KeyboardType& other) const { return ! (*this == other); }

    bool isInternalKeyboard(void) const;

#include "keycode/output/include.KeyboardType.hpp"

  private:
    unsigned int value_;
  };

  // ======================================================================
  class ModifierFlag {
  public:
    unsigned int get(void) const { return value_; }
    bool operator==(const ModifierFlag& other) const { return value_ == other.get(); }
    bool operator!=(const ModifierFlag& other) const { return ! (*this == other); }

    unsigned int operator~(void) const { return ~value_; }

    const KeyCode& getKeyCode(void) const;

#include "keycode/output/include.ModifierFlag.hpp"

  private:
    ModifierFlag(unsigned int v) : value_(v) {}
    unsigned int value_;
  };
  class Flags {
  public:
    Flags(unsigned int v = 0) : value_(v) {}
    Flags(const ModifierFlag& v) : value_(v.get()) {}
    unsigned int get(void) const { return value_; }
    bool operator==(const Flags& other) const { return value_ == other.get(); }
    bool operator!=(const Flags& other) const { return ! (*this == other); }

    Flags operator|(const Flags& other) const { return value_ | other.get(); }
    Flags operator&(const Flags& other) const { return value_ & other.get(); }

    Flags& add(const Flags& flags) { value_ |= flags.get(); return *this; }
    Flags& stripFN(void)     { value_ &= ~ModifierFlag::FN;     return *this; }
    Flags& stripCURSOR(void) { value_ &= ~ModifierFlag::CURSOR; return *this; }
    Flags& stripKEYPAD(void) { value_ &= ~ModifierFlag::KEYPAD; return *this; }
    Flags& stripNONE(void)   { value_ &= ~ModifierFlag::NONE;   return *this; }

    bool isOn(const ModifierFlag& flag) const {
      return (value_ & flag.get()) == flag.get();
    }
    bool isOn(const Flags& flags) const {
      if (flags.isOn(ModifierFlag::NONE)) {
        return (value_ | ModifierFlag::NONE.get()) == flags.get();
      } else {
        return (value_ & flags.get()) == flags.get();
      }
    }

  private:
    unsigned int value_;
  };
  inline Flags operator|(const ModifierFlag& lhs, const ModifierFlag& rhs) { return lhs.get() | rhs.get(); }

  // ======================================================================
  class KeyCode {
  public:
    KeyCode(unsigned int v = 0) : value_(v) {}
    unsigned int get(void) const { return value_; }
    bool operator==(const KeyCode& other) const { return value_ == other.get(); }
    bool operator!=(const KeyCode& other) const { return ! (*this == other); }

    bool operator> (const KeyCode& other) const { return value_ > other.get(); }
    bool operator>=(const KeyCode& other) const { return value_ >= other.get(); }

    void normalizeKey(Flags& flags, const KeyboardType& keyboardType);
    void reverseNormalizeKey(Flags& flags, const KeyboardType& keyboardType);

    ModifierFlag getModifierFlag(void) const;
    bool isModifier(void) const { return getModifierFlag() != ModifierFlag::NONE; }

#include "keycode/output/include.KeyCode.hpp"

  private:
    unsigned int value_;
  };

  // ======================================================================
  class ConsumerKeyCode {
  public:
    ConsumerKeyCode(unsigned int v = 0) : value_(v) {}
    unsigned int get(void) const { return value_; }
    bool operator==(const ConsumerKeyCode& other) const { return value_ == other.get(); }
    bool operator!=(const ConsumerKeyCode& other) const { return ! (*this == other); }

    bool operator> (const ConsumerKeyCode& other) const { return value_ > other.get(); }
    bool operator>=(const ConsumerKeyCode& other) const { return value_ >= other.get(); }

#include "keycode/output/include.ConsumerKeyCode.hpp"

  private:
    unsigned int value_;
  };

  // ======================================================================
  class PointingButton {
  public:
    unsigned int get(void) const { return value_; }
    bool operator==(const PointingButton& other) const { return value_ == other.get(); }
    bool operator!=(const PointingButton& other) const { return ! (*this == other); }

    unsigned int operator~(void) const { return ~value_; }

#include "keycode/output/include.PointingButton.hpp"

  private:
    PointingButton(unsigned int v) : value_(v) {}
    unsigned int value_;
  };
  class Buttons {
  public:
    Buttons(unsigned int v = 0) : value_(v) {}
    Buttons(const PointingButton& v) : value_(v.get()) {}
    unsigned int get(void) const { return value_; }
    bool operator==(const Buttons& other) const { return value_ == other.get(); }
    bool operator!=(const Buttons& other) const { return ! (*this == other); }

    unsigned int operator~(void) const { return ~value_; }
    Buttons operator|(const Buttons& other) const { return value_ | other.get(); }

    Buttons& add(const Buttons& button) { value_ |= button.get(); return *this; }
    Buttons& remove(const Buttons& button) { value_ &= ~button; return *this; }

    bool isNONE(void) const { return value_ == 0; }
    bool isOn(const Buttons& buttons) const {
      return (value_ & buttons.get()) == buttons.get();
    }

  private:
    unsigned int value_;
  };
  inline Buttons operator|(const PointingButton& lhs, const PointingButton& rhs) { return lhs.get() | rhs.get(); }
}

#endif
