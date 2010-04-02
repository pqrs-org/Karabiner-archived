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
    bool operator==(EventType other) const { return value_ == other.get(); }
    bool operator!=(EventType other) const { return ! (*this == other); }

    bool isKeyDownOrModifierDown(KeyCode key, Flags flags) const;

#include "keycode/output/include.EventType.hpp"

  private:
    unsigned int value_;
  };

  // ======================================================================
  class KeyboardType {
  public:
    KeyboardType(unsigned int v = 0) : value_(v) {}
    unsigned int get(void) const { return value_; }
    bool operator==(KeyboardType other) const { return value_ == other.get(); }
    bool operator!=(KeyboardType other) const { return ! (*this == other); }

#include "keycode/output/include.KeyboardType.hpp"

  private:
    unsigned int value_;
  };

  // ======================================================================
  class ModifierFlag {
  public:
    unsigned int get(void) const { return value_; }
    bool operator==(ModifierFlag other) const { return value_ == other.get(); }
    bool operator!=(ModifierFlag other) const { return ! (*this == other); }

    unsigned int operator~(void) const { return ~value_; }

    KeyCode getKeyCode(void) const;

#include "keycode/output/include.ModifierFlag.hpp"

  private:
    ModifierFlag(unsigned int v) : value_(v) {}
    unsigned int value_;
  };
  class Flags {
  public:
    Flags(unsigned int v = 0) : value_(v) {}
    Flags(ModifierFlag v) : value_(v.get()) {}
    unsigned int get(void) const { return value_; }
    bool operator==(Flags other) const { return value_ == other.get(); }
    bool operator!=(Flags other) const { return ! (*this == other); }

    unsigned int operator~(void) const { return ~value_; }
    Flags operator|(Flags other) const { return value_ | other.get(); }
    Flags operator&(Flags other) const { return value_ & other.get(); }

    Flags& add(Flags flags) { value_ |= flags.get(); return *this; }
    Flags& remove(Flags flags) { value_ &= ~flags; return *this; }
    Flags& stripFN(void)     { return remove(ModifierFlag::FN); }
    Flags& stripCURSOR(void) { return remove(ModifierFlag::CURSOR); }
    Flags& stripKEYPAD(void) { return remove(ModifierFlag::KEYPAD); }
    Flags& stripNONE(void)   { return remove(ModifierFlag::NONE); }

    bool isOn(ModifierFlag flag) const {
      return (value_ & flag.get()) == flag.get();
    }
    bool isOn(Flags flags) const {
      if (flags.isOn(ModifierFlag::NONE)) {
        return (value_ | ModifierFlag::NONE.get()) == flags.get();
      } else {
        return (value_ & flags.get()) == flags.get();
      }
    }

  private:
    unsigned int value_;
  };
  inline Flags operator|(ModifierFlag lhs, ModifierFlag rhs) { return lhs.get() | rhs.get(); }

  // ======================================================================
  class KeyCode {
  public:
    KeyCode(unsigned int v = 0) : value_(v) {}
    unsigned int get(void) const { return value_; }
    bool operator==(KeyCode other) const { return value_ == other.get(); }
    bool operator!=(KeyCode other) const { return ! (*this == other); }

    bool operator>(KeyCode other) const { return value_ > other.get(); }
    bool operator>=(KeyCode other) const { return value_ >= other.get(); }

    void normalizeKey(Flags& flags, KeyboardType keyboardType);
    void reverseNormalizeKey(Flags& flags, KeyboardType keyboardType);

    ModifierFlag getModifierFlag(void) const;
    bool isModifier(void) const { return getModifierFlag() != ModifierFlag::NONE; }

#include "keycode/output/include.KeyCode.hpp"

  private:
    unsigned int value_;
  };

  class CharCode {
  public:
    CharCode(unsigned int v = 0) : value_(v) {}
    unsigned int get(void) const { return value_; }
    bool operator==(CharCode other) const { return value_ == other.get(); }
    bool operator!=(CharCode other) const { return ! (*this == other); }

  private:
    unsigned int value_;
  };
  class CharSet {
  public:
    CharSet(unsigned int v = 0) : value_(v) {}
    unsigned int get(void) const { return value_; }
    bool operator==(CharSet other) const { return value_ == other.get(); }
    bool operator!=(CharSet other) const { return ! (*this == other); }

  private:
    unsigned int value_;
  };
  class OrigCharCode {
  public:
    OrigCharCode(unsigned int v = 0) : value_(v) {}
    unsigned int get(void) const { return value_; }
    bool operator==(OrigCharCode other) const { return value_ == other.get(); }
    bool operator!=(OrigCharCode other) const { return ! (*this == other); }

  private:
    unsigned int value_;
  };
  class OrigCharSet {
  public:
    OrigCharSet(unsigned int v = 0) : value_(v) {}
    unsigned int get(void) const { return value_; }
    bool operator==(OrigCharSet other) const { return value_ == other.get(); }
    bool operator!=(OrigCharSet other) const { return ! (*this == other); }

  private:
    unsigned int value_;
  };

  // ======================================================================
  class ConsumerKeyCode {
  public:
    ConsumerKeyCode(unsigned int v = 0) : value_(v) {}
    unsigned int get(void) const { return value_; }
    bool operator==(ConsumerKeyCode other) const { return value_ == other.get(); }
    bool operator!=(ConsumerKeyCode other) const { return ! (*this == other); }

    bool operator>(ConsumerKeyCode other) const { return value_ > other.get(); }
    bool operator>=(ConsumerKeyCode other) const { return value_ >= other.get(); }

#include "keycode/output/include.ConsumerKeyCode.hpp"

  private:
    unsigned int value_;
  };

  // ======================================================================
  class PointingButton {
  public:
    unsigned int get(void) const { return value_; }
    bool operator==(PointingButton other) const { return value_ == other.get(); }
    bool operator!=(PointingButton other) const { return ! (*this == other); }

    unsigned int operator~(void) const { return ~value_; }

#include "keycode/output/include.PointingButton.hpp"

  private:
    PointingButton(unsigned int v) : value_(v) {}
    unsigned int value_;
  };
  class Buttons {
  public:
    Buttons(unsigned int v = 0) : value_(v) {}
    Buttons(PointingButton v) : value_(v.get()) {}
    unsigned int get(void) const { return value_; }
    bool operator==(Buttons other) const { return value_ == other.get(); }
    bool operator!=(Buttons other) const { return ! (*this == other); }

    unsigned int operator~(void) const { return ~value_; }
    Buttons operator|(Buttons other) const { return value_ | other.get(); }

    Buttons& add(Buttons button) { value_ |= button.get(); return *this; }
    Buttons& remove(Buttons button) { value_ &= ~button; return *this; }

    bool isNONE(void) const { return value_ == 0; }
    bool isOn(Buttons buttons) const {
      return (value_ & buttons.get()) == buttons.get();
    }

  private:
    unsigned int value_;
  };
  inline Buttons operator|(PointingButton lhs, PointingButton rhs) { return lhs.get() | rhs.get(); }

  // ======================================================================
  typedef unsigned int DeviceVendorID;
  typedef unsigned int DeviceProductID;
}

#endif
