#include "Config.hpp"
#include "KeyCode.hpp"
#include "KeyCodeModifierFlagPairs.hpp"

namespace org_pqrs_KeyRemap4MacBook {
#include "../../../src/bridge/output/include.kext.keycode.cpp"

  // ------------------------------------------------------------
  bool
  EventType::isKeyDownOrModifierDown(KeyCode key, Flags flags) const
  {
    if (*this == EventType::DOWN) return true;
    if (*this == EventType::MODIFY) {
      return flags.isOn(key.getModifierFlag());
    }
    return false;
  }

  bool
  KeyCode::FNKeyHack::remap(KeyCode& key, Flags flags, EventType eventType, bool& active, KeyCode fromKeyCode, KeyCode toKeyCode)
  {
    if (key != fromKeyCode) return false;

    bool isKeyDown = eventType.isKeyDownOrModifierDown(key, flags);
    if (isKeyDown) {
      if (! flags.isOn(ModifierFlag::FN)) return false;
      active = true;
    } else {
      if (! active) return false;
      active = false;
    }

    key = toKeyCode;

    return true;
  }

  namespace {
    KeyCode::FNKeyHack fnkeyhack[] = {
      KeyCode::FNKeyHack(KeyCode::KEYPAD_0, KeyCode::M),
      KeyCode::FNKeyHack(KeyCode::KEYPAD_1, KeyCode::J),
      KeyCode::FNKeyHack(KeyCode::KEYPAD_2, KeyCode::K),
      KeyCode::FNKeyHack(KeyCode::KEYPAD_3, KeyCode::L),
      KeyCode::FNKeyHack(KeyCode::KEYPAD_4, KeyCode::U),
      KeyCode::FNKeyHack(KeyCode::KEYPAD_5, KeyCode::I),
      KeyCode::FNKeyHack(KeyCode::KEYPAD_6, KeyCode::O),
      KeyCode::FNKeyHack(KeyCode::KEYPAD_7, KeyCode::KEY_7),
      KeyCode::FNKeyHack(KeyCode::KEYPAD_8, KeyCode::KEY_8),
      KeyCode::FNKeyHack(KeyCode::KEYPAD_9, KeyCode::KEY_9),
      KeyCode::FNKeyHack(KeyCode::KEYPAD_CLEAR, KeyCode::KEY_6),
      KeyCode::FNKeyHack(KeyCode::KEYPAD_PLUS, KeyCode::SLASH),
      KeyCode::FNKeyHack(KeyCode::KEYPAD_MINUS, KeyCode::SEMICOLON),
      KeyCode::FNKeyHack(KeyCode::KEYPAD_MULTIPLY, KeyCode::P),
      KeyCode::FNKeyHack(KeyCode::KEYPAD_SLASH, KeyCode::KEY_0),
      KeyCode::FNKeyHack(KeyCode::KEYPAD_EQUAL, KeyCode::MINUS),
      KeyCode::FNKeyHack(KeyCode::KEYPAD_DOT, KeyCode::DOT),
      KeyCode::FNKeyHack(KeyCode::PAGEUP, KeyCode::CURSOR_UP),
      KeyCode::FNKeyHack(KeyCode::PAGEDOWN, KeyCode::CURSOR_DOWN),
      KeyCode::FNKeyHack(KeyCode::HOME, KeyCode::CURSOR_LEFT),
      KeyCode::FNKeyHack(KeyCode::END, KeyCode::CURSOR_RIGHT),
      KeyCode::FNKeyHack(KeyCode::ENTER, KeyCode::RETURN),
      KeyCode::FNKeyHack(KeyCode::FORWARD_DELETE, KeyCode::DELETE),
    };
  }

  void
  KeyCode::normalizeKey(KeyCode& key, Flags& flags, EventType eventType, KeyboardType keyboardType)
  {
    // We can drop NUMPAD flags, because we'll set these flags at reverseNormalizeKey.
    flags.stripNUMPAD();

    if (keyboardType == KeyboardType::POWERBOOK ||
        keyboardType == KeyboardType::POWERBOOK_G4 ||
        keyboardType == KeyboardType::POWERBOOK_G4_TI) {
      if (key == KeyCode::ENTER_POWERBOOK) { key = KeyCode::ENTER; }
    }

    if (! Config::get_essential_config(BRIDGE_ESSENTIAL_CONFIG_INDEX_general_disable_numpad_hack)) {
      for (unsigned int i = 0; i < sizeof(fnkeyhack) / sizeof(fnkeyhack[0]); ++i) {
        if (fnkeyhack[i].normalize(key, flags, eventType)) break;
      }
    }
  }

  void
  KeyCode::reverseNormalizeKey(KeyCode& key, Flags& flags, EventType eventType, KeyboardType keyboardType)
  {
    if (! Config::get_essential_config(BRIDGE_ESSENTIAL_CONFIG_INDEX_general_disable_numpad_hack)) {
      for (unsigned int i = 0; i < sizeof(fnkeyhack) / sizeof(fnkeyhack[0]); ++i) {
        if (fnkeyhack[i].reverse(key, flags, eventType)) break;
      }
    }

    if (keyboardType == KeyboardType::POWERBOOK ||
        keyboardType == KeyboardType::POWERBOOK_G4 ||
        keyboardType == KeyboardType::POWERBOOK_G4_TI) {
      if (key == KeyCode::ENTER) { key = KeyCode::ENTER_POWERBOOK; }
    }

    // ------------------------------------------------------------
    // Don't add ModifierFlag::FN automatically for F-keys, PageUp/PageDown/Home/End and Forward Delete.
    //
    // PageUp/PageDown/Home/End and Forward Delete are entered by fn+arrow, fn+delete normally,
    // And, from Cocoa Application, F-keys and PageUp,... keys have Fn modifier
    // even if Fn key is not pressed actually.
    // So, it's natural adding ModifierFlag::FN to these keys.
    // However, there is a reason we must not add ModifierFlag::FN to there keys.
    //
    // Mission Control may have "fn" as shortcut key.
    // If we add ModifierFlag::FN here,
    // "XXX to PageUp" launches Mission Control because Mission Control recognizes fn key was pressed.
    //
    // It's not intended behavior from users.
    // Therefore, we don't add ModifierFlag::FN for these keys.

    // ------------------------------------------------------------
    // set ModifierFlag::NUMPAD
    flags.stripNUMPAD();

    // Note: KEYPAD_CLEAR, KEYPAD_COMMA have no ModifierFlag::NUMPAD bit.
    if (key == KeyCode::KEYPAD_0 || key == KeyCode::KEYPAD_1 || key == KeyCode::KEYPAD_2 ||
        key == KeyCode::KEYPAD_3 || key == KeyCode::KEYPAD_4 || key == KeyCode::KEYPAD_5 ||
        key == KeyCode::KEYPAD_6 || key == KeyCode::KEYPAD_7 || key == KeyCode::KEYPAD_8 ||
        key == KeyCode::KEYPAD_9 ||
        key == KeyCode::KEYPAD_DOT ||
        key == KeyCode::KEYPAD_MULTIPLY ||
        key == KeyCode::KEYPAD_PLUS ||
        key == KeyCode::KEYPAD_SLASH ||
        key == KeyCode::KEYPAD_MINUS ||
        key == KeyCode::KEYPAD_EQUAL) {
      flags.add(ModifierFlag::NUMPAD);
    }

    if (key == KeyCode::CURSOR_UP ||
        key == KeyCode::CURSOR_DOWN ||
        key == KeyCode::CURSOR_LEFT ||
        key == KeyCode::CURSOR_RIGHT) {
      flags.add(ModifierFlag::NUMPAD);
    }
  }

  KeyCode
  ModifierFlag::getKeyCode(void) const {
    return KeyCodeModifierFlagPairs::getKeyCode(*this);
  }

  unsigned int
  ModifierFlag::getRawBits(void) const
  {
    if (*this == ModifierFlag::CAPSLOCK)  { return 0x10000; }
    if (*this == ModifierFlag::SHIFT_L)   { return 0x20002; }
    if (*this == ModifierFlag::SHIFT_R)   { return 0x20004; }
    if (*this == ModifierFlag::CONTROL_L) { return 0x40001; }
    if (*this == ModifierFlag::CONTROL_R) { return 0x42000; }
    if (*this == ModifierFlag::OPTION_L)  { return 0x80020; }
    if (*this == ModifierFlag::OPTION_R)  { return 0x80040; }
    if (*this == ModifierFlag::COMMAND_L) { return 0x100008; }
    if (*this == ModifierFlag::COMMAND_R) { return 0x100010; }
    if (*this == ModifierFlag::NUMPAD)    { return 0x200000; }
    if (*this == ModifierFlag::FN)        { return 0x800000; }
    return 0;
  }

  Flags&
  Flags::remove(ModifierFlag flag)
  {
    // We consider the following case.
    //   (ModifierFlag::SHIFT_L | ModifierFlag::SHIFT_R).remove(ModifierFlag::SHIFT_L).
    //
    // The value of SHIFT_L and SHIFT_R is below.
    //
    // ModifierFlag::SHIFT_L : 0x20002
    // ModifierFlag::SHIFT_R : 0x20004
    //
    // So, the correct value of above case is 0x20004 (SHIFT_R).
    //
    // If we remove bits simple way (value_ &= ~flags),
    // the result value becomes 0x00004. It's not right.
    //
    // Therefore, we save the old value, and restore the necessary bits from it.
    //
    Flags old = *this;

    value_ = 0;

    auto& pairs = KeyCodeModifierFlagPairs::getPairs();
    for (size_t i = 0; i < pairs.size(); ++i) {
      ModifierFlag f = pairs[i].getModifierFlag();

      if (f != flag && old.isOn(f)) {
        value_ |= f.getRawBits();
      }
    }

    return *this;
  }

  ModifierFlag
  KeyCode::getModifierFlag(void) const
  {
    return KeyCodeModifierFlagPairs::getModifierFlag(*this);
  }

  bool
  ConsumerKeyCode::isRepeatable(void) const
  {
    if (*this == ConsumerKeyCode::BRIGHTNESS_DOWN)    { return true; }
    if (*this == ConsumerKeyCode::BRIGHTNESS_UP)      { return true; }
    if (*this == ConsumerKeyCode::KEYBOARDLIGHT_LOW)  { return true; }
    if (*this == ConsumerKeyCode::KEYBOARDLIGHT_HIGH) { return true; }
    if (*this == ConsumerKeyCode::MUSIC_PREV)         { return true; }
    if (*this == ConsumerKeyCode::MUSIC_NEXT)         { return true; }
    if (*this == ConsumerKeyCode::VOLUME_DOWN)        { return true; }
    if (*this == ConsumerKeyCode::VOLUME_UP)          { return true; }

    return false;
  }
}
