#ifndef KEYCODE_HPP
#define KEYCODE_HPP

namespace org_pqrs_KeyRemap4MacBook {
  class ParamsItem {
  public:
    ParamsItem(void) {}
    ParamsItem(unsigned int v) : value_(v) {}

    unsigned int get(void) const { return value_; }
    void set(unsigned int v) { value_ = v; }

    bool operator==(const unsigned int& other) const { return value_ == other; }
    bool operator==(const ParamsItem& other) const { return value_ == other.get(); }
    bool operator!=(const unsigned int& other) const { return ! (*this == other); }
    bool operator!=(const ParamsItem& other) const { return ! (*this == other); }

  protected:
    unsigned int value_;
  };

  // ======================================================================
  class EventType : public ParamsItem {
  public:
    EventType(void) : ParamsItem() {}
    EventType(unsigned int v) : ParamsItem(v) {}

#include "keycode/output/include.EventType.hpp"
  };

  // ======================================================================
  class KeyboardType : public ParamsItem {
  public:
    KeyboardType(void) : ParamsItem() {}
    KeyboardType(unsigned int v) : ParamsItem(v) {}

    bool isInternalKeyboard(void) const;

#include "keycode/output/include.KeyboardType.hpp"
  };

  // ======================================================================
  class KeyCode;
  class Flags;

  class ModifierFlag : public ParamsItem {
  public:
    static const ModifierFlag list[];
    static const int listsize;

    const KeyCode& getKeyCode(void) const;

    unsigned int operator~(void) const { return ~value_; }
    Flags operator|(const ModifierFlag& other) const;

#include "keycode/output/include.ModifierFlag.hpp"

  private:
    ModifierFlag(void) : ParamsItem() {}
    ModifierFlag(unsigned int v) : ParamsItem(v) {}
  };
  class Flags : public ParamsItem {
  public:
    Flags(void) : ParamsItem() {}
    Flags(unsigned int v) : ParamsItem(v) {}
    Flags(const ModifierFlag& v) : ParamsItem(v.get()) {}

    Flags operator|(const Flags& other)        const { return value_ | other.get(); }
    Flags operator|(const ModifierFlag& other) const { return value_ | other.get(); }

    Flags& add(const ModifierFlag& flag) { value_ |= flag.get(); return *this; }
    Flags& stripFN(void)     { value_ &= ~ModifierFlag::FN;     return *this; }
    Flags& stripCURSOR(void) { value_ &= ~ModifierFlag::CURSOR; return *this; }
    Flags& stripKEYPAD(void) { value_ &= ~ModifierFlag::KEYPAD; return *this; }
    Flags& stripNONE(void)   { value_ &= ~ModifierFlag::NONE;   return *this; }

    bool isOn(const ModifierFlag& flag) const {
      return (value_ & flag.get()) == flag.get();
    }
  };

  // ======================================================================
  class KeyCode : public ParamsItem {
  public:
    KeyCode(void) : ParamsItem() {}
    KeyCode(unsigned int v) : ParamsItem(v) {}

    void normalizeKey(Flags & flags, const KeyboardType &keyboardType);
    void reverseNormalizeKey(Flags & flags, const KeyboardType &keyboardType);

    ModifierFlag getModifierFlag(void) const;
    bool isModifier(void) const { return getModifierFlag() != ModifierFlag::NONE; }

#include "keycode/output/include.KeyCode.hpp"
  };

  // ======================================================================
  class ConsumerKeyCode : public ParamsItem {
  public:
    ConsumerKeyCode(void) : ParamsItem() {}
    ConsumerKeyCode(unsigned int v) : ParamsItem(v) {}

#include "keycode/output/include.ConsumerKeyCode.hpp"
  };

  // ======================================================================
  class PointingButton : public ParamsItem {
  public:
#include "keycode/output/include.PointingButton.hpp"

  private:
    PointingButton(void) : ParamsItem() {}
    PointingButton(unsigned int v) : ParamsItem(v) {}
  };
  class Buttons : public ParamsItem {
  public:
    Buttons(void) : ParamsItem() {}
    Buttons(unsigned int v) : ParamsItem(v) {}
  };
}

#endif
