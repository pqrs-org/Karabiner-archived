#include <gtest/gtest.h>
#include "keycode.hpp"

using namespace org_pqrs_KeyRemap4MacBook;

TEST(KeyboardType, isInternalKeyboard) {
  {
    KeyboardType v(KeyboardType::MACBOOK);
    EXPECT_EQ(true, v.isInternalKeyboard());
  }
  {
    KeyboardType v(KeyboardType::JIS_APPLE_USB_KEYBOARD);
    EXPECT_EQ(false, v.isInternalKeyboard());
  }
  {
    KeyboardType v;
    EXPECT_EQ(false, v.isInternalKeyboard());
  }
}

TEST(Flags, add) {
  unsigned int mask = ModifierFlag::SHIFT_L | ModifierFlag::CONTROL_R | ModifierFlag::COMMAND_R;
  Flags flags = mask;
  EXPECT_EQ(mask, flags.stripFN().get());

  flags.add(ModifierFlag::OPTION_L);
  EXPECT_EQ(mask | ModifierFlag::OPTION_L, flags.get());

  flags.add(ModifierFlag::NONE);
  EXPECT_EQ(mask | ModifierFlag::OPTION_L | ModifierFlag::NONE, flags.get());
}

TEST(Flags, stripFN) {
  unsigned int mask = ModifierFlag::SHIFT_L | ModifierFlag::CONTROL_R | ModifierFlag::COMMAND_R;
  Flags flags = mask;
  EXPECT_EQ(mask, flags.stripFN().get());

  flags.add(ModifierFlag::FN);
  EXPECT_EQ(mask, flags.stripFN().get());
}

TEST(Flags, stripCURSOR) {
  unsigned int mask = ModifierFlag::SHIFT_L | ModifierFlag::CONTROL_R | ModifierFlag::COMMAND_R;
  Flags flags = mask;
  EXPECT_EQ(mask, flags.stripCURSOR().get());

  flags.add(ModifierFlag::CURSOR);
  EXPECT_EQ(mask, flags.stripCURSOR().get());
}

TEST(Flags, stripKEYPAD) {
  unsigned int mask = ModifierFlag::SHIFT_L | ModifierFlag::CONTROL_R | ModifierFlag::COMMAND_R;
  Flags flags = mask;
  EXPECT_EQ(mask, flags.stripKEYPAD().get());

  flags.add(ModifierFlag::KEYPAD);
  EXPECT_EQ(mask, flags.stripKEYPAD().get());
}

TEST(Flags, stripNONE) {
  unsigned int mask = ModifierFlag::SHIFT_L | ModifierFlag::CONTROL_R | ModifierFlag::COMMAND_R;
  Flags flags = mask;
  EXPECT_EQ(mask, flags.stripNONE().get());

  flags.add(ModifierFlag::NONE);
  EXPECT_EQ(mask, flags.stripNONE().get());
}

TEST(Flags, isOn) {
  unsigned int mask = ModifierFlag::SHIFT_L | ModifierFlag::CONTROL_R | ModifierFlag::COMMAND_R;
  Flags flags = mask;

  EXPECT_TRUE(flags.isOn(ModifierFlag::SHIFT_L));
  EXPECT_FALSE(flags.isOn(ModifierFlag::SHIFT_R));

  flags = ModifierFlag::NONE;
  EXPECT_TRUE(flags.isOn(ModifierFlag::NONE));
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
  KeyCode key;
  Flags flags;
  KeyboardType keyboardType;

#define CHECK_NORMALIZEKEY(KEYCODE, MODIFIERFLAG) {                     \
    EXPECT_EQ(static_cast<unsigned int>(KEYCODE), key.get());           \
    EXPECT_EQ(static_cast<unsigned int>(MODIFIERFLAG), flags.get());    \
  }

  // ENTER_POWERBOOK -> ENTER
  key = KeyCode::ENTER_POWERBOOK; flags = ModifierFlag::SHIFT_L; keyboardType = KeyboardType::POWERBOOK;
  key.normalizeKey(flags, keyboardType);
  CHECK_NORMALIZEKEY(KeyCode::ENTER, ModifierFlag::SHIFT_L);

  // ENTER_POWERBOOK(+FN) -> ENTER(+FN) -> RETURN
  key = KeyCode::ENTER_POWERBOOK; flags = ModifierFlag::SHIFT_L | ModifierFlag::FN; keyboardType = KeyboardType::POWERBOOK;
  key.normalizeKey(flags, keyboardType);
  CHECK_NORMALIZEKEY(KeyCode::RETURN, ModifierFlag::SHIFT_L);

  // normal key
  key = KeyCode::A; flags = ModifierFlag::SHIFT_L; keyboardType = KeyboardType::MACBOOK;
  key.normalizeKey(flags, keyboardType);
  CHECK_NORMALIZEKEY(KeyCode::A, ModifierFlag::SHIFT_L);

  // KEYPAD
  for (size_t i = 0; i < sizeof(keypads) / sizeof(keypads[0]); ++i) {
    key = keypads[i][0]; flags = ModifierFlag::SHIFT_L; keyboardType = KeyboardType::MACBOOK;
    if (key != KeyCode::KEYPAD_CLEAR && KeyCode::KEYPAD_COMMA) flags.add(ModifierFlag::KEYPAD);
    unsigned int flags_orig = flags.get();
    key.normalizeKey(flags, keyboardType);
    CHECK_NORMALIZEKEY(keypads[i][0], flags_orig);
  }

  // PAGEUP
  for (size_t i = 0; i < sizeof(cursors) / sizeof(cursors[0]); ++i) {
    key = cursors[i][0]; flags = ModifierFlag::SHIFT_L; keyboardType = KeyboardType::MACBOOK;
    key.normalizeKey(flags, keyboardType);
    CHECK_NORMALIZEKEY(cursors[i][0], ModifierFlag::SHIFT_L);
  }

  // ENTER
  key = KeyCode::ENTER; flags = ModifierFlag::SHIFT_L; keyboardType = KeyboardType::MACBOOK;
  key.normalizeKey(flags, keyboardType);
  CHECK_NORMALIZEKEY(KeyCode::ENTER, ModifierFlag::SHIFT_L);

  // FORWARD_DELETE
  key = KeyCode::FORWARD_DELETE; flags = ModifierFlag::SHIFT_L; keyboardType = KeyboardType::MACBOOK;
  key.normalizeKey(flags, keyboardType);
  CHECK_NORMALIZEKEY(KeyCode::FORWARD_DELETE, ModifierFlag::SHIFT_L);

  // normal key(+FN)
  key = KeyCode::A; flags = ModifierFlag::SHIFT_L | ModifierFlag::FN; keyboardType = KeyboardType::MACBOOK;
  key.normalizeKey(flags, keyboardType);
  CHECK_NORMALIZEKEY(KeyCode::A, ModifierFlag::SHIFT_L | ModifierFlag::FN);

  // KEYPAD(+FN)
  for (size_t i = 0; i < sizeof(keypads) / sizeof(keypads[0]); ++i) {
    key = keypads[i][0]; flags = ModifierFlag::SHIFT_L | ModifierFlag::FN; keyboardType = KeyboardType::MACBOOK;
    if (key != KeyCode::KEYPAD_CLEAR && KeyCode::KEYPAD_COMMA) flags.add(ModifierFlag::KEYPAD);
    if (key == KeyCode::KEYPAD_COMMA) continue;
    key.normalizeKey(flags, keyboardType);
    CHECK_NORMALIZEKEY(keypads[i][1], ModifierFlag::SHIFT_L);
  }

  // PAGEUP(+FN)
  for (size_t i = 0; i < sizeof(cursors) / sizeof(cursors[0]); ++i) {
    key = cursors[i][0]; flags = ModifierFlag::SHIFT_L | ModifierFlag::FN; keyboardType = KeyboardType::MACBOOK;
    key.normalizeKey(flags, keyboardType);
    CHECK_NORMALIZEKEY(cursors[i][1], ModifierFlag::SHIFT_L | ModifierFlag::CURSOR);
  }

  // ENTER(+FN)
  key = KeyCode::ENTER; flags = ModifierFlag::SHIFT_L | ModifierFlag::FN; keyboardType = KeyboardType::MACBOOK;
  key.normalizeKey(flags, keyboardType);
  CHECK_NORMALIZEKEY(KeyCode::RETURN, ModifierFlag::SHIFT_L);

  // FORWARD_DELETE(+FN)
  key = KeyCode::FORWARD_DELETE; flags = ModifierFlag::SHIFT_L | ModifierFlag::FN; keyboardType = KeyboardType::MACBOOK;
  key.normalizeKey(flags, keyboardType);
  CHECK_NORMALIZEKEY(KeyCode::DELETE, ModifierFlag::SHIFT_L);
}

TEST(KeyCode, reverseNormalizeKey) {
  KeyCode key;
  Flags flags;
  KeyboardType keyboardType;

#define CHECK_REVERSENORMALIZEKEY(KEYCODE, MODIFIERFLAG) {              \
    EXPECT_EQ(static_cast<unsigned int>(KEYCODE), key.get());           \
    EXPECT_EQ(static_cast<unsigned int>(MODIFIERFLAG), flags.get());    \
  }

  // ENTER_POWERBOOK -> ENTER
  key = KeyCode::ENTER_POWERBOOK; flags = ModifierFlag::SHIFT_L; keyboardType = KeyboardType::POWERBOOK;
  key.normalizeKey(flags, keyboardType);
  key.reverseNormalizeKey(flags, keyboardType);
  CHECK_REVERSENORMALIZEKEY(KeyCode::ENTER_POWERBOOK, ModifierFlag::SHIFT_L);

  // ENTER_POWERBOOK(+FN) -> ENTER(+FN) -> RETURN
  key = KeyCode::ENTER_POWERBOOK; flags = ModifierFlag::SHIFT_L | ModifierFlag::FN; keyboardType = KeyboardType::POWERBOOK;
  key.normalizeKey(flags, keyboardType);
  flags.add(ModifierFlag::FN);
  key.reverseNormalizeKey(flags, keyboardType);
  CHECK_REVERSENORMALIZEKEY(KeyCode::ENTER_POWERBOOK, ModifierFlag::SHIFT_L | ModifierFlag::FN);

  // normal key
  key = KeyCode::A; flags = ModifierFlag::SHIFT_L; keyboardType = KeyboardType::MACBOOK;
  key.normalizeKey(flags, keyboardType);
  key.reverseNormalizeKey(flags, keyboardType);
  CHECK_REVERSENORMALIZEKEY(KeyCode::A, ModifierFlag::SHIFT_L);

  // KEYPAD
  for (size_t i = 0; i < sizeof(keypads) / sizeof(keypads[0]); ++i) {
    key = keypads[i][0]; flags = ModifierFlag::SHIFT_L; keyboardType = KeyboardType::MACBOOK;
    if (key != KeyCode::KEYPAD_CLEAR && KeyCode::KEYPAD_COMMA) flags.add(ModifierFlag::KEYPAD);
    unsigned int flags_orig = flags.get();
    key.normalizeKey(flags, keyboardType);
    key.reverseNormalizeKey(flags, keyboardType);
    CHECK_REVERSENORMALIZEKEY(keypads[i][0], flags_orig);
  }

  // PAGEUP
  for (size_t i = 0; i < sizeof(cursors) / sizeof(cursors[0]); ++i) {
    key = cursors[i][0]; flags = ModifierFlag::SHIFT_L; keyboardType = KeyboardType::MACBOOK;
    key.normalizeKey(flags, keyboardType);
    key.reverseNormalizeKey(flags, keyboardType);
    CHECK_REVERSENORMALIZEKEY(cursors[i][0], ModifierFlag::SHIFT_L);
  }

  // ENTER
  key = KeyCode::ENTER; flags = ModifierFlag::SHIFT_L; keyboardType = KeyboardType::MACBOOK;
  key.normalizeKey(flags, keyboardType);
  key.reverseNormalizeKey(flags, keyboardType);
  CHECK_REVERSENORMALIZEKEY(KeyCode::ENTER, ModifierFlag::SHIFT_L);

  // FORWARD_DELETE
  key = KeyCode::FORWARD_DELETE; flags = ModifierFlag::SHIFT_L; keyboardType = KeyboardType::MACBOOK;
  key.normalizeKey(flags, keyboardType);
  key.reverseNormalizeKey(flags, keyboardType);
  CHECK_REVERSENORMALIZEKEY(KeyCode::FORWARD_DELETE, ModifierFlag::SHIFT_L);

  // CURSOR
  for (size_t i = 0; i < sizeof(cursors) / sizeof(cursors[0]); ++i) {
    key = cursors[i][1]; flags = ModifierFlag::CURSOR; keyboardType = KeyboardType::MACBOOK;
    key.normalizeKey(flags, keyboardType);
    key.reverseNormalizeKey(flags, keyboardType);
    CHECK_REVERSENORMALIZEKEY(cursors[i][1], ModifierFlag::CURSOR);
  }

  // normal key(+FN)
  key = KeyCode::A; flags = ModifierFlag::SHIFT_L | ModifierFlag::FN; keyboardType = KeyboardType::MACBOOK;
  key.normalizeKey(flags, keyboardType);
  flags.add(ModifierFlag::FN);
  key.reverseNormalizeKey(flags, keyboardType);
  CHECK_REVERSENORMALIZEKEY(KeyCode::A, ModifierFlag::SHIFT_L | ModifierFlag::FN);

  // KEYPAD(+FN)
  for (size_t i = 0; i < sizeof(keypads) / sizeof(keypads[0]); ++i) {
    key = keypads[i][0]; flags = ModifierFlag::SHIFT_L | ModifierFlag::FN; keyboardType = KeyboardType::MACBOOK;
    if (key != KeyCode::KEYPAD_CLEAR && KeyCode::KEYPAD_COMMA) flags.add(ModifierFlag::KEYPAD);
    if (key == KeyCode::KEYPAD_COMMA) continue;
    unsigned int flags_orig = flags.get();
    key.normalizeKey(flags, keyboardType);
    flags.add(ModifierFlag::FN);
    key.reverseNormalizeKey(flags, keyboardType);
    CHECK_REVERSENORMALIZEKEY(keypads[i][0], flags_orig);
  }

  // PAGEUP(+FN)
  for (size_t i = 0; i < sizeof(cursors) / sizeof(cursors[0]); ++i) {
    key = cursors[i][0]; flags = ModifierFlag::SHIFT_L | ModifierFlag::FN; keyboardType = KeyboardType::MACBOOK;
    key.normalizeKey(flags, keyboardType);
    flags.add(ModifierFlag::FN);
    key.reverseNormalizeKey(flags, keyboardType);
    CHECK_REVERSENORMALIZEKEY(cursors[i][0], ModifierFlag::SHIFT_L | ModifierFlag::FN);
  }

  // ENTER(+FN)
  key = KeyCode::ENTER; flags = ModifierFlag::SHIFT_L | ModifierFlag::FN; keyboardType = KeyboardType::MACBOOK;
  key.normalizeKey(flags, keyboardType);
  flags.add(ModifierFlag::FN);
  key.reverseNormalizeKey(flags, keyboardType);
  CHECK_REVERSENORMALIZEKEY(KeyCode::ENTER, ModifierFlag::SHIFT_L | ModifierFlag::FN);

  // FORWARD_DELETE(+FN)
  key = KeyCode::FORWARD_DELETE; flags = ModifierFlag::SHIFT_L | ModifierFlag::FN; keyboardType = KeyboardType::MACBOOK;
  key.normalizeKey(flags, keyboardType);
  flags.add(ModifierFlag::FN);
  key.reverseNormalizeKey(flags, keyboardType);
  CHECK_REVERSENORMALIZEKEY(KeyCode::FORWARD_DELETE, ModifierFlag::SHIFT_L | ModifierFlag::FN);
}

TEST(ModifierFlag, getKeyCode) {
  EXPECT_EQ(KeyCode::CAPSLOCK, ModifierFlag::getKeyCode(ModifierFlag::CAPSLOCK).get());
  EXPECT_EQ(KeyCode::SHIFT_L, ModifierFlag::getKeyCode(ModifierFlag::SHIFT_L).get());
  EXPECT_EQ(KeyCode::SHIFT_R, ModifierFlag::getKeyCode(ModifierFlag::SHIFT_R).get());
  EXPECT_EQ(KeyCode::CONTROL_L, ModifierFlag::getKeyCode(ModifierFlag::CONTROL_L).get());
  EXPECT_EQ(KeyCode::CONTROL_R, ModifierFlag::getKeyCode(ModifierFlag::CONTROL_R).get());
  EXPECT_EQ(KeyCode::OPTION_L, ModifierFlag::getKeyCode(ModifierFlag::OPTION_L).get());
  EXPECT_EQ(KeyCode::OPTION_R, ModifierFlag::getKeyCode(ModifierFlag::OPTION_R).get());
  EXPECT_EQ(KeyCode::COMMAND_L, ModifierFlag::getKeyCode(ModifierFlag::COMMAND_L).get());
  EXPECT_EQ(KeyCode::COMMAND_R, ModifierFlag::getKeyCode(ModifierFlag::COMMAND_R).get());
  EXPECT_EQ(KeyCode::FN, ModifierFlag::getKeyCode(ModifierFlag::FN).get());

  EXPECT_EQ(KeyCode::VK_NONE, ModifierFlag::getKeyCode(ModifierFlag::CURSOR).get());
  EXPECT_EQ(KeyCode::VK_NONE, ModifierFlag::getKeyCode(ModifierFlag::KEYPAD).get());

  EXPECT_EQ(KeyCode::VK_NONE, ModifierFlag::getKeyCode(ModifierFlag::CAPSLOCK | ModifierFlag::SHIFT_L).get());
}

TEST(KeyCode, getModifierFlag) {
  EXPECT_EQ(ModifierFlag::CAPSLOCK, KeyCode(KeyCode::CAPSLOCK).getModifierFlag());
  EXPECT_EQ(ModifierFlag::SHIFT_L, KeyCode(KeyCode::SHIFT_L).getModifierFlag());
  EXPECT_EQ(ModifierFlag::SHIFT_R, KeyCode(KeyCode::SHIFT_R).getModifierFlag());
  EXPECT_EQ(ModifierFlag::CONTROL_L, KeyCode(KeyCode::CONTROL_L).getModifierFlag());
  EXPECT_EQ(ModifierFlag::CONTROL_R, KeyCode(KeyCode::CONTROL_R).getModifierFlag());
  EXPECT_EQ(ModifierFlag::OPTION_L, KeyCode(KeyCode::OPTION_L).getModifierFlag());
  EXPECT_EQ(ModifierFlag::OPTION_R, KeyCode(KeyCode::OPTION_R).getModifierFlag());
  EXPECT_EQ(ModifierFlag::COMMAND_L, KeyCode(KeyCode::COMMAND_L).getModifierFlag());
  EXPECT_EQ(ModifierFlag::COMMAND_R, KeyCode(KeyCode::COMMAND_R).getModifierFlag());
  EXPECT_EQ(ModifierFlag::FN, KeyCode(KeyCode::FN).getModifierFlag());

  EXPECT_EQ(ModifierFlag::NONE, KeyCode(KeyCode::A).getModifierFlag());
  EXPECT_EQ(ModifierFlag::NONE, KeyCode(KeyCode::VK_NONE).getModifierFlag());
}

TEST(KeyCode, isModifier) {
  EXPECT_EQ(true, KeyCode(KeyCode::CAPSLOCK).isModifier());
  EXPECT_EQ(true, KeyCode(KeyCode::SHIFT_L).isModifier());
  EXPECT_EQ(false, KeyCode(KeyCode::A).isModifier());
  EXPECT_EQ(false, KeyCode(KeyCode::VK_NONE).isModifier());
}
