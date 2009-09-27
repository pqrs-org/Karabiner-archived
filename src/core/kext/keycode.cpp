#include "keycode.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace KeyCode {
    void
    normalizeKey(unsigned int &key, unsigned int &flags, unsigned int keyboardType)
    {
      if (keyboardType == KeyboardType::POWERBOOK ||
          keyboardType == KeyboardType::POWERBOOK_G4 ||
          keyboardType == KeyboardType::POWERBOOK_G4_TI) {
        if (key == KeyCode::ENTER_POWERBOOK) { key = KeyCode::ENTER; }
      }

      if (ModifierFlag::isOn(flags, ModifierFlag::FN)) {
        if (key == KeyCode::KEYPAD_0)        { key = KeyCode::M;            flags = ModifierFlag::stripFN(flags); }
        if (key == KeyCode::KEYPAD_1)        { key = KeyCode::J;            flags = ModifierFlag::stripFN(flags); }
        if (key == KeyCode::KEYPAD_2)        { key = KeyCode::K;            flags = ModifierFlag::stripFN(flags); }
        if (key == KeyCode::KEYPAD_3)        { key = KeyCode::L;            flags = ModifierFlag::stripFN(flags); }
        if (key == KeyCode::KEYPAD_4)        { key = KeyCode::U;            flags = ModifierFlag::stripFN(flags); }
        if (key == KeyCode::KEYPAD_5)        { key = KeyCode::I;            flags = ModifierFlag::stripFN(flags); }
        if (key == KeyCode::KEYPAD_6)        { key = KeyCode::O;            flags = ModifierFlag::stripFN(flags); }
        if (key == KeyCode::KEYPAD_7)        { key = KeyCode::KEY_7;        flags = ModifierFlag::stripFN(flags); }
        if (key == KeyCode::KEYPAD_8)        { key = KeyCode::KEY_8;        flags = ModifierFlag::stripFN(flags); }
        if (key == KeyCode::KEYPAD_9)        { key = KeyCode::KEY_9;        flags = ModifierFlag::stripFN(flags); }
        if (key == KeyCode::KEYPAD_CLEAR)    { key = KeyCode::KEY_6;        flags = ModifierFlag::stripFN(flags); }
        if (key == KeyCode::KEYPAD_PLUS)     { key = KeyCode::SLASH;        flags = ModifierFlag::stripFN(flags); }
        if (key == KeyCode::KEYPAD_MINUS)    { key = KeyCode::SEMICOLON;    flags = ModifierFlag::stripFN(flags); }
        if (key == KeyCode::KEYPAD_MULTIPLY) { key = KeyCode::P;            flags = ModifierFlag::stripFN(flags); }
        if (key == KeyCode::KEYPAD_SLASH)    { key = KeyCode::KEY_0;        flags = ModifierFlag::stripFN(flags); }
        if (key == KeyCode::KEYPAD_EQUAL)    { key = KeyCode::MINUS;        flags = ModifierFlag::stripFN(flags); }
        if (key == KeyCode::KEYPAD_DOT)      { key = KeyCode::DOT;          flags = ModifierFlag::stripFN(flags); }
        if (key == KeyCode::PAGEUP)          { key = KeyCode::CURSOR_UP;    flags = ModifierFlag::stripFN(flags) | ModifierFlag::CURSOR; }
        if (key == KeyCode::PAGEDOWN)        { key = KeyCode::CURSOR_DOWN;  flags = ModifierFlag::stripFN(flags) | ModifierFlag::CURSOR; }
        if (key == KeyCode::HOME)            { key = KeyCode::CURSOR_LEFT;  flags = ModifierFlag::stripFN(flags) | ModifierFlag::CURSOR; }
        if (key == KeyCode::END)             { key = KeyCode::CURSOR_RIGHT; flags = ModifierFlag::stripFN(flags) | ModifierFlag::CURSOR; }
        if (key == KeyCode::ENTER)           { key = KeyCode::RETURN;       flags = ModifierFlag::stripFN(flags); }
        if (key == KeyCode::FORWARD_DELETE)  { key = KeyCode::DELETE;       flags = ModifierFlag::stripFN(flags); }
      }
    }

    void
    reverseNormalizeKey(unsigned int &key, unsigned int &flags, unsigned int keyboardType)
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
        if (key == KeyCode::CURSOR_UP)    { key = KeyCode::PAGEUP;   flags = ModifierFlag::stripCURSOR(flags); }
        if (key == KeyCode::CURSOR_DOWN)  { key = KeyCode::PAGEDOWN; flags = ModifierFlag::stripCURSOR(flags); }
        if (key == KeyCode::CURSOR_LEFT)  { key = KeyCode::HOME;     flags = ModifierFlag::stripCURSOR(flags); }
        if (key == KeyCode::CURSOR_RIGHT) { key = KeyCode::END;      flags = ModifierFlag::stripCURSOR(flags); }
        if (key == KeyCode::RETURN)       { key = KeyCode::ENTER; }
        if (key == KeyCode::DELETE)       { key = KeyCode::FORWARD_DELETE; }
      }
      if (keyboardType == KeyboardType::POWERBOOK ||
          keyboardType == KeyboardType::POWERBOOK_G4 ||
          keyboardType == KeyboardType::POWERBOOK_G4_TI) {
        if (key == KeyCode::ENTER) { key = KeyCode::ENTER_POWERBOOK; }
      }
    }
  }
}
