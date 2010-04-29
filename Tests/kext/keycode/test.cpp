#include <ostream>
#include <gtest/gtest.h>
#include "KeyCode.hpp"

using namespace org_pqrs_KeyRemap4MacBook;

std::ostream& operator<<(std::ostream& os, const EventType& v) { return os << v.get(); }
std::ostream& operator<<(std::ostream& os, const KeyboardType& v) { return os << v.get(); }
std::ostream& operator<<(std::ostream& os, const ModifierFlag& v) { return os << v.get(); }
std::ostream& operator<<(std::ostream& os, const Flags& v) { return os << v.get(); }
std::ostream& operator<<(std::ostream& os, const KeyCode& v) { return os << v.get(); }
std::ostream& operator<<(std::ostream& os, const ConsumerKeyCode& v) { return os << v.get(); }
std::ostream& operator<<(std::ostream& os, const PointingButton& v) { return os << v.get(); }
std::ostream& operator<<(std::ostream& os, const Buttons& v) { return os << v.get(); }

TEST(Generic, sizeof_) {
  EXPECT_EQ(sizeof(unsigned int), sizeof(EventType));
  EXPECT_EQ(sizeof(unsigned int), sizeof(KeyboardType));
  EXPECT_EQ(sizeof(unsigned int), sizeof(ModifierFlag));
  EXPECT_EQ(sizeof(unsigned int), sizeof(Flags));
  EXPECT_EQ(sizeof(unsigned int), sizeof(KeyCode));
  EXPECT_EQ(sizeof(unsigned int), sizeof(ConsumerKeyCode));
  EXPECT_EQ(sizeof(unsigned int), sizeof(PointingButton));
  EXPECT_EQ(sizeof(unsigned int), sizeof(Buttons));
}

TEST(EventType, isKeyDownOrModifierDown) {
  EXPECT_TRUE(EventType::DOWN.isKeyDownOrModifierDown(KeyCode::A, 0));
  EXPECT_FALSE(EventType::UP.isKeyDownOrModifierDown(KeyCode::A, 0));

  EXPECT_TRUE(EventType::MODIFY.isKeyDownOrModifierDown(KeyCode::SHIFT_L, ModifierFlag::SHIFT_L | ModifierFlag::CONTROL_L));
  EXPECT_FALSE(EventType::MODIFY.isKeyDownOrModifierDown(KeyCode::SHIFT_L, ModifierFlag::CONTROL_L));
}

TEST(ModifierFlag, get) {
  EXPECT_EQ(static_cast<unsigned int>(0x10000), ModifierFlag::CAPSLOCK.get());
}

TEST(Flags, add) {
  Flags mask = ModifierFlag::SHIFT_L | ModifierFlag::CONTROL_R | ModifierFlag::COMMAND_R;
  Flags flags = mask;
  EXPECT_EQ(mask, flags.stripFN());

  flags.add(ModifierFlag::OPTION_L);
  EXPECT_EQ(mask | ModifierFlag::OPTION_L, flags);

  flags.add(ModifierFlag::NONE);
  EXPECT_EQ(mask | ModifierFlag::OPTION_L | ModifierFlag::NONE, flags);
}

TEST(Flags, remove) {
  Flags flags = ModifierFlag::SHIFT_L | ModifierFlag::CONTROL_R | ModifierFlag::COMMAND_R;
  Flags removed = ModifierFlag::CONTROL_R | ModifierFlag::COMMAND_R;
  EXPECT_EQ(removed, flags.remove(ModifierFlag::SHIFT_L));
}

TEST(Flags, stripFN) {
  Flags mask = ModifierFlag::SHIFT_L | ModifierFlag::CONTROL_R | ModifierFlag::COMMAND_R;
  Flags flags = mask;
  EXPECT_EQ(mask, flags.stripFN());

  flags.add(ModifierFlag::FN);
  EXPECT_EQ(mask, flags.stripFN());
}

TEST(Flags, stripEXTRA) {
  Flags mask = ModifierFlag::SHIFT_L | ModifierFlag::CONTROL_R | ModifierFlag::COMMAND_R;
  Flags flags = mask;
  EXPECT_EQ(mask, flags.stripEXTRA());

  flags.add(ModifierFlag::EXTRA1);
  EXPECT_EQ(mask, flags.stripEXTRA());

  flags.add(ModifierFlag::EXTRA2);
  EXPECT_EQ(mask, flags.stripEXTRA());

  flags.add(ModifierFlag::EXTRA3 | ModifierFlag::EXTRA5);
  EXPECT_EQ(mask, flags.stripEXTRA());
}

TEST(Flags, stripCURSOR) {
  Flags mask = ModifierFlag::SHIFT_L | ModifierFlag::CONTROL_R | ModifierFlag::COMMAND_R;
  Flags flags = mask;
  EXPECT_EQ(mask, flags.stripCURSOR());

  flags.add(ModifierFlag::CURSOR);
  EXPECT_EQ(mask, flags.stripCURSOR());
}

TEST(Flags, stripKEYPAD) {
  Flags mask = ModifierFlag::SHIFT_L | ModifierFlag::CONTROL_R | ModifierFlag::COMMAND_R;
  Flags flags = mask;
  EXPECT_EQ(mask, flags.stripKEYPAD());

  flags.add(ModifierFlag::KEYPAD);
  EXPECT_EQ(mask, flags.stripKEYPAD());
}

TEST(Flags, stripNONE) {
  Flags mask = ModifierFlag::SHIFT_L | ModifierFlag::CONTROL_R | ModifierFlag::COMMAND_R;
  Flags flags = mask;
  EXPECT_EQ(mask, flags.stripNONE());

  flags.add(ModifierFlag::NONE);
  EXPECT_EQ(mask, flags.stripNONE());
}

TEST(Flags, isOn) {
  Flags mask = ModifierFlag::SHIFT_L | ModifierFlag::CONTROL_R | ModifierFlag::COMMAND_R;
  Flags flags = mask;

  EXPECT_TRUE(flags.isOn(Flags(0)));

  EXPECT_TRUE(flags.isOn(ModifierFlag::SHIFT_L));
  EXPECT_FALSE(flags.isOn(ModifierFlag::SHIFT_R));

  flags = ModifierFlag::NONE;
  EXPECT_TRUE(flags.isOn(ModifierFlag::NONE));

  flags = 0;
  EXPECT_FALSE(flags.isOn(ModifierFlag::NONE));
  EXPECT_TRUE(flags.isOn(Flags(ModifierFlag::NONE)));
  EXPECT_TRUE(flags.isOn(Flags(0)));

  flags = ModifierFlag::SHIFT_L | ModifierFlag::CONTROL_R | ModifierFlag::COMMAND_R;
  EXPECT_TRUE(flags.isOn(ModifierFlag::SHIFT_L | ModifierFlag::CONTROL_R));
  EXPECT_TRUE(flags.isOn(Flags(0)));
  EXPECT_TRUE(flags.isOn(ModifierFlag::SHIFT_L | ModifierFlag::CONTROL_R | ModifierFlag::COMMAND_R | ModifierFlag::NONE));
  EXPECT_FALSE(flags.isOn(ModifierFlag::SHIFT_L | ModifierFlag::CONTROL_R | ModifierFlag::NONE));
}

namespace {
  KeyCode keypads[][2] = {
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

  KeyCode cursors[][2] = {
    { KeyCode::PAGEUP, KeyCode::CURSOR_UP },
    { KeyCode::PAGEDOWN, KeyCode::CURSOR_DOWN },
    { KeyCode::HOME, KeyCode::CURSOR_LEFT },
    { KeyCode::END, KeyCode::CURSOR_RIGHT },
  };
}

TEST(KeyCode, op) {
  EXPECT_TRUE(KeyCode::A == KeyCode::A);
  EXPECT_TRUE(KeyCode::VK_NONE > KeyCode::A);
}

TEST(KeyCode, get) {
  EXPECT_EQ(static_cast<unsigned int>(57), KeyCode::CAPSLOCK.get());
}

TEST(KeyCode, normalizeKey) {
  KeyCode key;
  Flags flags;
  KeyboardType keyboardType;

#define CHECK_NORMALIZEKEY(KEYCODE, FLAGS) { \
    EXPECT_EQ(KEYCODE, key);                 \
    EXPECT_EQ(Flags(flags), FLAGS);          \
}

  // ENTER_POWERBOOK -> ENTER
  key = KeyCode::ENTER_POWERBOOK; flags = ModifierFlag::SHIFT_L; keyboardType = KeyboardType::POWERBOOK;
  key.normalizeKey(flags, keyboardType);
  CHECK_NORMALIZEKEY(KeyCode::ENTER, ModifierFlag::SHIFT_L);

  // ENTER_POWERBOOK(+FN) -> ENTER(+FN) -> RETURN
  key = KeyCode::ENTER_POWERBOOK; flags = ModifierFlag::SHIFT_L | ModifierFlag::FN; keyboardType = KeyboardType::POWERBOOK;
  key.normalizeKey(flags, keyboardType);
  CHECK_NORMALIZEKEY(KeyCode::RETURN, ModifierFlag::SHIFT_L | ModifierFlag::FN);

  // normal key
  key = KeyCode::A; flags = ModifierFlag::SHIFT_L; keyboardType = KeyboardType::MACBOOK;
  key.normalizeKey(flags, keyboardType);
  CHECK_NORMALIZEKEY(KeyCode::A, ModifierFlag::SHIFT_L);

  // KEYPAD
  for (size_t i = 0; i < sizeof(keypads) / sizeof(keypads[0]); ++i) {
    key = keypads[i][0]; flags = ModifierFlag::SHIFT_L; keyboardType = KeyboardType::MACBOOK;
    if (key != KeyCode::KEYPAD_CLEAR && key != KeyCode::KEYPAD_COMMA) flags.add(ModifierFlag::KEYPAD);
    Flags flags_orig = flags;
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

  key = KeyCode::K; flags = ModifierFlag::SHIFT_L | ModifierFlag::FN; keyboardType = KeyboardType::MACBOOK;
  key.normalizeKey(flags, keyboardType);
  CHECK_NORMALIZEKEY(KeyCode::K, ModifierFlag::SHIFT_L | ModifierFlag::FN);

  // KEYPAD(+FN)
  for (size_t i = 0; i < sizeof(keypads) / sizeof(keypads[0]); ++i) {
    key = keypads[i][0]; flags = ModifierFlag::SHIFT_L | ModifierFlag::FN; keyboardType = KeyboardType::MACBOOK;
    if (key != KeyCode::KEYPAD_CLEAR && key != KeyCode::KEYPAD_COMMA) flags.add(ModifierFlag::KEYPAD);
    if (key == KeyCode::KEYPAD_COMMA) continue;
    key.normalizeKey(flags, keyboardType);
    CHECK_NORMALIZEKEY(keypads[i][1], ModifierFlag::SHIFT_L | ModifierFlag::FN);
  }

  // PAGEUP(+FN)
  for (size_t i = 0; i < sizeof(cursors) / sizeof(cursors[0]); ++i) {
    key = cursors[i][0]; flags = ModifierFlag::SHIFT_L | ModifierFlag::FN; keyboardType = KeyboardType::MACBOOK;
    key.normalizeKey(flags, keyboardType);
    CHECK_NORMALIZEKEY(cursors[i][1], ModifierFlag::SHIFT_L | ModifierFlag::FN | ModifierFlag::CURSOR);
  }

  // ENTER(+FN)
  key = KeyCode::ENTER; flags = ModifierFlag::SHIFT_L | ModifierFlag::FN; keyboardType = KeyboardType::MACBOOK;
  key.normalizeKey(flags, keyboardType);
  CHECK_NORMALIZEKEY(KeyCode::RETURN, ModifierFlag::SHIFT_L | ModifierFlag::FN);

  // FORWARD_DELETE(+FN)
  key = KeyCode::FORWARD_DELETE; flags = ModifierFlag::SHIFT_L | ModifierFlag::FN; keyboardType = KeyboardType::MACBOOK;
  key.normalizeKey(flags, keyboardType);
  CHECK_NORMALIZEKEY(KeyCode::DELETE, ModifierFlag::SHIFT_L | ModifierFlag::FN);
}

TEST(KeyCode, reverseNormalizeKey) {
  KeyCode key;
  Flags flags;
  KeyboardType keyboardType;

#define CHECK_REVERSENORMALIZEKEY(KEYCODE, MODIFIERFLAG) { \
    EXPECT_EQ(KEYCODE, key);                               \
    EXPECT_EQ(Flags(MODIFIERFLAG), flags);                 \
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
    if (key != KeyCode::KEYPAD_CLEAR && key != KeyCode::KEYPAD_COMMA) flags.add(ModifierFlag::KEYPAD);
    Flags flags_orig = flags;
    key.normalizeKey(flags, keyboardType);
    key.reverseNormalizeKey(flags, keyboardType);
    CHECK_REVERSENORMALIZEKEY(keypads[i][0], flags_orig);
  }

  // PAGEUP
  for (size_t i = 0; i < sizeof(cursors) / sizeof(cursors[0]); ++i) {
    key = cursors[i][0]; flags = ModifierFlag::SHIFT_L; keyboardType = KeyboardType::MACBOOK;
    key.normalizeKey(flags, keyboardType);
    key.reverseNormalizeKey(flags, keyboardType);
    CHECK_REVERSENORMALIZEKEY(cursors[i][0], ModifierFlag::SHIFT_L | ModifierFlag::FN);
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
  CHECK_REVERSENORMALIZEKEY(KeyCode::FORWARD_DELETE, ModifierFlag::SHIFT_L | ModifierFlag::FN);

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
    if (key != KeyCode::KEYPAD_CLEAR && key != KeyCode::KEYPAD_COMMA) flags.add(ModifierFlag::KEYPAD);
    if (key == KeyCode::KEYPAD_COMMA) continue;
    Flags flags_orig = flags;
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
  EXPECT_EQ(KeyCode::CAPSLOCK, ModifierFlag::CAPSLOCK.getKeyCode());
  EXPECT_EQ(KeyCode::SHIFT_L, ModifierFlag::SHIFT_L.getKeyCode());
  EXPECT_EQ(KeyCode::SHIFT_R, ModifierFlag::SHIFT_R.getKeyCode());
  EXPECT_EQ(KeyCode::CONTROL_L, ModifierFlag::CONTROL_L.getKeyCode());
  EXPECT_EQ(KeyCode::CONTROL_R, ModifierFlag::CONTROL_R.getKeyCode());
  EXPECT_EQ(KeyCode::OPTION_L, ModifierFlag::OPTION_L.getKeyCode());
  EXPECT_EQ(KeyCode::OPTION_R, ModifierFlag::OPTION_R.getKeyCode());
  EXPECT_EQ(KeyCode::COMMAND_L, ModifierFlag::COMMAND_L.getKeyCode());
  EXPECT_EQ(KeyCode::COMMAND_R, ModifierFlag::COMMAND_R.getKeyCode());
  EXPECT_EQ(KeyCode::FN, ModifierFlag::FN.getKeyCode());
  EXPECT_EQ(KeyCode::VK_MODIFIER_EXTRA1, ModifierFlag::EXTRA1.getKeyCode());
  EXPECT_EQ(KeyCode::VK_MODIFIER_EXTRA2, ModifierFlag::EXTRA2.getKeyCode());
  EXPECT_EQ(KeyCode::VK_MODIFIER_EXTRA3, ModifierFlag::EXTRA3.getKeyCode());
  EXPECT_EQ(KeyCode::VK_MODIFIER_EXTRA4, ModifierFlag::EXTRA4.getKeyCode());
  EXPECT_EQ(KeyCode::VK_MODIFIER_EXTRA5, ModifierFlag::EXTRA5.getKeyCode());

  EXPECT_EQ(KeyCode::VK_NONE, ModifierFlag::CURSOR.getKeyCode());
  EXPECT_EQ(KeyCode::VK_NONE, ModifierFlag::KEYPAD.getKeyCode());
}

TEST(KeyCode, getModifierFlag) {
  EXPECT_EQ(ModifierFlag::CAPSLOCK, KeyCode::CAPSLOCK.getModifierFlag());
  EXPECT_EQ(ModifierFlag::SHIFT_L, KeyCode::SHIFT_L.getModifierFlag());
  EXPECT_EQ(ModifierFlag::SHIFT_R, KeyCode::SHIFT_R.getModifierFlag());
  EXPECT_EQ(ModifierFlag::CONTROL_L, KeyCode::CONTROL_L.getModifierFlag());
  EXPECT_EQ(ModifierFlag::CONTROL_R, KeyCode::CONTROL_R.getModifierFlag());
  EXPECT_EQ(ModifierFlag::OPTION_L, KeyCode::OPTION_L.getModifierFlag());
  EXPECT_EQ(ModifierFlag::OPTION_R, KeyCode::OPTION_R.getModifierFlag());
  EXPECT_EQ(ModifierFlag::COMMAND_L, KeyCode::COMMAND_L.getModifierFlag());
  EXPECT_EQ(ModifierFlag::COMMAND_R, KeyCode::COMMAND_R.getModifierFlag());
  EXPECT_EQ(ModifierFlag::FN, KeyCode::FN.getModifierFlag());
  EXPECT_EQ(ModifierFlag::EXTRA1, KeyCode::VK_MODIFIER_EXTRA1.getModifierFlag());
  EXPECT_EQ(ModifierFlag::EXTRA2, KeyCode::VK_MODIFIER_EXTRA2.getModifierFlag());
  EXPECT_EQ(ModifierFlag::EXTRA3, KeyCode::VK_MODIFIER_EXTRA3.getModifierFlag());
  EXPECT_EQ(ModifierFlag::EXTRA4, KeyCode::VK_MODIFIER_EXTRA4.getModifierFlag());
  EXPECT_EQ(ModifierFlag::EXTRA5, KeyCode::VK_MODIFIER_EXTRA5.getModifierFlag());

  EXPECT_EQ(ModifierFlag::NONE, KeyCode(KeyCode::A).getModifierFlag());
  EXPECT_EQ(ModifierFlag::NONE, KeyCode(KeyCode::VK_NONE).getModifierFlag());
}

TEST(KeyCode, isModifier) {
  EXPECT_EQ(true, KeyCode(KeyCode::CAPSLOCK).isModifier());
  EXPECT_EQ(true, KeyCode(KeyCode::SHIFT_L).isModifier());
  EXPECT_EQ(false, KeyCode(KeyCode::A).isModifier());
  EXPECT_EQ(false, KeyCode(KeyCode::VK_NONE).isModifier());
}

TEST(Buttons, add) {
  Buttons buttons;
  buttons.add(PointingButton::LEFT);
  EXPECT_EQ(Buttons(PointingButton::LEFT), buttons);

  buttons.add(PointingButton::MIDDLE);
  EXPECT_EQ(Buttons(PointingButton::LEFT | PointingButton::MIDDLE), buttons);

  // some PointingButton twice.
  buttons.add(PointingButton::LEFT);
  EXPECT_EQ(Buttons(PointingButton::LEFT | PointingButton::MIDDLE), buttons);

  buttons = 0;
  buttons.add(PointingButton::LEFT | PointingButton::MIDDLE);
  EXPECT_EQ(Buttons(PointingButton::LEFT | PointingButton::MIDDLE), buttons);
}

TEST(Buttons, remove) {
  Buttons buttons(PointingButton::LEFT | PointingButton::MIDDLE);
  buttons.remove(PointingButton::LEFT);
  EXPECT_EQ(Buttons(PointingButton::MIDDLE), buttons);

  // unexist PointingButton.
  buttons.remove(PointingButton::RIGHT);
  EXPECT_EQ(Buttons(PointingButton::MIDDLE), buttons);

  buttons = PointingButton::LEFT | PointingButton::MIDDLE | PointingButton::RIGHT;
  buttons.remove(PointingButton::LEFT | PointingButton::RIGHT);
  EXPECT_EQ(Buttons(PointingButton::MIDDLE), buttons);
}

TEST(Buttons, isOn) {
  Buttons buttons(PointingButton::LEFT | PointingButton::MIDDLE | PointingButton::BUTTON4);
  EXPECT_TRUE(buttons.isOn(PointingButton::LEFT));
  EXPECT_TRUE(buttons.isOn(PointingButton::LEFT | PointingButton::MIDDLE));

  EXPECT_FALSE(buttons.isOn(PointingButton::RIGHT));
  EXPECT_FALSE(buttons.isOn(PointingButton::LEFT | PointingButton::RIGHT));
}
