#include "keycode.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  bool
  KeyboardType::isInternalKeyboard(void)
  {
    if (value_ == KeyboardType::MACBOOK) return true;
    if (value_ == KeyboardType::MACBOOK_COREDUO) return true;
    if (value_ == KeyboardType::POWERBOOK) return true;
    if (value_ == KeyboardType::POWERBOOK_G4) return true;
    if (value_ == KeyboardType::POWERBOOK_G4_TI) return true;
    if (value_ == KeyboardType::JIS_MACBOOK) return true;
    if (value_ == KeyboardType::JIS_MACBOOK_2008) return true;
    return false;
  }

  void
  KeyCode::normalizeKey(unsigned int &key, unsigned int &flags, const KeyboardType& keyboardType)
  {
    if (keyboardType == KeyboardType::POWERBOOK ||
        keyboardType == KeyboardType::POWERBOOK_G4 ||
        keyboardType == KeyboardType::POWERBOOK_G4_TI) {
      if (key == KeyCode::ENTER_POWERBOOK) { key = KeyCode::ENTER; }
    }

    if (ModifierFlag::isOn(flags, ModifierFlag::FN)) {
      // Note: KEYPAD_CLEAR has no ModifierFlag::KEYPAD bit.
      if (key == KeyCode::KEYPAD_0)        { key = KeyCode::M;            flags = ModifierFlag::stripFN(ModifierFlag::stripKEYPAD(flags)); }
      if (key == KeyCode::KEYPAD_1)        { key = KeyCode::J;            flags = ModifierFlag::stripFN(ModifierFlag::stripKEYPAD(flags)); }
      if (key == KeyCode::KEYPAD_2)        { key = KeyCode::K;            flags = ModifierFlag::stripFN(ModifierFlag::stripKEYPAD(flags)); }
      if (key == KeyCode::KEYPAD_3)        { key = KeyCode::L;            flags = ModifierFlag::stripFN(ModifierFlag::stripKEYPAD(flags)); }
      if (key == KeyCode::KEYPAD_4)        { key = KeyCode::U;            flags = ModifierFlag::stripFN(ModifierFlag::stripKEYPAD(flags)); }
      if (key == KeyCode::KEYPAD_5)        { key = KeyCode::I;            flags = ModifierFlag::stripFN(ModifierFlag::stripKEYPAD(flags)); }
      if (key == KeyCode::KEYPAD_6)        { key = KeyCode::O;            flags = ModifierFlag::stripFN(ModifierFlag::stripKEYPAD(flags)); }
      if (key == KeyCode::KEYPAD_7)        { key = KeyCode::KEY_7;        flags = ModifierFlag::stripFN(ModifierFlag::stripKEYPAD(flags)); }
      if (key == KeyCode::KEYPAD_8)        { key = KeyCode::KEY_8;        flags = ModifierFlag::stripFN(ModifierFlag::stripKEYPAD(flags)); }
      if (key == KeyCode::KEYPAD_9)        { key = KeyCode::KEY_9;        flags = ModifierFlag::stripFN(ModifierFlag::stripKEYPAD(flags)); }
      if (key == KeyCode::KEYPAD_CLEAR)    { key = KeyCode::KEY_6;        flags = ModifierFlag::stripFN(flags); }
      if (key == KeyCode::KEYPAD_PLUS)     { key = KeyCode::SLASH;        flags = ModifierFlag::stripFN(ModifierFlag::stripKEYPAD(flags)); }
      if (key == KeyCode::KEYPAD_MINUS)    { key = KeyCode::SEMICOLON;    flags = ModifierFlag::stripFN(ModifierFlag::stripKEYPAD(flags)); }
      if (key == KeyCode::KEYPAD_MULTIPLY) { key = KeyCode::P;            flags = ModifierFlag::stripFN(ModifierFlag::stripKEYPAD(flags)); }
      if (key == KeyCode::KEYPAD_SLASH)    { key = KeyCode::KEY_0;        flags = ModifierFlag::stripFN(ModifierFlag::stripKEYPAD(flags)); }
      if (key == KeyCode::KEYPAD_EQUAL)    { key = KeyCode::MINUS;        flags = ModifierFlag::stripFN(ModifierFlag::stripKEYPAD(flags)); }
      if (key == KeyCode::KEYPAD_DOT)      { key = KeyCode::DOT;          flags = ModifierFlag::stripFN(ModifierFlag::stripKEYPAD(flags)); }
      if (key == KeyCode::PAGEUP)          { key = KeyCode::CURSOR_UP;    flags = ModifierFlag::stripFN(flags) | ModifierFlag::CURSOR; }
      if (key == KeyCode::PAGEDOWN)        { key = KeyCode::CURSOR_DOWN;  flags = ModifierFlag::stripFN(flags) | ModifierFlag::CURSOR; }
      if (key == KeyCode::HOME)            { key = KeyCode::CURSOR_LEFT;  flags = ModifierFlag::stripFN(flags) | ModifierFlag::CURSOR; }
      if (key == KeyCode::END)             { key = KeyCode::CURSOR_RIGHT; flags = ModifierFlag::stripFN(flags) | ModifierFlag::CURSOR; }
      if (key == KeyCode::ENTER)           { key = KeyCode::RETURN;       flags = ModifierFlag::stripFN(flags); }
      if (key == KeyCode::FORWARD_DELETE)  { key = KeyCode::DELETE;       flags = ModifierFlag::stripFN(flags); }
    }
  }

  void
  KeyCode::reverseNormalizeKey(unsigned int &key, unsigned int &flags, const KeyboardType& keyboardType)
  {
    if (ModifierFlag::isOn(flags, ModifierFlag::FN)) {
      if (key == KeyCode::M)            { key = KeyCode::KEYPAD_0; }
      if (key == KeyCode::J)            { key = KeyCode::KEYPAD_1; }
      if (key == KeyCode::K)            { key = KeyCode::KEYPAD_2; }
      if (key == KeyCode::L)            { key = KeyCode::KEYPAD_3; }
      if (key == KeyCode::U)            { key = KeyCode::KEYPAD_4; }
      if (key == KeyCode::I)            { key = KeyCode::KEYPAD_5; }
      if (key == KeyCode::O)            { key = KeyCode::KEYPAD_6; }
      if (key == KeyCode::KEY_7)        { key = KeyCode::KEYPAD_7; }
      if (key == KeyCode::KEY_8)        { key = KeyCode::KEYPAD_8; }
      if (key == KeyCode::KEY_9)        { key = KeyCode::KEYPAD_9; }
      if (key == KeyCode::KEY_6)        { key = KeyCode::KEYPAD_CLEAR; }
      if (key == KeyCode::SLASH)        { key = KeyCode::KEYPAD_PLUS; }
      if (key == KeyCode::SEMICOLON)    { key = KeyCode::KEYPAD_MINUS; }
      if (key == KeyCode::P)            { key = KeyCode::KEYPAD_MULTIPLY; }
      if (key == KeyCode::KEY_0)        { key = KeyCode::KEYPAD_SLASH; }
      if (key == KeyCode::MINUS)        { key = KeyCode::KEYPAD_EQUAL; }
      if (key == KeyCode::DOT)          { key = KeyCode::KEYPAD_DOT; }
      if (key == KeyCode::CURSOR_UP)    { key = KeyCode::PAGEUP; }
      if (key == KeyCode::CURSOR_DOWN)  { key = KeyCode::PAGEDOWN; }
      if (key == KeyCode::CURSOR_LEFT)  { key = KeyCode::HOME; }
      if (key == KeyCode::CURSOR_RIGHT) { key = KeyCode::END; }
      if (key == KeyCode::RETURN)       { key = KeyCode::ENTER; }
      if (key == KeyCode::DELETE)       { key = KeyCode::FORWARD_DELETE; }
    }
    if (keyboardType == KeyboardType::POWERBOOK ||
        keyboardType == KeyboardType::POWERBOOK_G4 ||
        keyboardType == KeyboardType::POWERBOOK_G4_TI) {
      if (key == KeyCode::ENTER) { key = KeyCode::ENTER_POWERBOOK; }
    }

    // ------------------------------------------------------------
    // set ModifierFlag::KEYPAD, ModifierFlag::CURSOR
    flags = ModifierFlag::stripCURSOR(flags);
    flags = ModifierFlag::stripKEYPAD(flags);

    // Note: KEYPAD_CLEAR, KEYPAD_COMMA have no ModifierFlag::KEYPAD bit.
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
      flags |= ModifierFlag::KEYPAD;
    }

    if (key == KeyCode::CURSOR_UP ||
        key == KeyCode::CURSOR_DOWN ||
        key == KeyCode::CURSOR_LEFT ||
        key == KeyCode::CURSOR_RIGHT) {
      flags |= ModifierFlag::CURSOR;
    }
  }

  KeyCode::KeyCode
  ModifierFlag::getKeyCode(unsigned int flag) {
    if (flag == ModifierFlag::CAPSLOCK) return KeyCode::CAPSLOCK;
    if (flag == ModifierFlag::SHIFT_L) return KeyCode::SHIFT_L;
    if (flag == ModifierFlag::SHIFT_R) return KeyCode::SHIFT_R;
    if (flag == ModifierFlag::CONTROL_L) return KeyCode::CONTROL_L;
    if (flag == ModifierFlag::CONTROL_R) return KeyCode::CONTROL_R;
    if (flag == ModifierFlag::OPTION_L) return KeyCode::OPTION_L;
    if (flag == ModifierFlag::OPTION_R) return KeyCode::OPTION_R;
    if (flag == ModifierFlag::COMMAND_L) return KeyCode::COMMAND_L;
    if (flag == ModifierFlag::COMMAND_R) return KeyCode::COMMAND_R;
    if (flag == ModifierFlag::FN) return KeyCode::FN;

    return KeyCode::VK_NONE;
  }

  ModifierFlag::ModifierFlag
  KeyCode::getModifierFlag(unsigned int key)
  {
    if (key == KeyCode::CAPSLOCK) return ModifierFlag::CAPSLOCK;
    if (key == KeyCode::SHIFT_L) return ModifierFlag::SHIFT_L;
    if (key == KeyCode::SHIFT_R) return ModifierFlag::SHIFT_R;
    if (key == KeyCode::CONTROL_L) return ModifierFlag::CONTROL_L;
    if (key == KeyCode::CONTROL_R) return ModifierFlag::CONTROL_R;
    if (key == KeyCode::OPTION_L) return ModifierFlag::OPTION_L;
    if (key == KeyCode::OPTION_R) return ModifierFlag::OPTION_R;
    if (key == KeyCode::COMMAND_L) return ModifierFlag::COMMAND_L;
    if (key == KeyCode::COMMAND_R) return ModifierFlag::COMMAND_R;
    if (key == KeyCode::FN) return ModifierFlag::FN;

    return ModifierFlag::NONE;
  }
}
