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

  const ModifierFlag::Mask ModifierFlag::list[] = {
    ModifierFlag::CAPSLOCK,
    ModifierFlag::SHIFT_L,
    ModifierFlag::SHIFT_R,
    ModifierFlag::CONTROL_L,
    ModifierFlag::CONTROL_R,
    ModifierFlag::OPTION_L,
    ModifierFlag::OPTION_R,
    ModifierFlag::COMMAND_L,
    ModifierFlag::COMMAND_R,
    ModifierFlag::FN,
  };
  const int ModifierFlag::listsize = sizeof(ModifierFlag::list) / sizeof(ModifierFlag::list[0]);

  void
  KeyCode::normalizeKey(Flags& flags, const KeyboardType& keyboardType)
  {
    if (keyboardType == KeyboardType::POWERBOOK ||
        keyboardType == KeyboardType::POWERBOOK_G4 ||
        keyboardType == KeyboardType::POWERBOOK_G4_TI) {
      if (value_ == KeyCode::ENTER_POWERBOOK) { value_ = KeyCode::ENTER; }
    }

    if (flags.isOn(ModifierFlag::FN)) {
      // Note: KEYPAD_CLEAR has no ModifierFlag::KEYPAD bit.
      if (value_ == KeyCode::KEYPAD_0)        { value_ = KeyCode::M;            flags.stripFN().stripKEYPAD(); }
      if (value_ == KeyCode::KEYPAD_1)        { value_ = KeyCode::J;            flags.stripFN().stripKEYPAD(); }
      if (value_ == KeyCode::KEYPAD_2)        { value_ = KeyCode::K;            flags.stripFN().stripKEYPAD(); }
      if (value_ == KeyCode::KEYPAD_3)        { value_ = KeyCode::L;            flags.stripFN().stripKEYPAD(); }
      if (value_ == KeyCode::KEYPAD_4)        { value_ = KeyCode::U;            flags.stripFN().stripKEYPAD(); }
      if (value_ == KeyCode::KEYPAD_5)        { value_ = KeyCode::I;            flags.stripFN().stripKEYPAD(); }
      if (value_ == KeyCode::KEYPAD_6)        { value_ = KeyCode::O;            flags.stripFN().stripKEYPAD(); }
      if (value_ == KeyCode::KEYPAD_7)        { value_ = KeyCode::KEY_7;        flags.stripFN().stripKEYPAD(); }
      if (value_ == KeyCode::KEYPAD_8)        { value_ = KeyCode::KEY_8;        flags.stripFN().stripKEYPAD(); }
      if (value_ == KeyCode::KEYPAD_9)        { value_ = KeyCode::KEY_9;        flags.stripFN().stripKEYPAD(); }
      if (value_ == KeyCode::KEYPAD_CLEAR)    { value_ = KeyCode::KEY_6;        flags.stripFN(); }
      if (value_ == KeyCode::KEYPAD_PLUS)     { value_ = KeyCode::SLASH;        flags.stripFN().stripKEYPAD(); }
      if (value_ == KeyCode::KEYPAD_MINUS)    { value_ = KeyCode::SEMICOLON;    flags.stripFN().stripKEYPAD(); }
      if (value_ == KeyCode::KEYPAD_MULTIPLY) { value_ = KeyCode::P;            flags.stripFN().stripKEYPAD(); }
      if (value_ == KeyCode::KEYPAD_SLASH)    { value_ = KeyCode::KEY_0;        flags.stripFN().stripKEYPAD(); }
      if (value_ == KeyCode::KEYPAD_EQUAL)    { value_ = KeyCode::MINUS;        flags.stripFN().stripKEYPAD(); }
      if (value_ == KeyCode::KEYPAD_DOT)      { value_ = KeyCode::DOT;          flags.stripFN().stripKEYPAD(); }
      if (value_ == KeyCode::PAGEUP)          { value_ = KeyCode::CURSOR_UP;    flags.stripFN().add(ModifierFlag::CURSOR); }
      if (value_ == KeyCode::PAGEDOWN)        { value_ = KeyCode::CURSOR_DOWN;  flags.stripFN().add(ModifierFlag::CURSOR); }
      if (value_ == KeyCode::HOME)            { value_ = KeyCode::CURSOR_LEFT;  flags.stripFN().add(ModifierFlag::CURSOR); }
      if (value_ == KeyCode::END)             { value_ = KeyCode::CURSOR_RIGHT; flags.stripFN().add(ModifierFlag::CURSOR); }
      if (value_ == KeyCode::ENTER)           { value_ = KeyCode::RETURN;       flags.stripFN(); }
      if (value_ == KeyCode::FORWARD_DELETE)  { value_ = KeyCode::DELETE;       flags.stripFN(); }
    }
  }

  void
  KeyCode::reverseNormalizeKey(Flags& flags, const KeyboardType& keyboardType)
  {
    if (flags.isOn(ModifierFlag::FN)) {
      if (value_ == KeyCode::M)            { value_ = KeyCode::KEYPAD_0; }
      if (value_ == KeyCode::J)            { value_ = KeyCode::KEYPAD_1; }
      if (value_ == KeyCode::K)            { value_ = KeyCode::KEYPAD_2; }
      if (value_ == KeyCode::L)            { value_ = KeyCode::KEYPAD_3; }
      if (value_ == KeyCode::U)            { value_ = KeyCode::KEYPAD_4; }
      if (value_ == KeyCode::I)            { value_ = KeyCode::KEYPAD_5; }
      if (value_ == KeyCode::O)            { value_ = KeyCode::KEYPAD_6; }
      if (value_ == KeyCode::KEY_7)        { value_ = KeyCode::KEYPAD_7; }
      if (value_ == KeyCode::KEY_8)        { value_ = KeyCode::KEYPAD_8; }
      if (value_ == KeyCode::KEY_9)        { value_ = KeyCode::KEYPAD_9; }
      if (value_ == KeyCode::KEY_6)        { value_ = KeyCode::KEYPAD_CLEAR; }
      if (value_ == KeyCode::SLASH)        { value_ = KeyCode::KEYPAD_PLUS; }
      if (value_ == KeyCode::SEMICOLON)    { value_ = KeyCode::KEYPAD_MINUS; }
      if (value_ == KeyCode::P)            { value_ = KeyCode::KEYPAD_MULTIPLY; }
      if (value_ == KeyCode::KEY_0)        { value_ = KeyCode::KEYPAD_SLASH; }
      if (value_ == KeyCode::MINUS)        { value_ = KeyCode::KEYPAD_EQUAL; }
      if (value_ == KeyCode::DOT)          { value_ = KeyCode::KEYPAD_DOT; }
      if (value_ == KeyCode::CURSOR_UP)    { value_ = KeyCode::PAGEUP; }
      if (value_ == KeyCode::CURSOR_DOWN)  { value_ = KeyCode::PAGEDOWN; }
      if (value_ == KeyCode::CURSOR_LEFT)  { value_ = KeyCode::HOME; }
      if (value_ == KeyCode::CURSOR_RIGHT) { value_ = KeyCode::END; }
      if (value_ == KeyCode::RETURN)       { value_ = KeyCode::ENTER; }
      if (value_ == KeyCode::DELETE)       { value_ = KeyCode::FORWARD_DELETE; }
    }
    if (keyboardType == KeyboardType::POWERBOOK ||
        keyboardType == KeyboardType::POWERBOOK_G4 ||
        keyboardType == KeyboardType::POWERBOOK_G4_TI) {
      if (value_ == KeyCode::ENTER) { value_ = KeyCode::ENTER_POWERBOOK; }
    }

    // ------------------------------------------------------------
    // set ModifierFlag::KEYPAD, ModifierFlag::CURSOR
    flags.stripCURSOR().stripKEYPAD();

    // Note: KEYPAD_CLEAR, KEYPAD_COMMA have no ModifierFlag::KEYPAD bit.
    if (value_ == KeyCode::KEYPAD_0 || value_ == KeyCode::KEYPAD_1 || value_ == KeyCode::KEYPAD_2 ||
        value_ == KeyCode::KEYPAD_3 || value_ == KeyCode::KEYPAD_4 || value_ == KeyCode::KEYPAD_5 ||
        value_ == KeyCode::KEYPAD_6 || value_ == KeyCode::KEYPAD_7 || value_ == KeyCode::KEYPAD_8 ||
        value_ == KeyCode::KEYPAD_9 ||
        value_ == KeyCode::KEYPAD_DOT ||
        value_ == KeyCode::KEYPAD_MULTIPLY ||
        value_ == KeyCode::KEYPAD_PLUS ||
        value_ == KeyCode::KEYPAD_SLASH ||
        value_ == KeyCode::KEYPAD_MINUS ||
        value_ == KeyCode::KEYPAD_EQUAL) {
      flags.add(ModifierFlag::KEYPAD);
    }

    if (value_ == KeyCode::CURSOR_UP ||
        value_ == KeyCode::CURSOR_DOWN ||
        value_ == KeyCode::CURSOR_LEFT ||
        value_ == KeyCode::CURSOR_RIGHT) {
      flags.add(ModifierFlag::CURSOR);
    }
  }

  KeyCode
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

  ModifierFlag::Mask
  KeyCode::getModifierFlag(void)
  {
    if (value_ == KeyCode::CAPSLOCK) return ModifierFlag::CAPSLOCK;
    if (value_ == KeyCode::SHIFT_L) return ModifierFlag::SHIFT_L;
    if (value_ == KeyCode::SHIFT_R) return ModifierFlag::SHIFT_R;
    if (value_ == KeyCode::CONTROL_L) return ModifierFlag::CONTROL_L;
    if (value_ == KeyCode::CONTROL_R) return ModifierFlag::CONTROL_R;
    if (value_ == KeyCode::OPTION_L) return ModifierFlag::OPTION_L;
    if (value_ == KeyCode::OPTION_R) return ModifierFlag::OPTION_R;
    if (value_ == KeyCode::COMMAND_L) return ModifierFlag::COMMAND_L;
    if (value_ == KeyCode::COMMAND_R) return ModifierFlag::COMMAND_R;
    if (value_ == KeyCode::FN) return ModifierFlag::FN;

    return ModifierFlag::NONE;
  }
}
