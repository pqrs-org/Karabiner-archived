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
    Flags& stripEXTRA(void) {
      return remove(Flags(ModifierFlag::EXTRA1) |
                    Flags(ModifierFlag::EXTRA2) |
                    Flags(ModifierFlag::EXTRA3) |
                    Flags(ModifierFlag::EXTRA4) |
                    Flags(ModifierFlag::EXTRA5));
    }

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

    void normalizeKey(Flags& flags, EventType eventType, KeyboardType keyboardType);
    void reverseNormalizeKey(Flags& flags, EventType eventType, KeyboardType keyboardType);

    ModifierFlag getModifierFlag(void) const;
    bool isModifier(void) const { return getModifierFlag() != ModifierFlag::NONE; }

#include "keycode/output/include.KeyCode.hpp"

    // When FN key and Arrow key were pushed together, another key code was sent (Home,End,PageUp,PageDown or something).
    // We need to change these Home,End,PageUp,PageDown keys to FN+Arrow key before sending key code to remapper.
    // (If we change FN to Control_L, FN+Up-Arrow must be changed to Control_L+Up-Arrow. Not Control_L+PageUp).
    // We also need to change FN+Arrow to Home,End,PageUp,PageDown before outputting key code.
    //
    // This class handles the above.
    class FNKeyHack {
    public:
      FNKeyHack(const KeyCode& fk, const KeyCode& tk) : fromKeyCode_(fk), toKeyCode_(tk), active_normalize_(false), active_reverse_(false) {}
      // FN+PageUp to FN+Up-Arrow
      bool normalize(KeyCode& key, Flags flags, EventType eventType) { return remap(key, flags, eventType, active_normalize_, fromKeyCode_, toKeyCode_); }
      // FN+Up-Arrow to PageUp
      bool reverse(KeyCode& key, Flags flags, EventType eventType) { return remap(key, flags, eventType, active_reverse_, toKeyCode_, fromKeyCode_); }

    private:
      bool remap(KeyCode& key, Flags flags, EventType eventType, bool& active, KeyCode fromKeyCode, KeyCode toKeyCode);

      const KeyCode& fromKeyCode_;
      const KeyCode& toKeyCode_;
      bool active_normalize_;
      bool active_reverse_;
    };

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
    PointingButton(unsigned int v = 0) : value_(v) {}
    unsigned int get(void) const { return value_; }
    bool operator==(PointingButton other) const { return value_ == other.get(); }
    bool operator!=(PointingButton other) const { return ! (*this == other); }

    unsigned int operator~(void) const { return ~value_; }

#include "keycode/output/include.PointingButton.hpp"

  private:
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

  namespace DeviceType {
    enum DeviceType {
      UNKNOWN,

      APPLE_INTERNAL,
      APPLE_EXTERNAL,

      USB_OVERDRIVE,
    };
  }
}

#endif
