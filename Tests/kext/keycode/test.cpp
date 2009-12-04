#include <gtest/gtest.h>
#include "keycode.hpp"

using namespace org_pqrs_KeyRemap4MacBook;

TEST(ModifierFlag, stripFN) {
  unsigned int flags = ModifierFlag::SHIFT_L | ModifierFlag::CONTROL_R | ModifierFlag::COMMAND_R;
  EXPECT_EQ(ModifierFlag::stripFN(flags), flags);
  EXPECT_EQ(ModifierFlag::stripFN(flags | ModifierFlag::FN), flags);
}

TEST(ModifierFlag, stripCURSOR) {
  unsigned int flags = ModifierFlag::SHIFT_L | ModifierFlag::CONTROL_R | ModifierFlag::COMMAND_R;
  EXPECT_EQ(ModifierFlag::stripCURSOR(flags), flags);
  EXPECT_EQ(ModifierFlag::stripCURSOR(flags | ModifierFlag::CURSOR), flags);
}

TEST(ModifierFlag, stripKEYPAD) {
  unsigned int flags = ModifierFlag::SHIFT_L | ModifierFlag::CONTROL_R | ModifierFlag::COMMAND_R;
  EXPECT_EQ(ModifierFlag::stripKEYPAD(flags), flags);
  EXPECT_EQ(ModifierFlag::stripKEYPAD(flags | ModifierFlag::KEYPAD), flags);
}

TEST(ModifierFlag, stripNONE) {
  unsigned int flags = ModifierFlag::SHIFT_L | ModifierFlag::CONTROL_R | ModifierFlag::COMMAND_R;
  EXPECT_EQ(ModifierFlag::stripNONE(flags), flags);
  EXPECT_EQ(ModifierFlag::stripNONE(flags | ModifierFlag::NONE), flags);
}

TEST(ModifierFlag, isOn) {
  unsigned int flags = ModifierFlag::SHIFT_L | ModifierFlag::CONTROL_R | ModifierFlag::COMMAND_R;
  EXPECT_TRUE(ModifierFlag::isOn(flags, ModifierFlag::SHIFT_L));
  EXPECT_FALSE(ModifierFlag::isOn(flags, ModifierFlag::SHIFT_R));

  flags = ModifierFlag::NONE;
  EXPECT_TRUE(ModifierFlag::isOn(flags, ModifierFlag::NONE));
}

namespace {
  unsigned int keypads[][2] = {
    { KeyCode::KEYPAD_0, KeyCode::M },
    { KeyCode::KEYPAD_1, KeyCode::J },
    { KeyCode::KEYPAD_2, KeyCode::K },
    { KeyCode::KEYPAD_3, KeyCode::L },
    { KeyCode::KEYPAD_4, KeyCode::U },
    { KeyCode::KEYPAD_5, KeyCode::I },
    { KeyCode::KEYPAD_6, KeyCode::O },
    { KeyCode::KEYPAD_7, KeyCode::KEY_7 },
    { KeyCode::KEYPAD_8, KeyCode::KEY_8 },
    { KeyCode::KEYPAD_9, KeyCode::KEY_9 },
    { KeyCode::KEYPAD_CLEAR, KeyCode::KEY_6 },
    { KeyCode::KEYPAD_PLUS, KeyCode::SLASH },
    { KeyCode::KEYPAD_MINUS, KeyCode::SEMICOLON },
    { KeyCode::KEYPAD_MULTIPLY, KeyCode::P },
    { KeyCode::KEYPAD_SLASH, KeyCode::KEY_0 },
    { KeyCode::KEYPAD_EQUAL, KeyCode::MINUS },
    { KeyCode::KEYPAD_DOT, KeyCode::DOT },
  };

  unsigned int cursors[][2] = {
    { KeyCode::PAGEUP, KeyCode::CURSOR_UP },
    { KeyCode::PAGEDOWN, KeyCode::CURSOR_DOWN },
    { KeyCode::HOME, KeyCode::CURSOR_LEFT },
    { KeyCode::END, KeyCode::CURSOR_RIGHT },
  };
}


TEST(KeyCode, normalizeKey) {
  unsigned int key = 0;
  unsigned int flags = 0;
  unsigned int keyboardType = 0;

  // ENTER_POWERBOOK -> ENTER
  key = KeyCode::ENTER_POWERBOOK; flags = ModifierFlag::SHIFT_L; keyboardType = KeyboardType::POWERBOOK;
  KeyCode::normalizeKey(key, flags, keyboardType);
  EXPECT_EQ(key, static_cast<unsigned int>(KeyCode::ENTER)); EXPECT_EQ(flags, static_cast<unsigned int>(ModifierFlag::SHIFT_L));

  // ENTER_POWERBOOK(+FN) -> ENTER(+FN) -> RETURN
  key = KeyCode::ENTER_POWERBOOK; flags = ModifierFlag::SHIFT_L | ModifierFlag::FN; keyboardType = KeyboardType::POWERBOOK;
  KeyCode::normalizeKey(key, flags, keyboardType);
  EXPECT_EQ(key, static_cast<unsigned int>(KeyCode::RETURN)); EXPECT_EQ(flags, static_cast<unsigned int>(ModifierFlag::SHIFT_L));

  // normal key
  key = KeyCode::A; flags = ModifierFlag::SHIFT_L; keyboardType = KeyboardType::MACBOOK;
  KeyCode::normalizeKey(key, flags, keyboardType);
  EXPECT_EQ(key, static_cast<unsigned int>(KeyCode::A)); EXPECT_EQ(flags, static_cast<unsigned int>(ModifierFlag::SHIFT_L));

  // KEYPAD
  for (size_t i = 0; i < sizeof(keypads) / sizeof(keypads[0]); ++i) {
    key = keypads[i][0]; flags = ModifierFlag::SHIFT_L; keyboardType = KeyboardType::MACBOOK;
    if (key != KeyCode::KEYPAD_CLEAR && KeyCode::KEYPAD_COMMA) flags |= ModifierFlag::KEYPAD;
    unsigned int flags_orig = flags;
    KeyCode::normalizeKey(key, flags, keyboardType);
    EXPECT_EQ(key, keypads[i][0]); EXPECT_EQ(flags, flags_orig);
  }

  // PAGEUP
  for (size_t i = 0; i < sizeof(cursors) / sizeof(cursors[0]); ++i) {
    key = cursors[i][0]; flags = ModifierFlag::SHIFT_L; keyboardType = KeyboardType::MACBOOK;
    KeyCode::normalizeKey(key, flags, keyboardType);
    EXPECT_EQ(key, cursors[i][0]); EXPECT_EQ(flags, static_cast<unsigned int>(ModifierFlag::SHIFT_L));
  }

  // ENTER
  key = KeyCode::ENTER; flags = ModifierFlag::SHIFT_L; keyboardType = KeyboardType::MACBOOK;
  KeyCode::normalizeKey(key, flags, keyboardType);
  EXPECT_EQ(key, static_cast<unsigned int>(KeyCode::ENTER)); EXPECT_EQ(flags, static_cast<unsigned int>(ModifierFlag::SHIFT_L));

  // FORWARD_DELETE
  key = KeyCode::FORWARD_DELETE; flags = ModifierFlag::SHIFT_L; keyboardType = KeyboardType::MACBOOK;
  KeyCode::normalizeKey(key, flags, keyboardType);
  EXPECT_EQ(key, static_cast<unsigned int>(KeyCode::FORWARD_DELETE)); EXPECT_EQ(flags, static_cast<unsigned int>(ModifierFlag::SHIFT_L));

  // normal key(+FN)
  key = KeyCode::A; flags = ModifierFlag::SHIFT_L | ModifierFlag::FN; keyboardType = KeyboardType::MACBOOK;
  KeyCode::normalizeKey(key, flags, keyboardType);
  EXPECT_EQ(key, static_cast<unsigned int>(KeyCode::A)); EXPECT_EQ(flags, static_cast<unsigned int>(ModifierFlag::SHIFT_L | ModifierFlag::FN));

  // KEYPAD(+FN)
  for (size_t i = 0; i < sizeof(keypads) / sizeof(keypads[0]); ++i) {
    key = keypads[i][0]; flags = ModifierFlag::SHIFT_L | ModifierFlag::FN; keyboardType = KeyboardType::MACBOOK;
    if (key != KeyCode::KEYPAD_CLEAR && KeyCode::KEYPAD_COMMA) flags |= ModifierFlag::KEYPAD;
    if (key == KeyCode::KEYPAD_COMMA) continue;
    KeyCode::normalizeKey(key, flags, keyboardType);
    EXPECT_EQ(key, static_cast<unsigned int>(keypads[i][1])); EXPECT_EQ(flags, static_cast<unsigned int>(ModifierFlag::SHIFT_L));
  }

  // PAGEUP(+FN)
  for (size_t i = 0; i < sizeof(cursors) / sizeof(cursors[0]); ++i) {
    key = cursors[i][0]; flags = ModifierFlag::SHIFT_L | ModifierFlag::FN; keyboardType = KeyboardType::MACBOOK;
    KeyCode::normalizeKey(key, flags, keyboardType);
    EXPECT_EQ(key, cursors[i][1]); EXPECT_EQ(flags, static_cast<unsigned int>(ModifierFlag::SHIFT_L | ModifierFlag::CURSOR));
  }

  // ENTER(+FN)
  key = KeyCode::ENTER; flags = ModifierFlag::SHIFT_L | ModifierFlag::FN; keyboardType = KeyboardType::MACBOOK;
  KeyCode::normalizeKey(key, flags, keyboardType);
  EXPECT_EQ(key, static_cast<unsigned int>(KeyCode::RETURN)); EXPECT_EQ(flags, static_cast<unsigned int>(ModifierFlag::SHIFT_L));

  // FORWARD_DELETE(+FN)
  key = KeyCode::FORWARD_DELETE; flags = ModifierFlag::SHIFT_L | ModifierFlag::FN; keyboardType = KeyboardType::MACBOOK;
  KeyCode::normalizeKey(key, flags, keyboardType);
  EXPECT_EQ(key, static_cast<unsigned int>(KeyCode::DELETE)); EXPECT_EQ(flags, static_cast<unsigned int>(ModifierFlag::SHIFT_L));
}

TEST(KeyCode, reverseNormalizeKey) {
  unsigned int key = 0;
  unsigned int flags = 0;
  unsigned int keyboardType = 0;

  // ENTER_POWERBOOK -> ENTER
  key = KeyCode::ENTER_POWERBOOK; flags = ModifierFlag::SHIFT_L; keyboardType = KeyboardType::POWERBOOK;
  KeyCode::normalizeKey(key, flags, keyboardType);
  KeyCode::reverseNormalizeKey(key, flags, keyboardType);
  EXPECT_EQ(key, static_cast<unsigned int>(KeyCode::ENTER_POWERBOOK)); EXPECT_EQ(flags, static_cast<unsigned int>(ModifierFlag::SHIFT_L));

  // ENTER_POWERBOOK(+FN) -> ENTER(+FN) -> RETURN
  key = KeyCode::ENTER_POWERBOOK; flags = ModifierFlag::SHIFT_L | ModifierFlag::FN; keyboardType = KeyboardType::POWERBOOK;
  KeyCode::normalizeKey(key, flags, keyboardType);
  flags |= ModifierFlag::FN;
  KeyCode::reverseNormalizeKey(key, flags, keyboardType);
  EXPECT_EQ(key, static_cast<unsigned int>(KeyCode::ENTER_POWERBOOK)); EXPECT_EQ(flags, static_cast<unsigned int>(ModifierFlag::SHIFT_L | ModifierFlag::FN));

  // normal key
  key = KeyCode::A; flags = ModifierFlag::SHIFT_L; keyboardType = KeyboardType::MACBOOK;
  KeyCode::normalizeKey(key, flags, keyboardType);
  KeyCode::reverseNormalizeKey(key, flags, keyboardType);
  EXPECT_EQ(key, static_cast<unsigned int>(KeyCode::A)); EXPECT_EQ(flags, static_cast<unsigned int>(ModifierFlag::SHIFT_L));

  // KEYPAD
  for (size_t i = 0; i < sizeof(keypads) / sizeof(keypads[0]); ++i) {
    key = keypads[i][0]; flags = ModifierFlag::SHIFT_L; keyboardType = KeyboardType::MACBOOK;
    if (key != KeyCode::KEYPAD_CLEAR && KeyCode::KEYPAD_COMMA) flags |= ModifierFlag::KEYPAD;
    unsigned int flags_orig = flags;
    KeyCode::normalizeKey(key, flags, keyboardType);
    KeyCode::reverseNormalizeKey(key, flags, keyboardType);
    EXPECT_EQ(key, keypads[i][0]); EXPECT_EQ(flags, flags_orig);
  }

  // PAGEUP
  for (size_t i = 0; i < sizeof(cursors) / sizeof(cursors[0]); ++i) {
    key = cursors[i][0]; flags = ModifierFlag::SHIFT_L; keyboardType = KeyboardType::MACBOOK;
    KeyCode::normalizeKey(key, flags, keyboardType);
    KeyCode::reverseNormalizeKey(key, flags, keyboardType);
    EXPECT_EQ(key, cursors[i][0]); EXPECT_EQ(flags, static_cast<unsigned int>(ModifierFlag::SHIFT_L));
  }

  // ENTER
  key = KeyCode::ENTER; flags = ModifierFlag::SHIFT_L; keyboardType = KeyboardType::MACBOOK;
  KeyCode::normalizeKey(key, flags, keyboardType);
  KeyCode::reverseNormalizeKey(key, flags, keyboardType);
  EXPECT_EQ(key, static_cast<unsigned int>(KeyCode::ENTER)); EXPECT_EQ(flags, static_cast<unsigned int>(ModifierFlag::SHIFT_L));

  // FORWARD_DELETE
  key = KeyCode::FORWARD_DELETE; flags = ModifierFlag::SHIFT_L; keyboardType = KeyboardType::MACBOOK;
  KeyCode::normalizeKey(key, flags, keyboardType);
  KeyCode::reverseNormalizeKey(key, flags, keyboardType);
  EXPECT_EQ(key, static_cast<unsigned int>(KeyCode::FORWARD_DELETE)); EXPECT_EQ(flags, static_cast<unsigned int>(ModifierFlag::SHIFT_L));

  // CURSOR
  for (size_t i = 0; i < sizeof(cursors) / sizeof(cursors[0]); ++i) {
    key = cursors[i][1]; flags = ModifierFlag::CURSOR; keyboardType = KeyboardType::MACBOOK;
    KeyCode::normalizeKey(key, flags, keyboardType);
    KeyCode::reverseNormalizeKey(key, flags, keyboardType);
    EXPECT_EQ(key, static_cast<unsigned int>(cursors[i][1])); EXPECT_EQ(flags, static_cast<unsigned int>(ModifierFlag::CURSOR));
  }

  // normal key(+FN)
  key = KeyCode::A; flags = ModifierFlag::SHIFT_L | ModifierFlag::FN; keyboardType = KeyboardType::MACBOOK;
  KeyCode::normalizeKey(key, flags, keyboardType);
  flags |= ModifierFlag::FN;
  KeyCode::reverseNormalizeKey(key, flags, keyboardType);
  EXPECT_EQ(key, static_cast<unsigned int>(KeyCode::A)); EXPECT_EQ(flags, static_cast<unsigned int>(ModifierFlag::SHIFT_L | ModifierFlag::FN));

  // KEYPAD(+FN)
  for (size_t i = 0; i < sizeof(keypads) / sizeof(keypads[0]); ++i) {
    key = keypads[i][0]; flags = ModifierFlag::SHIFT_L | ModifierFlag::FN; keyboardType = KeyboardType::MACBOOK;
    if (key != KeyCode::KEYPAD_CLEAR && KeyCode::KEYPAD_COMMA) flags |= ModifierFlag::KEYPAD;
    if (key == KeyCode::KEYPAD_COMMA) continue;
    unsigned int flags_orig = flags;
    KeyCode::normalizeKey(key, flags, keyboardType);
    flags |= ModifierFlag::FN;
    KeyCode::reverseNormalizeKey(key, flags, keyboardType);
    EXPECT_EQ(key, static_cast<unsigned int>(keypads[i][0])); EXPECT_EQ(flags, flags_orig);
  }

  // PAGEUP(+FN)
  for (size_t i = 0; i < sizeof(cursors) / sizeof(cursors[0]); ++i) {
    key = cursors[i][0]; flags = ModifierFlag::SHIFT_L | ModifierFlag::FN; keyboardType = KeyboardType::MACBOOK;
    KeyCode::normalizeKey(key, flags, keyboardType);
    flags |= ModifierFlag::FN;
    KeyCode::reverseNormalizeKey(key, flags, keyboardType);
    EXPECT_EQ(key, cursors[i][0]); EXPECT_EQ(flags, static_cast<unsigned int>(ModifierFlag::SHIFT_L | ModifierFlag::FN));
  }

  // ENTER(+FN)
  key = KeyCode::ENTER; flags = ModifierFlag::SHIFT_L | ModifierFlag::FN; keyboardType = KeyboardType::MACBOOK;
  KeyCode::normalizeKey(key, flags, keyboardType);
  flags |= ModifierFlag::FN;
  KeyCode::reverseNormalizeKey(key, flags, keyboardType);
  EXPECT_EQ(key, static_cast<unsigned int>(KeyCode::ENTER)); EXPECT_EQ(flags, static_cast<unsigned int>(ModifierFlag::SHIFT_L | ModifierFlag::FN));

  // FORWARD_DELETE(+FN)
  key = KeyCode::FORWARD_DELETE; flags = ModifierFlag::SHIFT_L | ModifierFlag::FN; keyboardType = KeyboardType::MACBOOK;
  KeyCode::normalizeKey(key, flags, keyboardType);
  flags |= ModifierFlag::FN;
  KeyCode::reverseNormalizeKey(key, flags, keyboardType);
  EXPECT_EQ(key, static_cast<unsigned int>(KeyCode::FORWARD_DELETE)); EXPECT_EQ(flags, static_cast<unsigned int>(ModifierFlag::SHIFT_L | ModifierFlag::FN));
}

TEST(ModifierFlag, getKeyCode) {
  EXPECT_EQ(KeyCode::CAPSLOCK, ModifierFlag::getKeyCode(ModifierFlag::CAPSLOCK));
  EXPECT_EQ(KeyCode::SHIFT_L, ModifierFlag::getKeyCode(ModifierFlag::SHIFT_L));
  EXPECT_EQ(KeyCode::SHIFT_R, ModifierFlag::getKeyCode(ModifierFlag::SHIFT_R));
  EXPECT_EQ(KeyCode::CONTROL_L, ModifierFlag::getKeyCode(ModifierFlag::CONTROL_L));
  EXPECT_EQ(KeyCode::CONTROL_R, ModifierFlag::getKeyCode(ModifierFlag::CONTROL_R));
  EXPECT_EQ(KeyCode::OPTION_L, ModifierFlag::getKeyCode(ModifierFlag::OPTION_L));
  EXPECT_EQ(KeyCode::OPTION_R, ModifierFlag::getKeyCode(ModifierFlag::OPTION_R));
  EXPECT_EQ(KeyCode::COMMAND_L, ModifierFlag::getKeyCode(ModifierFlag::COMMAND_L));
  EXPECT_EQ(KeyCode::COMMAND_R, ModifierFlag::getKeyCode(ModifierFlag::COMMAND_R));
  EXPECT_EQ(KeyCode::FN, ModifierFlag::getKeyCode(ModifierFlag::FN));

  EXPECT_EQ(KeyCode::VK_NONE, ModifierFlag::getKeyCode(ModifierFlag::CURSOR));
  EXPECT_EQ(KeyCode::VK_NONE, ModifierFlag::getKeyCode(ModifierFlag::KEYPAD));

  EXPECT_EQ(KeyCode::VK_NONE, ModifierFlag::getKeyCode(ModifierFlag::CAPSLOCK | ModifierFlag::SHIFT_L));
}

TEST(KeyCode, getModifierFlag) {
  EXPECT_EQ(ModifierFlag::CAPSLOCK, KeyCode::getModifierFlag(KeyCode::CAPSLOCK));
  EXPECT_EQ(ModifierFlag::SHIFT_L, KeyCode::getModifierFlag(KeyCode::SHIFT_L));
  EXPECT_EQ(ModifierFlag::SHIFT_R, KeyCode::getModifierFlag(KeyCode::SHIFT_R));
  EXPECT_EQ(ModifierFlag::CONTROL_L, KeyCode::getModifierFlag(KeyCode::CONTROL_L));
  EXPECT_EQ(ModifierFlag::CONTROL_R, KeyCode::getModifierFlag(KeyCode::CONTROL_R));
  EXPECT_EQ(ModifierFlag::OPTION_L, KeyCode::getModifierFlag(KeyCode::OPTION_L));
  EXPECT_EQ(ModifierFlag::OPTION_R, KeyCode::getModifierFlag(KeyCode::OPTION_R));
  EXPECT_EQ(ModifierFlag::COMMAND_L, KeyCode::getModifierFlag(KeyCode::COMMAND_L));
  EXPECT_EQ(ModifierFlag::COMMAND_R, KeyCode::getModifierFlag(KeyCode::COMMAND_R));
  EXPECT_EQ(ModifierFlag::FN, KeyCode::getModifierFlag(KeyCode::FN));

  EXPECT_EQ(ModifierFlag::NONE, KeyCode::getModifierFlag(KeyCode::A));
  EXPECT_EQ(ModifierFlag::NONE, KeyCode::getModifierFlag(KeyCode::VK_NONE));
}

TEST(KeyCode, isModifier) {
  EXPECT_EQ(true, KeyCode::isModifier(KeyCode::CAPSLOCK));
  EXPECT_EQ(true, KeyCode::isModifier(KeyCode::SHIFT_L));
  EXPECT_EQ(false, KeyCode::isModifier(KeyCode::A));
  EXPECT_EQ(false, KeyCode::isModifier(KeyCode::VK_NONE));
}
