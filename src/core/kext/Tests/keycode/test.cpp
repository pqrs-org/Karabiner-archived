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
    key = keypads[i][0]; flags = ModifierFlag::SHIFT_L | ModifierFlag::KEYPAD; keyboardType = KeyboardType::MACBOOK;
    KeyCode::normalizeKey(key, flags, keyboardType);
    EXPECT_EQ(key, keypads[i][0]); EXPECT_EQ(flags, static_cast<unsigned int>(ModifierFlag::SHIFT_L | ModifierFlag::KEYPAD));
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
    key = keypads[i][0]; flags = ModifierFlag::SHIFT_L | ModifierFlag::FN | ModifierFlag::KEYPAD; keyboardType = KeyboardType::MACBOOK;
    KeyCode::normalizeKey(key, flags, keyboardType);
    EXPECT_EQ(key, static_cast<unsigned int>(keypads[i][1])); EXPECT_EQ(flags, static_cast<unsigned int>(ModifierFlag::SHIFT_L | ModifierFlag::KEYPAD));
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
    key = keypads[i][0]; flags = ModifierFlag::SHIFT_L | ModifierFlag::KEYPAD; keyboardType = KeyboardType::MACBOOK;
    KeyCode::normalizeKey(key, flags, keyboardType);
    KeyCode::reverseNormalizeKey(key, flags, keyboardType);
    EXPECT_EQ(key, keypads[i][0]); EXPECT_EQ(flags, static_cast<unsigned int>(ModifierFlag::SHIFT_L | ModifierFlag::KEYPAD));
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
    key = keypads[i][0]; flags = ModifierFlag::SHIFT_L | ModifierFlag::FN | ModifierFlag::KEYPAD; keyboardType = KeyboardType::MACBOOK;
    KeyCode::normalizeKey(key, flags, keyboardType);
    flags |= ModifierFlag::FN;
    KeyCode::reverseNormalizeKey(key, flags, keyboardType);
    EXPECT_EQ(key, static_cast<unsigned int>(keypads[i][0])); EXPECT_EQ(flags, static_cast<unsigned int>(ModifierFlag::SHIFT_L | ModifierFlag::FN | ModifierFlag::KEYPAD));
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
