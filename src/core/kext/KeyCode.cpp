#include "KeyCode.hpp"

namespace org_pqrs_KeyRemap4MacBook {
#include "keycode/output/include.EventType.cpp"
#include "keycode/output/include.KeyboardType.cpp"
#include "keycode/output/include.ModifierFlag.cpp"
#include "keycode/output/include.KeyCode.cpp"
#include "keycode/output/include.ConsumerKeyCode.cpp"
#include "keycode/output/include.PointingButton.cpp"

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

  void
  KeyCode::normalizeKey(Flags& flags, KeyboardType keyboardType)
  {
    if (keyboardType == KeyboardType::POWERBOOK ||
        keyboardType == KeyboardType::POWERBOOK_G4 ||
        keyboardType == KeyboardType::POWERBOOK_G4_TI) {
      if (*this == KeyCode::ENTER_POWERBOOK) { *this = KeyCode::ENTER; }
    }

    if (flags.isOn(ModifierFlag::FN)) {
      // Note: KEYPAD_CLEAR has no ModifierFlag::KEYPAD bit.
      if (*this == KeyCode::KEYPAD_0)        { *this = KeyCode::M;            flags.stripKEYPAD(); }
      if (*this == KeyCode::KEYPAD_1)        { *this = KeyCode::J;            flags.stripKEYPAD(); }
      if (*this == KeyCode::KEYPAD_2)        { *this = KeyCode::K;            flags.stripKEYPAD(); }
      if (*this == KeyCode::KEYPAD_3)        { *this = KeyCode::L;            flags.stripKEYPAD(); }
      if (*this == KeyCode::KEYPAD_4)        { *this = KeyCode::U;            flags.stripKEYPAD(); }
      if (*this == KeyCode::KEYPAD_5)        { *this = KeyCode::I;            flags.stripKEYPAD(); }
      if (*this == KeyCode::KEYPAD_6)        { *this = KeyCode::O;            flags.stripKEYPAD(); }
      if (*this == KeyCode::KEYPAD_7)        { *this = KeyCode::KEY_7;        flags.stripKEYPAD(); }
      if (*this == KeyCode::KEYPAD_8)        { *this = KeyCode::KEY_8;        flags.stripKEYPAD(); }
      if (*this == KeyCode::KEYPAD_9)        { *this = KeyCode::KEY_9;        flags.stripKEYPAD(); }
      if (*this == KeyCode::KEYPAD_CLEAR)    { *this = KeyCode::KEY_6; }
      if (*this == KeyCode::KEYPAD_PLUS)     { *this = KeyCode::SLASH;        flags.stripKEYPAD(); }
      if (*this == KeyCode::KEYPAD_MINUS)    { *this = KeyCode::SEMICOLON;    flags.stripKEYPAD(); }
      if (*this == KeyCode::KEYPAD_MULTIPLY) { *this = KeyCode::P;            flags.stripKEYPAD(); }
      if (*this == KeyCode::KEYPAD_SLASH)    { *this = KeyCode::KEY_0;        flags.stripKEYPAD(); }
      if (*this == KeyCode::KEYPAD_EQUAL)    { *this = KeyCode::MINUS;        flags.stripKEYPAD(); }
      if (*this == KeyCode::KEYPAD_DOT)      { *this = KeyCode::DOT;          flags.stripKEYPAD(); }
      if (*this == KeyCode::PAGEUP)          { *this = KeyCode::CURSOR_UP;    flags.add(ModifierFlag::CURSOR); }
      if (*this == KeyCode::PAGEDOWN)        { *this = KeyCode::CURSOR_DOWN;  flags.add(ModifierFlag::CURSOR); }
      if (*this == KeyCode::HOME)            { *this = KeyCode::CURSOR_LEFT;  flags.add(ModifierFlag::CURSOR); }
      if (*this == KeyCode::END)             { *this = KeyCode::CURSOR_RIGHT; flags.add(ModifierFlag::CURSOR); }
      if (*this == KeyCode::ENTER)           { *this = KeyCode::RETURN; }
      if (*this == KeyCode::FORWARD_DELETE)  { *this = KeyCode::DELETE; }
    }
  }

  void
  KeyCode::reverseNormalizeKey(Flags& flags, KeyboardType keyboardType)
  {
    if (flags.isOn(ModifierFlag::FN)) {
      if (*this == KeyCode::M)            { *this = KeyCode::KEYPAD_0; }
      if (*this == KeyCode::J)            { *this = KeyCode::KEYPAD_1; }
      if (*this == KeyCode::K)            { *this = KeyCode::KEYPAD_2; }
      if (*this == KeyCode::L)            { *this = KeyCode::KEYPAD_3; }
      if (*this == KeyCode::U)            { *this = KeyCode::KEYPAD_4; }
      if (*this == KeyCode::I)            { *this = KeyCode::KEYPAD_5; }
      if (*this == KeyCode::O)            { *this = KeyCode::KEYPAD_6; }
      if (*this == KeyCode::KEY_7)        { *this = KeyCode::KEYPAD_7; }
      if (*this == KeyCode::KEY_8)        { *this = KeyCode::KEYPAD_8; }
      if (*this == KeyCode::KEY_9)        { *this = KeyCode::KEYPAD_9; }
      if (*this == KeyCode::KEY_6)        { *this = KeyCode::KEYPAD_CLEAR; }
      if (*this == KeyCode::SLASH)        { *this = KeyCode::KEYPAD_PLUS; }
      if (*this == KeyCode::SEMICOLON)    { *this = KeyCode::KEYPAD_MINUS; }
      if (*this == KeyCode::P)            { *this = KeyCode::KEYPAD_MULTIPLY; }
      if (*this == KeyCode::KEY_0)        { *this = KeyCode::KEYPAD_SLASH; }
      if (*this == KeyCode::MINUS)        { *this = KeyCode::KEYPAD_EQUAL; }
      if (*this == KeyCode::DOT)          { *this = KeyCode::KEYPAD_DOT; }
      if (*this == KeyCode::CURSOR_UP)    { *this = KeyCode::PAGEUP; }
      if (*this == KeyCode::CURSOR_DOWN)  { *this = KeyCode::PAGEDOWN; }
      if (*this == KeyCode::CURSOR_LEFT)  { *this = KeyCode::HOME; }
      if (*this == KeyCode::CURSOR_RIGHT) { *this = KeyCode::END; }
      if (*this == KeyCode::RETURN)       { *this = KeyCode::ENTER; }
      if (*this == KeyCode::DELETE)       { *this = KeyCode::FORWARD_DELETE; }
    }
    if (keyboardType == KeyboardType::POWERBOOK ||
        keyboardType == KeyboardType::POWERBOOK_G4 ||
        keyboardType == KeyboardType::POWERBOOK_G4_TI) {
      if (*this == KeyCode::ENTER) { *this = KeyCode::ENTER_POWERBOOK; }
    }

    // ------------------------------------------------------------
    // set ModifierFlag::FN
    //
    // We add ModifierFlag::FN for Cocoa Application.
    // Cocoa Application manages the flag status inside,
    // so unless we attach FN, the flag status becomes invalid in Cocoa.
    if (*this == KeyCode::HOME ||
        *this == KeyCode::END ||
        *this == KeyCode::PAGEUP ||
        *this == KeyCode::PAGEDOWN ||
        *this == KeyCode::FORWARD_DELETE ||
        *this == KeyCode::HELP) {
      flags.add(ModifierFlag::FN);
    }

    // ------------------------------------------------------------
    // set ModifierFlag::KEYPAD, ModifierFlag::CURSOR
    flags.stripCURSOR().stripKEYPAD();

    // Note: KEYPAD_CLEAR, KEYPAD_COMMA have no ModifierFlag::KEYPAD bit.
    if (*this == KeyCode::KEYPAD_0 || *this == KeyCode::KEYPAD_1 || *this == KeyCode::KEYPAD_2 ||
        *this == KeyCode::KEYPAD_3 || *this == KeyCode::KEYPAD_4 || *this == KeyCode::KEYPAD_5 ||
        *this == KeyCode::KEYPAD_6 || *this == KeyCode::KEYPAD_7 || *this == KeyCode::KEYPAD_8 ||
        *this == KeyCode::KEYPAD_9 ||
        *this == KeyCode::KEYPAD_DOT ||
        *this == KeyCode::KEYPAD_MULTIPLY ||
        *this == KeyCode::KEYPAD_PLUS ||
        *this == KeyCode::KEYPAD_SLASH ||
        *this == KeyCode::KEYPAD_MINUS ||
        *this == KeyCode::KEYPAD_EQUAL) {
      flags.add(ModifierFlag::KEYPAD);
    }

    if (*this == KeyCode::CURSOR_UP ||
        *this == KeyCode::CURSOR_DOWN ||
        *this == KeyCode::CURSOR_LEFT ||
        *this == KeyCode::CURSOR_RIGHT) {
      flags.add(ModifierFlag::CURSOR);
    }
  }

  KeyCode
  ModifierFlag::getKeyCode(void) const {
    if (*this == ModifierFlag::CAPSLOCK) return KeyCode::CAPSLOCK;
    if (*this == ModifierFlag::SHIFT_L) return KeyCode::SHIFT_L;
    if (*this == ModifierFlag::SHIFT_R) return KeyCode::SHIFT_R;
    if (*this == ModifierFlag::CONTROL_L) return KeyCode::CONTROL_L;
    if (*this == ModifierFlag::CONTROL_R) return KeyCode::CONTROL_R;
    if (*this == ModifierFlag::OPTION_L) return KeyCode::OPTION_L;
    if (*this == ModifierFlag::OPTION_R) return KeyCode::OPTION_R;
    if (*this == ModifierFlag::COMMAND_L) return KeyCode::COMMAND_L;
    if (*this == ModifierFlag::COMMAND_R) return KeyCode::COMMAND_R;
    if (*this == ModifierFlag::FN) return KeyCode::FN;
    if (*this == ModifierFlag::EXTRA1) return KeyCode::VK_MODIFIER_EXTRA1;
    if (*this == ModifierFlag::EXTRA2) return KeyCode::VK_MODIFIER_EXTRA2;
    if (*this == ModifierFlag::EXTRA3) return KeyCode::VK_MODIFIER_EXTRA3;
    if (*this == ModifierFlag::EXTRA4) return KeyCode::VK_MODIFIER_EXTRA4;
    if (*this == ModifierFlag::EXTRA5) return KeyCode::VK_MODIFIER_EXTRA5;

    return KeyCode::VK_NONE;
  }

  ModifierFlag
  KeyCode::getModifierFlag(void) const
  {
    if (*this == KeyCode::CAPSLOCK) return ModifierFlag::CAPSLOCK;
    if (*this == KeyCode::SHIFT_L) return ModifierFlag::SHIFT_L;
    if (*this == KeyCode::SHIFT_R) return ModifierFlag::SHIFT_R;
    if (*this == KeyCode::CONTROL_L) return ModifierFlag::CONTROL_L;
    if (*this == KeyCode::CONTROL_R) return ModifierFlag::CONTROL_R;
    if (*this == KeyCode::OPTION_L) return ModifierFlag::OPTION_L;
    if (*this == KeyCode::OPTION_R) return ModifierFlag::OPTION_R;
    if (*this == KeyCode::COMMAND_L) return ModifierFlag::COMMAND_L;
    if (*this == KeyCode::COMMAND_R) return ModifierFlag::COMMAND_R;
    if (*this == KeyCode::FN) return ModifierFlag::FN;
    if (*this == KeyCode::VK_MODIFIER_EXTRA1) return ModifierFlag::EXTRA1;
    if (*this == KeyCode::VK_MODIFIER_EXTRA2) return ModifierFlag::EXTRA2;
    if (*this == KeyCode::VK_MODIFIER_EXTRA3) return ModifierFlag::EXTRA3;
    if (*this == KeyCode::VK_MODIFIER_EXTRA4) return ModifierFlag::EXTRA4;
    if (*this == KeyCode::VK_MODIFIER_EXTRA5) return ModifierFlag::EXTRA5;

    return ModifierFlag::NONE;
  }
}
