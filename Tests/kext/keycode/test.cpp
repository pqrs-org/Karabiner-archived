#include <ostream>
#include <vector>
#include <gtest/gtest.h>
#include "KeyCode.hpp"
#include "Config.hpp"

using namespace org_pqrs_KeyRemap4MacBook;
Config config;

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

TEST(Flags, isVirtualModifiersOn) {
  Flags flags = ModifierFlag::SHIFT_L | ModifierFlag::CONTROL_R | ModifierFlag::COMMAND_R;
  EXPECT_FALSE(flags.isVirtualModifiersOn());

  flags = 0;
  EXPECT_FALSE(flags.isVirtualModifiersOn());

  flags = ModifierFlag::SHIFT_L | ModifierFlag::CONTROL_R | ModifierFlag::COMMAND_R | ModifierFlag::NONE;
  EXPECT_TRUE(flags.isVirtualModifiersOn());

  flags = ModifierFlag::SHIFT_L | ModifierFlag::CONTROL_R | ModifierFlag::COMMAND_R | ModifierFlag::EXTRA3;
  EXPECT_TRUE(flags.isVirtualModifiersOn());
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

struct NormalizeItem {
  NormalizeItem(KeyCode fk, Flags ff, KeyCode tk, Flags tf, KeyboardType kt) : fromKeyCode(fk), fromFlags(ff), toKeyCode(tk), toFlags(tf), keyboardType(kt) {}

  KeyCode fromKeyCode;
  Flags fromFlags;
  KeyCode toKeyCode;
  Flags toFlags;
  KeyboardType keyboardType;
};

TEST(KeyCode, normalizeKey) {
  std::vector<NormalizeItem> vec;

  // ENTER_POWERBOOK (without FN) -> ENTER
  vec.push_back(NormalizeItem(KeyCode::ENTER_POWERBOOK, ModifierFlag::SHIFT_L,
                              KeyCode::ENTER,           ModifierFlag::SHIFT_L, KeyboardType::POWERBOOK));

  // ENTER_POWERBOOK(+FN) -> ENTER(+FN) -> RETURN
  vec.push_back(NormalizeItem(KeyCode::ENTER_POWERBOOK, ModifierFlag::SHIFT_L | ModifierFlag::FN,
                              KeyCode::RETURN,          ModifierFlag::SHIFT_L | ModifierFlag::FN, KeyboardType::POWERBOOK));

  // ----------------------------------------
  // normal key (without FN)
  vec.push_back(NormalizeItem(KeyCode::A, ModifierFlag::SHIFT_L,
                              KeyCode::A, ModifierFlag::SHIFT_L, KeyboardType::MACBOOK));

  // KEYPAD (without FN) -> Don't change key.
  for (size_t i = 0; i < sizeof(keypads) / sizeof(keypads[0]); ++i) {
    vec.push_back(NormalizeItem(keypads[i][0], ModifierFlag::SHIFT_L,
                                keypads[i][0], ModifierFlag::SHIFT_L, KeyboardType::MACBOOK));
  }

  // PAGEUP (without FN) -> Don't change key.
  for (size_t i = 0; i < sizeof(cursors) / sizeof(cursors[0]); ++i) {
    vec.push_back(NormalizeItem(cursors[i][0], ModifierFlag::SHIFT_L,
                                cursors[i][0], ModifierFlag::SHIFT_L, KeyboardType::MACBOOK));
  }

  // ENTER (without FN)
  vec.push_back(NormalizeItem(KeyCode::ENTER, ModifierFlag::SHIFT_L,
                              KeyCode::ENTER, ModifierFlag::SHIFT_L, KeyboardType::MACBOOK));

  // FORWARD_DELETE (without FN)
  vec.push_back(NormalizeItem(KeyCode::FORWARD_DELETE, ModifierFlag::SHIFT_L,
                              KeyCode::FORWARD_DELETE, ModifierFlag::SHIFT_L, KeyboardType::MACBOOK));

  // ----------------------------------------
  // normal key(+FN)
  vec.push_back(NormalizeItem(KeyCode::A, ModifierFlag::SHIFT_L | ModifierFlag::FN,
                              KeyCode::A, ModifierFlag::SHIFT_L | ModifierFlag::FN, KeyboardType::MACBOOK));

  vec.push_back(NormalizeItem(KeyCode::K, ModifierFlag::SHIFT_L | ModifierFlag::FN,
                              KeyCode::K, ModifierFlag::SHIFT_L | ModifierFlag::FN, KeyboardType::MACBOOK));

  // KEYPAD(+FN) -> JKL...+FN
  for (size_t i = 0; i < sizeof(keypads) / sizeof(keypads[0]); ++i) {
    vec.push_back(NormalizeItem(keypads[i][0], ModifierFlag::SHIFT_L | ModifierFlag::FN,
                                keypads[i][1], ModifierFlag::SHIFT_L | ModifierFlag::FN, KeyboardType::MACBOOK));
  }

  // PAGEUP(+FN) -> Arrow+FN
  for (size_t i = 0; i < sizeof(cursors) / sizeof(cursors[0]); ++i) {
    vec.push_back(NormalizeItem(cursors[i][0], ModifierFlag::SHIFT_L | ModifierFlag::FN,
                                cursors[i][1], ModifierFlag::SHIFT_L | ModifierFlag::FN, KeyboardType::MACBOOK));
  }

  // ENTER(+FN) -> RETURN+FN
  vec.push_back(NormalizeItem(KeyCode::ENTER,  ModifierFlag::SHIFT_L | ModifierFlag::FN,
                              KeyCode::RETURN, ModifierFlag::SHIFT_L | ModifierFlag::FN, KeyboardType::MACBOOK));

  // FORWARD_DELETE(+FN) -> DELETE+FN
  vec.push_back(NormalizeItem(KeyCode::FORWARD_DELETE, ModifierFlag::SHIFT_L | ModifierFlag::FN,
                              KeyCode::DELETE,         ModifierFlag::SHIFT_L | ModifierFlag::FN, KeyboardType::MACBOOK));

  // ----------------------------------------
  for (std::vector<NormalizeItem>::iterator it = vec.begin(); it != vec.end(); ++it) {
    KeyCode key;

    // Down
    key = it->fromKeyCode;
    KeyCode::normalizeKey(key, it->fromFlags, EventType::DOWN, it->keyboardType);
    EXPECT_EQ(it->toKeyCode, key);
    EXPECT_EQ(it->fromFlags, it->toFlags);

    // Up
    key = it->fromKeyCode;
    KeyCode::normalizeKey(key, it->fromFlags, EventType::UP, it->keyboardType);
    EXPECT_EQ(it->toKeyCode, key);
    EXPECT_EQ(it->fromFlags, it->toFlags);
  }

  // ======================================================================
  // Test case for the following key sequence.
  //  (1) FN Down
  //  (2) Right-Arrow Down
  //  (3) FN Up
  //  (4) Right-Arrow Up
  // KeyRemap4MacBook KeyboardEventCallback [ caught]: eventType 12, flags 0x800000, key 63, kbdType 37
  // KeyRemap4MacBook KeyboardEventCallback [ caught]: eventType 10, flags 0x800000, key 119, kbdType 37
  // KeyRemap4MacBook KeyboardEventCallback [ caught]: eventType 12, flags 0x0, key 63, kbdType 37
  // KeyRemap4MacBook KeyboardEventCallback [ caught]: eventType 11, flags 0x0, key 119, kbdType 37
  {
    KeyCode key = KeyCode::END;
    Flags flags = ModifierFlag::FN;
    KeyCode::normalizeKey(key, flags, EventType::DOWN, KeyboardType::MACBOOK);
    EXPECT_EQ(key, KeyCode::CURSOR_RIGHT);
    EXPECT_EQ(flags, ModifierFlag::FN);

    key = KeyCode::END;
    flags = Flags(0);
    KeyCode::normalizeKey(key, flags, EventType::UP, KeyboardType::MACBOOK);
    EXPECT_EQ(key, KeyCode::CURSOR_RIGHT);
    EXPECT_EQ(flags, Flags(0));
  }

  // Test case for the following key sequence.
  //  (1) Right-Arrow Down
  //  (2) FN Down
  //  (3) Right-Arrow UP
  //  (4) FN Up
  // KeyRemap4MacBook KeyboardEventCallback [ caught]: eventType 10, flags 0x200000, key 124, kbdType 37
  // KeyRemap4MacBook KeyboardEventCallback [ caught]: eventType 12, flags 0xa00000, key 63, kbdType 37
  // KeyRemap4MacBook KeyboardEventCallback [ caught]: eventType 11, flags 0xa00000, key 124, kbdType 37
  // KeyRemap4MacBook KeyboardEventCallback [ caught]: eventType 12, flags 0x0, key 63, kbdType 37
  {
    KeyCode key = KeyCode::CURSOR_RIGHT;
    Flags flags = Flags(ModifierFlag::CURSOR);
    KeyCode::normalizeKey(key, flags, EventType::DOWN, KeyboardType::MACBOOK);
    EXPECT_EQ(key, KeyCode::CURSOR_RIGHT);
    EXPECT_EQ(flags, Flags(0));

    key = KeyCode::CURSOR_RIGHT;
    flags = Flags(ModifierFlag::FN | ModifierFlag::CURSOR);
    KeyCode::normalizeKey(key, flags, EventType::UP, KeyboardType::MACBOOK);
    EXPECT_EQ(key, KeyCode::CURSOR_RIGHT);
    EXPECT_EQ(flags, ModifierFlag::FN);
  }
}

TEST(KeyCode, reverseNormalizeKey) {
  std::vector<NormalizeItem> vec;

  // ENTER_POWERBOOK
  vec.push_back(NormalizeItem(KeyCode::ENTER_POWERBOOK, ModifierFlag::SHIFT_L,
                              KeyCode::ENTER_POWERBOOK, ModifierFlag::SHIFT_L, KeyboardType::POWERBOOK));

  // ENTER_POWERBOOK(+FN)
  vec.push_back(NormalizeItem(KeyCode::ENTER_POWERBOOK, ModifierFlag::SHIFT_L | ModifierFlag::FN,
                              KeyCode::ENTER_POWERBOOK, ModifierFlag::SHIFT_L | ModifierFlag::FN, KeyboardType::POWERBOOK));

  // ----------------------------------------
  // normal key (without FN)
  vec.push_back(NormalizeItem(KeyCode::A, ModifierFlag::SHIFT_L,
                              KeyCode::A, ModifierFlag::SHIFT_L, KeyboardType::MACBOOK));

  // KEYPAD (without FN)
  for (size_t i = 0; i < sizeof(keypads) / sizeof(keypads[0]); ++i) {
    KeyCode key = keypads[i][0];
    Flags fromFlags = ModifierFlag::SHIFT_L;
    Flags toFlags = fromFlags;
    if (key != KeyCode::KEYPAD_CLEAR && key != KeyCode::KEYPAD_COMMA) toFlags.add(ModifierFlag::KEYPAD);

    vec.push_back(NormalizeItem(key, fromFlags,
                                key, toFlags, KeyboardType::MACBOOK));
  }

  // PAGEUP (without FN)
  for (size_t i = 0; i < sizeof(cursors) / sizeof(cursors[0]); ++i) {
    vec.push_back(NormalizeItem(cursors[i][0], ModifierFlag::SHIFT_L,
                                cursors[i][0], ModifierFlag::SHIFT_L | ModifierFlag::FN, KeyboardType::MACBOOK));
  }

  // ENTER (without FN)
  vec.push_back(NormalizeItem(KeyCode::ENTER, ModifierFlag::SHIFT_L,
                              KeyCode::ENTER, ModifierFlag::SHIFT_L, KeyboardType::MACBOOK));

  // FORWARD_DELETE (without FN)
  vec.push_back(NormalizeItem(KeyCode::FORWARD_DELETE, ModifierFlag::SHIFT_L,
                              KeyCode::FORWARD_DELETE, ModifierFlag::SHIFT_L | ModifierFlag::FN, KeyboardType::MACBOOK));

  // CURSOR (without FN)
  for (size_t i = 0; i < sizeof(cursors) / sizeof(cursors[0]); ++i) {
    vec.push_back(NormalizeItem(cursors[i][1], ModifierFlag::CURSOR,
                                cursors[i][1], ModifierFlag::CURSOR, KeyboardType::MACBOOK));
  }

  // ----------------------------------------
  // normal key(+FN)
  vec.push_back(NormalizeItem(KeyCode::A, ModifierFlag::SHIFT_L | ModifierFlag::FN,
                              KeyCode::A, ModifierFlag::SHIFT_L | ModifierFlag::FN, KeyboardType::MACBOOK));

  // KEYPAD(+FN)
  for (size_t i = 0; i < sizeof(keypads) / sizeof(keypads[0]); ++i) {
    KeyCode key = keypads[i][0];
    Flags fromFlags = ModifierFlag::SHIFT_L | ModifierFlag::FN;
    Flags toFlags = fromFlags;
    if (key != KeyCode::KEYPAD_CLEAR && key != KeyCode::KEYPAD_COMMA) toFlags.add(ModifierFlag::KEYPAD);

    vec.push_back(NormalizeItem(key, fromFlags,
                                key, toFlags, KeyboardType::MACBOOK));
  }

  // PAGEUP(+FN)
  for (size_t i = 0; i < sizeof(cursors) / sizeof(cursors[0]); ++i) {
    vec.push_back(NormalizeItem(cursors[i][0], ModifierFlag::SHIFT_L | ModifierFlag::FN,
                                cursors[i][0], ModifierFlag::SHIFT_L | ModifierFlag::FN, KeyboardType::MACBOOK));
  }

  // ENTER(+FN)
  vec.push_back(NormalizeItem(KeyCode::ENTER, ModifierFlag::SHIFT_L | ModifierFlag::FN,
                              KeyCode::ENTER, ModifierFlag::SHIFT_L | ModifierFlag::FN, KeyboardType::MACBOOK));

  // FORWARD_DELETE(+FN)
  vec.push_back(NormalizeItem(KeyCode::FORWARD_DELETE, ModifierFlag::SHIFT_L | ModifierFlag::FN,
                              KeyCode::FORWARD_DELETE, ModifierFlag::SHIFT_L | ModifierFlag::FN, KeyboardType::MACBOOK));

  // ----------------------------------------
  for (std::vector<NormalizeItem>::iterator it = vec.begin(); it != vec.end(); ++it) {
    KeyCode key;

    // Down
    key = it->fromKeyCode;
    KeyCode::normalizeKey(key, it->fromFlags, EventType::DOWN, it->keyboardType);
    KeyCode::reverseNormalizeKey(key, it->fromFlags, EventType::DOWN, it->keyboardType);
    EXPECT_EQ(it->toKeyCode, key);
    EXPECT_EQ(it->fromFlags, it->toFlags);

    // Up
    key = it->fromKeyCode;
    KeyCode::normalizeKey(key, it->fromFlags, EventType::UP, it->keyboardType);
    KeyCode::reverseNormalizeKey(key, it->fromFlags, EventType::UP, it->keyboardType);
    EXPECT_EQ(it->toKeyCode, key);
    EXPECT_EQ(it->fromFlags, it->toFlags);
  }

  // ======================================================================
  // Test case for the following key sequence.
  //  (1) FN Down
  //  (2) Right-Arrow Down
  //  (3) FN Up
  //  (4) Right-Arrow Up
  // KeyRemap4MacBook KeyboardEventCallback [ caught]: eventType 12, flags 0x800000, key 63, kbdType 37
  // KeyRemap4MacBook KeyboardEventCallback [ caught]: eventType 10, flags 0x800000, key 119, kbdType 37
  // KeyRemap4MacBook KeyboardEventCallback [ caught]: eventType 12, flags 0x0, key 63, kbdType 37
  // KeyRemap4MacBook KeyboardEventCallback [ caught]: eventType 11, flags 0x0, key 119, kbdType 37
  {
    KeyCode key = KeyCode::END;
    Flags flags = ModifierFlag::FN;
    KeyCode::normalizeKey(key, flags, EventType::DOWN, KeyboardType::MACBOOK);
    KeyCode::reverseNormalizeKey(key, flags, EventType::DOWN, KeyboardType::MACBOOK);
    EXPECT_EQ(key, KeyCode::END);
    EXPECT_EQ(flags, ModifierFlag::FN);

    key = KeyCode::END;
    flags = Flags(0);
    KeyCode::normalizeKey(key, flags, EventType::UP, KeyboardType::MACBOOK);
    KeyCode::reverseNormalizeKey(key, flags, EventType::UP, KeyboardType::MACBOOK);
    EXPECT_EQ(key, KeyCode::END);
    EXPECT_EQ(flags, ModifierFlag::FN);
  }

  // Test case for the following key sequence.
  //  (1) Right-Arrow Down
  //  (2) FN Down
  //  (3) Right-Arrow UP
  //  (4) FN Up
  // KeyRemap4MacBook KeyboardEventCallback [ caught]: eventType 10, flags 0x200000, key 124, kbdType 37
  // KeyRemap4MacBook KeyboardEventCallback [ caught]: eventType 12, flags 0xa00000, key 63, kbdType 37
  // KeyRemap4MacBook KeyboardEventCallback [ caught]: eventType 11, flags 0xa00000, key 124, kbdType 37
  // KeyRemap4MacBook KeyboardEventCallback [ caught]: eventType 12, flags 0x0, key 63, kbdType 37
  {
    KeyCode key = KeyCode::CURSOR_RIGHT;
    Flags flags = Flags(ModifierFlag::CURSOR);
    KeyCode::normalizeKey(key, flags, EventType::DOWN, KeyboardType::MACBOOK);
    KeyCode::reverseNormalizeKey(key, flags, EventType::DOWN, KeyboardType::MACBOOK);
    EXPECT_EQ(key, KeyCode::CURSOR_RIGHT);
    EXPECT_EQ(flags, ModifierFlag::CURSOR);

    key = KeyCode::CURSOR_RIGHT;
    flags = Flags(ModifierFlag::FN | ModifierFlag::CURSOR);
    KeyCode::normalizeKey(key, flags, EventType::UP, KeyboardType::MACBOOK);
    KeyCode::reverseNormalizeKey(key, flags, EventType::UP, KeyboardType::MACBOOK);
    EXPECT_EQ(key, KeyCode::CURSOR_RIGHT);
    EXPECT_EQ(flags, ModifierFlag::FN | ModifierFlag::CURSOR);
  }
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

TEST(Buttons, justPressed) {
  Buttons previous(PointingButton::LEFT | PointingButton::MIDDLE | PointingButton::BUTTON4);
  Buttons buttons(PointingButton::RIGHT | PointingButton::MIDDLE | PointingButton::BUTTON5);

  EXPECT_EQ(Buttons(PointingButton::RIGHT | PointingButton::BUTTON5), buttons.justPressed(previous));
  EXPECT_EQ(Buttons(PointingButton::LEFT | PointingButton::BUTTON4), buttons.justReleased(previous));
}

TEST(Buttons, count) {
  Buttons buttons0(0);
  EXPECT_EQ(static_cast<unsigned int>(0), buttons0.count());

  Buttons buttons1(PointingButton::BUTTON4);
  EXPECT_EQ(static_cast<unsigned int>(1), buttons1.count());

  Buttons buttons3(PointingButton::RIGHT | PointingButton::MIDDLE | PointingButton::BUTTON5);
  EXPECT_EQ(static_cast<unsigned int>(3), buttons3.count());
}
