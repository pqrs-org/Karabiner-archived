#define CATCH_CONFIG_MAIN
#include "../../include/catch.hpp"

#include <ostream>
#include <vector>

#include "Config.hpp"
#include "KeyCode.hpp"
#include "KeyCodeModifierFlagPairs.hpp"

using namespace org_pqrs_Karabiner;
Config config;

Flags operator|(ModifierFlag lhs, ModifierFlag rhs) { return Flags(lhs.getRawBits() | rhs.getRawBits()); }

TEST_CASE("setUp", "[Generic]") {
  KeyCodeModifierFlagPairs::clearVirtualModifiers();
}

TEST_CASE("sizeof_", "[Generic]") {
  REQUIRE(sizeof(EventType) == sizeof(unsigned int));
  REQUIRE(sizeof(KeyboardType) == sizeof(unsigned int));
  REQUIRE(sizeof(ModifierFlag) == sizeof(unsigned int));
  REQUIRE(sizeof(Flags) == sizeof(unsigned int));
  REQUIRE(sizeof(KeyCode) == sizeof(unsigned int));
  REQUIRE(sizeof(ConsumerKeyCode) == sizeof(unsigned int));
  REQUIRE(sizeof(PointingButton) == sizeof(unsigned int));
  REQUIRE(sizeof(Buttons) == sizeof(unsigned int));
  REQUIRE(sizeof(ScrollWheel) == sizeof(unsigned int));
  REQUIRE(sizeof(PointingRelative) == sizeof(unsigned int));
  REQUIRE(sizeof(Option) == sizeof(unsigned int));
  REQUIRE(sizeof(ApplicationType) == sizeof(unsigned int));
  REQUIRE(sizeof(InputSource) == sizeof(unsigned int));
  REQUIRE(sizeof(DeviceVendor) == sizeof(unsigned int));
  REQUIRE(sizeof(DeviceProduct) == sizeof(unsigned int));
}

TEST_CASE("isKeyDownOrModifierDown", "[EventType]") {
  REQUIRE(EventType::DOWN.isKeyDownOrModifierDown(KeyCode::A, Flags(0)) == true);
  REQUIRE(EventType::UP.isKeyDownOrModifierDown(KeyCode::A, Flags(0)) == false);

  REQUIRE(EventType::MODIFY.isKeyDownOrModifierDown(KeyCode::SHIFT_L, ModifierFlag::SHIFT_L | ModifierFlag::CONTROL_L) == true);
  REQUIRE(EventType::MODIFY.isKeyDownOrModifierDown(KeyCode::SHIFT_L, Flags(ModifierFlag::CONTROL_L)) == false);
}

TEST_CASE("Flags.add", "[Flags]") {
  Flags mask = ModifierFlag::SHIFT_L | ModifierFlag::CONTROL_R | ModifierFlag::COMMAND_R;
  Flags flags = mask;
  REQUIRE(flags.stripFN() == mask);

  flags.add(ModifierFlag::OPTION_L);
  REQUIRE(flags == (mask | ModifierFlag::OPTION_L));

  flags.add(ModifierFlag::NONE);
  REQUIRE(flags == (mask | ModifierFlag::OPTION_L | ModifierFlag::NONE));
}

TEST_CASE("Flags.remove", "[Flags]") {
  {
    Flags flags = ModifierFlag::SHIFT_L | ModifierFlag::CONTROL_R | ModifierFlag::COMMAND_R;
    Flags removed = ModifierFlag::CONTROL_R | ModifierFlag::COMMAND_R;
    REQUIRE(flags.remove(ModifierFlag::SHIFT_L) == removed);
  }
  {
    Flags flags = ModifierFlag::SHIFT_L | ModifierFlag::SHIFT_R | ModifierFlag::NUMPAD | ModifierFlag::NONE;
    Flags removed = ModifierFlag::SHIFT_R | ModifierFlag::NUMPAD | ModifierFlag::NONE;
    REQUIRE(flags.remove(ModifierFlag::SHIFT_L) == removed);

    flags.remove(ModifierFlag::NUMPAD);
    removed = ModifierFlag::SHIFT_R | ModifierFlag::NONE;
    REQUIRE(flags == removed);

    flags.remove(ModifierFlag::SHIFT_R);
    flags.remove(ModifierFlag::NONE);
    removed = Flags(0);
    REQUIRE(flags == removed);

    flags.remove(ModifierFlag::OPTION_L);
    flags.remove(ModifierFlag::COMMAND_R);
    removed = Flags(0);
    REQUIRE(flags == removed);
  }
  {
    // chain
    Flags flags = ModifierFlag::SHIFT_L | ModifierFlag::SHIFT_R | ModifierFlag::CONTROL_R | ModifierFlag::COMMAND_R;
    Flags removed = ModifierFlag::CONTROL_R | ModifierFlag::COMMAND_R;
    REQUIRE(flags.remove(ModifierFlag::SHIFT_L).remove(ModifierFlag::SHIFT_R) == removed);
  }
}

TEST_CASE("stripFN", "[Flags]") {
  Flags mask = ModifierFlag::SHIFT_L | ModifierFlag::CONTROL_R | ModifierFlag::COMMAND_R;
  Flags flags = mask;
  REQUIRE(flags.stripFN() == mask);

  flags.add(ModifierFlag::FN);
  REQUIRE(flags.stripFN() == mask);
}

TEST_CASE("stripNUMPAD", "[Flags]") {
  Flags mask = ModifierFlag::SHIFT_L | ModifierFlag::CONTROL_R | ModifierFlag::COMMAND_R;
  Flags flags = mask;
  REQUIRE(flags.stripNUMPAD() == mask);

  flags.add(ModifierFlag::NUMPAD);
  REQUIRE(flags.stripNUMPAD() == mask);
}

TEST_CASE("Flags.isOn", "[Flags]") {
  Flags mask = ModifierFlag::SHIFT_L | ModifierFlag::CONTROL_R | ModifierFlag::COMMAND_R;
  Flags flags = mask;

  REQUIRE(flags.isOn(ModifierFlag::SHIFT_L) == true);
  REQUIRE(flags.isOn(ModifierFlag::SHIFT_R) == false);

  flags = Flags();
  REQUIRE(flags.isOn(ModifierFlag::ZERO) == true);

  flags = Flags();
  REQUIRE(flags.isOn(ModifierFlag::NONE) == false);
}

namespace {
KeyCode cursors[][2] = {
    {KeyCode::PAGEUP, KeyCode::CURSOR_UP},
    {KeyCode::PAGEDOWN, KeyCode::CURSOR_DOWN},
    {KeyCode::HOME, KeyCode::CURSOR_LEFT},
    {KeyCode::END, KeyCode::CURSOR_RIGHT},
};
}

TEST_CASE("op", "[KeyCode]") {
  REQUIRE(KeyCode::A == KeyCode::A);
  REQUIRE(KeyCode::VK_NONE > KeyCode::A);
}

TEST_CASE("get", "[KeyCode]") {
  REQUIRE(KeyCode::CAPSLOCK.get() == static_cast<unsigned int>(57));
}

struct NormalizeItem {
  NormalizeItem(KeyCode fk, Flags ff, KeyCode tk, Flags tf, KeyboardType kt) : fromKeyCode(fk), fromFlags(ff), toKeyCode(tk), toFlags(tf), keyboardType(kt) {}

  KeyCode fromKeyCode;
  Flags fromFlags;
  KeyCode toKeyCode;
  Flags toFlags;
  KeyboardType keyboardType;
};

TEST_CASE("normalizeKey", "[KeyCode]") {
  std::vector<NormalizeItem> vec;

  // ----------------------------------------
  // normal key (without FN)
  vec.push_back(NormalizeItem(KeyCode::A, Flags(ModifierFlag::SHIFT_L),
                              KeyCode::A, Flags(ModifierFlag::SHIFT_L), KeyboardType::MACBOOK));

  // PAGEUP (without FN) -> Don't change key.
  for (size_t i = 0; i < sizeof(cursors) / sizeof(cursors[0]); ++i) {
    vec.push_back(NormalizeItem(cursors[i][0], Flags(ModifierFlag::SHIFT_L),
                                cursors[i][0], Flags(ModifierFlag::SHIFT_L), KeyboardType::MACBOOK));
  }

  // ENTER (without FN)
  vec.push_back(NormalizeItem(KeyCode::ENTER, Flags(ModifierFlag::SHIFT_L),
                              KeyCode::ENTER, Flags(ModifierFlag::SHIFT_L), KeyboardType::MACBOOK));

  // FORWARD_DELETE (without FN)
  vec.push_back(NormalizeItem(KeyCode::FORWARD_DELETE, Flags(ModifierFlag::SHIFT_L),
                              KeyCode::FORWARD_DELETE, Flags(ModifierFlag::SHIFT_L), KeyboardType::MACBOOK));

  // ----------------------------------------
  // normal key(+FN)
  vec.push_back(NormalizeItem(KeyCode::A, ModifierFlag::SHIFT_L | ModifierFlag::FN,
                              KeyCode::A, ModifierFlag::SHIFT_L | ModifierFlag::FN, KeyboardType::MACBOOK));

  vec.push_back(NormalizeItem(KeyCode::K, ModifierFlag::SHIFT_L | ModifierFlag::FN,
                              KeyCode::K, ModifierFlag::SHIFT_L | ModifierFlag::FN, KeyboardType::MACBOOK));

  // PAGEUP(+FN) -> Arrow+FN
  for (size_t i = 0; i < sizeof(cursors) / sizeof(cursors[0]); ++i) {
    vec.push_back(NormalizeItem(cursors[i][0], ModifierFlag::SHIFT_L | ModifierFlag::FN,
                                cursors[i][1], ModifierFlag::SHIFT_L | ModifierFlag::FN, KeyboardType::MACBOOK));
  }

  // ENTER(+FN) -> RETURN+FN
  vec.push_back(NormalizeItem(KeyCode::ENTER, ModifierFlag::SHIFT_L | ModifierFlag::FN,
                              KeyCode::RETURN, ModifierFlag::SHIFT_L | ModifierFlag::FN, KeyboardType::MACBOOK));

  // FORWARD_DELETE(+FN) -> DELETE+FN
  vec.push_back(NormalizeItem(KeyCode::FORWARD_DELETE, ModifierFlag::SHIFT_L | ModifierFlag::FN,
                              KeyCode::DELETE, ModifierFlag::SHIFT_L | ModifierFlag::FN, KeyboardType::MACBOOK));

  // ----------------------------------------
  for (std::vector<NormalizeItem>::iterator it = vec.begin(); it != vec.end(); ++it) {
    KeyCode key;

    // Down
    key = it->fromKeyCode;
    KeyCode::normalizeKey(key, it->fromFlags, EventType::DOWN, it->keyboardType);
    REQUIRE(it->toKeyCode == key);
    REQUIRE(it->fromFlags == it->toFlags);

    // Up
    key = it->fromKeyCode;
    KeyCode::normalizeKey(key, it->fromFlags, EventType::UP, it->keyboardType);
    REQUIRE(it->toKeyCode == key);
    REQUIRE(it->fromFlags == it->toFlags);
  }

  // ======================================================================
  // Test case for the following key sequence.
  //  (1) FN Down
  //  (2) Right-Arrow Down
  //  (3) FN Up
  //  (4) Right-Arrow Up
  // Karabiner KeyboardEventCallback [ caught]: eventType 12, flags 0x800000, key 63, kbdType 37
  // Karabiner KeyboardEventCallback [ caught]: eventType 10, flags 0x800000, key 119, kbdType 37
  // Karabiner KeyboardEventCallback [ caught]: eventType 12, flags 0x0, key 63, kbdType 37
  // Karabiner KeyboardEventCallback [ caught]: eventType 11, flags 0x0, key 119, kbdType 37
  {
    KeyCode key = KeyCode::END;
    Flags flags = Flags(ModifierFlag::FN);
    KeyCode::normalizeKey(key, flags, EventType::DOWN, KeyboardType::MACBOOK);
    REQUIRE(key == KeyCode::CURSOR_RIGHT);
    REQUIRE(flags == Flags(ModifierFlag::FN));

    key = KeyCode::END;
    flags = Flags(0);
    KeyCode::normalizeKey(key, flags, EventType::UP, KeyboardType::MACBOOK);
    REQUIRE(key == KeyCode::CURSOR_RIGHT);
    REQUIRE(flags == Flags(0));
  }

  // Test case for the following key sequence.
  //  (1) Right-Arrow Down
  //  (2) FN Down
  //  (3) Right-Arrow UP
  //  (4) FN Up
  // Karabiner KeyboardEventCallback [ caught]: eventType 10, flags 0x200000, key 124, kbdType 37
  // Karabiner KeyboardEventCallback [ caught]: eventType 12, flags 0xa00000, key 63, kbdType 37
  // Karabiner KeyboardEventCallback [ caught]: eventType 11, flags 0xa00000, key 124, kbdType 37
  // Karabiner KeyboardEventCallback [ caught]: eventType 12, flags 0x0, key 63, kbdType 37
  {
    KeyCode key = KeyCode::CURSOR_RIGHT;
    Flags flags = Flags(ModifierFlag::NUMPAD);
    KeyCode::normalizeKey(key, flags, EventType::DOWN, KeyboardType::MACBOOK);
    REQUIRE(key == KeyCode::CURSOR_RIGHT);
    REQUIRE(flags == Flags(0));

    key = KeyCode::CURSOR_RIGHT;
    flags = Flags(ModifierFlag::FN | ModifierFlag::NUMPAD);
    KeyCode::normalizeKey(key, flags, EventType::UP, KeyboardType::MACBOOK);
    REQUIRE(key == KeyCode::CURSOR_RIGHT);
    REQUIRE(flags == Flags(ModifierFlag::FN));
  }
}

TEST_CASE("reverseNormalizeKey", "[KeyCode]") {
  std::vector<NormalizeItem> vec;

  // ENTER_POWERBOOK
  vec.push_back(NormalizeItem(KeyCode::ENTER_POWERBOOK, Flags(ModifierFlag::SHIFT_L),
                              KeyCode::ENTER_POWERBOOK, Flags(ModifierFlag::SHIFT_L), KeyboardType::POWERBOOK));

  // ENTER_POWERBOOK(+FN)
  vec.push_back(NormalizeItem(KeyCode::ENTER_POWERBOOK, ModifierFlag::SHIFT_L | ModifierFlag::FN,
                              KeyCode::ENTER_POWERBOOK, ModifierFlag::SHIFT_L | ModifierFlag::FN, KeyboardType::POWERBOOK));

  // ----------------------------------------
  // normal key (without FN)
  vec.push_back(NormalizeItem(KeyCode::A, Flags(ModifierFlag::SHIFT_L),
                              KeyCode::A, Flags(ModifierFlag::SHIFT_L), KeyboardType::MACBOOK));

  // PAGEUP (without FN)
  for (size_t i = 0; i < sizeof(cursors) / sizeof(cursors[0]); ++i) {
    vec.push_back(NormalizeItem(cursors[i][0], Flags(ModifierFlag::SHIFT_L),
                                cursors[i][0], Flags(ModifierFlag::SHIFT_L), KeyboardType::MACBOOK));
  }

  // ENTER (without FN)
  vec.push_back(NormalizeItem(KeyCode::ENTER, Flags(ModifierFlag::SHIFT_L),
                              KeyCode::ENTER, Flags(ModifierFlag::SHIFT_L), KeyboardType::MACBOOK));

  // FORWARD_DELETE (without FN)
  vec.push_back(NormalizeItem(KeyCode::FORWARD_DELETE, Flags(ModifierFlag::SHIFT_L),
                              KeyCode::FORWARD_DELETE, Flags(ModifierFlag::SHIFT_L), KeyboardType::MACBOOK));

  // CURSOR (without FN)
  for (size_t i = 0; i < sizeof(cursors) / sizeof(cursors[0]); ++i) {
    vec.push_back(NormalizeItem(cursors[i][1], Flags(ModifierFlag::NUMPAD),
                                cursors[i][1], Flags(ModifierFlag::NUMPAD), KeyboardType::MACBOOK));
  }

  // ----------------------------------------
  // normal key(+FN)
  vec.push_back(NormalizeItem(KeyCode::A, ModifierFlag::SHIFT_L | ModifierFlag::FN,
                              KeyCode::A, ModifierFlag::SHIFT_L | ModifierFlag::FN, KeyboardType::MACBOOK));

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
    REQUIRE(it->toKeyCode == key);
    REQUIRE(it->fromFlags == it->toFlags);

    // Up
    key = it->fromKeyCode;
    KeyCode::normalizeKey(key, it->fromFlags, EventType::UP, it->keyboardType);
    KeyCode::reverseNormalizeKey(key, it->fromFlags, EventType::UP, it->keyboardType);
    REQUIRE(it->toKeyCode == key);
    REQUIRE(it->fromFlags == it->toFlags);
  }

  // ======================================================================
  // Test case for the following key sequence.
  //  (1) FN Down
  //  (2) Right-Arrow Down
  //  (3) FN Up
  //  (4) Right-Arrow Up
  // Karabiner KeyboardEventCallback [ caught]: eventType 12, flags 0x800000, key 63, kbdType 37
  // Karabiner KeyboardEventCallback [ caught]: eventType 10, flags 0x800000, key 119, kbdType 37
  // Karabiner KeyboardEventCallback [ caught]: eventType 12, flags 0x0, key 63, kbdType 37
  // Karabiner KeyboardEventCallback [ caught]: eventType 11, flags 0x0, key 119, kbdType 37
  {
    KeyCode key(KeyCode::END);
    Flags flags(ModifierFlag::FN);
    KeyCode::normalizeKey(key, flags, EventType::DOWN, KeyboardType::MACBOOK);
    KeyCode::reverseNormalizeKey(key, flags, EventType::DOWN, KeyboardType::MACBOOK);
    REQUIRE(key == KeyCode::END);
    REQUIRE(flags == Flags(ModifierFlag::FN));

    key = KeyCode::END;
    flags = Flags(0);
    KeyCode::normalizeKey(key, flags, EventType::UP, KeyboardType::MACBOOK);
    KeyCode::reverseNormalizeKey(key, flags, EventType::UP, KeyboardType::MACBOOK);
    REQUIRE(key == KeyCode::END);
    REQUIRE(flags == Flags(0));
  }

  // Test case for the following key sequence.
  //  (1) Right-Arrow Down
  //  (2) FN Down
  //  (3) Right-Arrow UP
  //  (4) FN Up
  // Karabiner KeyboardEventCallback [ caught]: eventType 10, flags 0x200000, key 124, kbdType 37
  // Karabiner KeyboardEventCallback [ caught]: eventType 12, flags 0xa00000, key 63, kbdType 37
  // Karabiner KeyboardEventCallback [ caught]: eventType 11, flags 0xa00000, key 124, kbdType 37
  // Karabiner KeyboardEventCallback [ caught]: eventType 12, flags 0x0, key 63, kbdType 37
  {
    KeyCode key = KeyCode::CURSOR_RIGHT;
    Flags flags = Flags(ModifierFlag::NUMPAD);
    KeyCode::normalizeKey(key, flags, EventType::DOWN, KeyboardType::MACBOOK);
    KeyCode::reverseNormalizeKey(key, flags, EventType::DOWN, KeyboardType::MACBOOK);
    REQUIRE(key == KeyCode::CURSOR_RIGHT);
    REQUIRE(flags == Flags(ModifierFlag::NUMPAD));

    key = KeyCode::CURSOR_RIGHT;
    flags = Flags(ModifierFlag::FN | ModifierFlag::NUMPAD);
    KeyCode::normalizeKey(key, flags, EventType::UP, KeyboardType::MACBOOK);
    KeyCode::reverseNormalizeKey(key, flags, EventType::UP, KeyboardType::MACBOOK);
    REQUIRE(key == KeyCode::CURSOR_RIGHT);
    REQUIRE(flags == (ModifierFlag::FN | ModifierFlag::NUMPAD));
  }
}

TEST_CASE("getKeyCode", "[ModifierFlag]") {
  REQUIRE(ModifierFlag::CAPSLOCK.getKeyCode() == KeyCode::CAPSLOCK);
  REQUIRE(ModifierFlag::SHIFT_L.getKeyCode() == KeyCode::SHIFT_L);
  REQUIRE(ModifierFlag::SHIFT_R.getKeyCode() == KeyCode::SHIFT_R);
  REQUIRE(ModifierFlag::CONTROL_L.getKeyCode() == KeyCode::CONTROL_L);
  REQUIRE(ModifierFlag::CONTROL_R.getKeyCode() == KeyCode::CONTROL_R);
  REQUIRE(ModifierFlag::OPTION_L.getKeyCode() == KeyCode::OPTION_L);
  REQUIRE(ModifierFlag::OPTION_R.getKeyCode() == KeyCode::OPTION_R);
  REQUIRE(ModifierFlag::COMMAND_L.getKeyCode() == KeyCode::COMMAND_L);
  REQUIRE(ModifierFlag::COMMAND_R.getKeyCode() == KeyCode::COMMAND_R);
  REQUIRE(ModifierFlag::FN.getKeyCode() == KeyCode::FN);

  REQUIRE(ModifierFlag::NUMPAD.getKeyCode() == KeyCode::VK_NONE);
}

TEST_CASE("getRawBits", "[ModifierFlag]") {
  REQUIRE(ModifierFlag::CAPSLOCK.getRawBits() == 0x10000);
  REQUIRE(ModifierFlag::COMMAND_L.getRawBits() == 0x100008);
  REQUIRE(ModifierFlag::ZERO.getRawBits() == 0);
  REQUIRE(ModifierFlag::NONE.getRawBits() == 0);
}

TEST_CASE("getModifierFlag", "[KeyCode]") {
  REQUIRE(KeyCode::CAPSLOCK.getModifierFlag() == ModifierFlag::CAPSLOCK);
  REQUIRE(KeyCode::SHIFT_L.getModifierFlag() == ModifierFlag::SHIFT_L);
  REQUIRE(KeyCode::SHIFT_R.getModifierFlag() == ModifierFlag::SHIFT_R);
  REQUIRE(KeyCode::CONTROL_L.getModifierFlag() == ModifierFlag::CONTROL_L);
  REQUIRE(KeyCode::CONTROL_R.getModifierFlag() == ModifierFlag::CONTROL_R);
  REQUIRE(KeyCode::OPTION_L.getModifierFlag() == ModifierFlag::OPTION_L);
  REQUIRE(KeyCode::OPTION_R.getModifierFlag() == ModifierFlag::OPTION_R);
  REQUIRE(KeyCode::COMMAND_L.getModifierFlag() == ModifierFlag::COMMAND_L);
  REQUIRE(KeyCode::COMMAND_R.getModifierFlag() == ModifierFlag::COMMAND_R);
  REQUIRE(KeyCode::FN.getModifierFlag() == ModifierFlag::FN);

  REQUIRE(KeyCode(KeyCode::A).getModifierFlag() == ModifierFlag::ZERO);
  REQUIRE(KeyCode(KeyCode::VK_NONE).getModifierFlag() == ModifierFlag::ZERO);
}

TEST_CASE("isModifier", "[KeyCode]") {
  REQUIRE(KeyCode(KeyCode::CAPSLOCK).isModifier() == true);
  REQUIRE(KeyCode(KeyCode::SHIFT_L).isModifier() == true);
  REQUIRE(KeyCode(KeyCode::A).isModifier() == false);
  REQUIRE(KeyCode(KeyCode::VK_NONE).isModifier() == false);
}

TEST_CASE("isRepeatable", "[ConsumerKeyCode]") {
  REQUIRE(ConsumerKeyCode::BRIGHTNESS_DOWN.isRepeatable() == true);
  REQUIRE(ConsumerKeyCode::BRIGHTNESS_UP.isRepeatable() == true);

  REQUIRE(ConsumerKeyCode::KEYBOARDLIGHT_OFF.isRepeatable() == false);
  REQUIRE(ConsumerKeyCode::KEYBOARDLIGHT_LOW.isRepeatable() == true);
  REQUIRE(ConsumerKeyCode::KEYBOARDLIGHT_HIGH.isRepeatable() == true);

  REQUIRE(ConsumerKeyCode::MUSIC_PREV.isRepeatable() == true);
  REQUIRE(ConsumerKeyCode::MUSIC_PLAY.isRepeatable() == false);
  REQUIRE(ConsumerKeyCode::MUSIC_NEXT.isRepeatable() == true);

  REQUIRE(ConsumerKeyCode::VOLUME_MUTE.isRepeatable() == false);
  REQUIRE(ConsumerKeyCode::VOLUME_DOWN.isRepeatable() == true);
  REQUIRE(ConsumerKeyCode::VOLUME_UP.isRepeatable() == true);

  REQUIRE(ConsumerKeyCode::EJECT.isRepeatable() == false);
  REQUIRE(ConsumerKeyCode::POWER.isRepeatable() == false);
  REQUIRE(ConsumerKeyCode::NUMLOCK.isRepeatable() == false);
  REQUIRE(ConsumerKeyCode::VIDEO_MIRROR.isRepeatable() == false);
}

TEST_CASE("Buttons.add", "[Buttons]") {
  Buttons buttons;
  buttons.add(PointingButton::LEFT);
  REQUIRE(buttons == Buttons(PointingButton::LEFT));

  buttons.add(PointingButton::MIDDLE);
  REQUIRE(buttons == Buttons(PointingButton::LEFT | PointingButton::MIDDLE));

  // some PointingButton twice.
  buttons.add(PointingButton::LEFT);
  REQUIRE(buttons == Buttons(PointingButton::LEFT | PointingButton::MIDDLE));

  buttons = Buttons(0);
  buttons.add(PointingButton::LEFT | PointingButton::MIDDLE);
  REQUIRE(buttons == Buttons(PointingButton::LEFT | PointingButton::MIDDLE));
}

TEST_CASE("Buttons.remove", "[Buttons]") {
  Buttons buttons(PointingButton::LEFT | PointingButton::MIDDLE);
  buttons.remove(PointingButton::LEFT);
  REQUIRE(buttons == Buttons(PointingButton::MIDDLE));

  // unexist PointingButton.
  buttons.remove(PointingButton::RIGHT);
  REQUIRE(buttons == Buttons(PointingButton::MIDDLE));

  buttons = PointingButton::LEFT | PointingButton::MIDDLE | PointingButton::RIGHT;
  buttons.remove(PointingButton::LEFT | PointingButton::RIGHT);
  REQUIRE(buttons == Buttons(PointingButton::MIDDLE));
}

TEST_CASE("Buttons.isOn", "[Buttons]") {
  Buttons buttons(PointingButton::LEFT | PointingButton::MIDDLE | PointingButton::BUTTON4);
  REQUIRE(buttons.isOn(PointingButton::LEFT) == true);
  REQUIRE(buttons.isOn(PointingButton::LEFT | PointingButton::MIDDLE) == true);

  REQUIRE(buttons.isOn(PointingButton::RIGHT) == false);
  REQUIRE(buttons.isOn(PointingButton::LEFT | PointingButton::RIGHT) == false);
}

TEST_CASE("justPressed", "[Buttons]") {
  Buttons previous(PointingButton::LEFT | PointingButton::MIDDLE | PointingButton::BUTTON4);
  Buttons buttons(PointingButton::RIGHT | PointingButton::MIDDLE | PointingButton::BUTTON5);

  REQUIRE(buttons.justPressed(previous) == Buttons(PointingButton::RIGHT | PointingButton::BUTTON5));
  REQUIRE(buttons.justReleased(previous) == Buttons(PointingButton::LEFT | PointingButton::BUTTON4));
}

TEST_CASE("count", "[Buttons]") {
  Buttons buttons0(0);
  REQUIRE(buttons0.count() == static_cast<unsigned int>(0));

  Buttons buttons1(PointingButton::BUTTON4);
  REQUIRE(buttons1.count() == static_cast<unsigned int>(1));

  Buttons buttons3(PointingButton::RIGHT | PointingButton::MIDDLE | PointingButton::BUTTON5);
  REQUIRE(buttons3.count() == static_cast<unsigned int>(3));
}

TEST_CASE("getScrollWheelFromDelta", "[ScrollWheel]") {
  REQUIRE(ScrollWheel::getScrollWheelFromDelta(0, 0) == ScrollWheel::NONE);

  REQUIRE(ScrollWheel::getScrollWheelFromDelta(1, 0) == ScrollWheel::UP);
  REQUIRE(ScrollWheel::getScrollWheelFromDelta(-1, 0) == ScrollWheel::DOWN);
  REQUIRE(ScrollWheel::getScrollWheelFromDelta(0, 1) == ScrollWheel::LEFT);
  REQUIRE(ScrollWheel::getScrollWheelFromDelta(0, -1) == ScrollWheel::RIGHT);

  REQUIRE(ScrollWheel::getScrollWheelFromDelta(10, 3) == ScrollWheel::UP);
  REQUIRE(ScrollWheel::getScrollWheelFromDelta(-10, 3) == ScrollWheel::DOWN);
  REQUIRE(ScrollWheel::getScrollWheelFromDelta(3, 10) == ScrollWheel::LEFT);
  REQUIRE(ScrollWheel::getScrollWheelFromDelta(3, -10) == ScrollWheel::RIGHT);

  REQUIRE(ScrollWheel::getScrollWheelFromDelta(10, 10) == ScrollWheel::UP);
  REQUIRE(ScrollWheel::getScrollWheelFromDelta(-10, -10) == ScrollWheel::DOWN);
}

TEST_CASE("getPointingRelativeFromDelta", "[PointingRelative]") {
  REQUIRE(PointingRelative::getPointingRelativeFromDelta(0, 0) == PointingRelative::NONE);

  REQUIRE(PointingRelative::getPointingRelativeFromDelta(0, -10) == PointingRelative::UP);
  REQUIRE(PointingRelative::getPointingRelativeFromDelta(0, 10) == PointingRelative::DOWN);
  REQUIRE(PointingRelative::getPointingRelativeFromDelta(-10, 0) == PointingRelative::LEFT);
  REQUIRE(PointingRelative::getPointingRelativeFromDelta(10, 0) == PointingRelative::RIGHT);

  REQUIRE(PointingRelative::getPointingRelativeFromDelta(3, -10) == PointingRelative::UP);
  REQUIRE(PointingRelative::getPointingRelativeFromDelta(3, 10) == PointingRelative::DOWN);
  REQUIRE(PointingRelative::getPointingRelativeFromDelta(-10, 3) == PointingRelative::LEFT);
  REQUIRE(PointingRelative::getPointingRelativeFromDelta(10, 3) == PointingRelative::RIGHT);

  // diagonal direction
  REQUIRE(PointingRelative::getPointingRelativeFromDelta(10, -10) == PointingRelative::NONE);
  REQUIRE(PointingRelative::getPointingRelativeFromDelta(-10, 10) == PointingRelative::NONE);

  REQUIRE(PointingRelative::getPointingRelativeFromDelta(10, 8) == PointingRelative::NONE);
  REQUIRE(PointingRelative::getPointingRelativeFromDelta(8, 10) == PointingRelative::NONE);

  REQUIRE(PointingRelative::getPointingRelativeFromDelta(10, 6) == PointingRelative::NONE);
  REQUIRE(PointingRelative::getPointingRelativeFromDelta(6, 10) == PointingRelative::NONE);
}
