#include <ostream>
#include <gtest/gtest.h>
#include "Config.hpp"
#include "FlagStatus.hpp"
#include "KeyCode.hpp"
#include "KeyCodeModifierFlagPairs.hpp"

using namespace org_pqrs_KeyRemap4MacBook;
Config config;

std::ostream& operator<<(std::ostream& os, const EventType& v) { return os << v.get(); }
std::ostream& operator<<(std::ostream& os, const KeyboardType& v) { return os << v.get(); }
std::ostream& operator<<(std::ostream& os, const ModifierFlag& v) { return os << v.getRawBits(); }
std::ostream& operator<<(std::ostream& os, const Flags& v) { return os << v.get(); }
std::ostream& operator<<(std::ostream& os, const KeyCode& v) { return os << v.get(); }
std::ostream& operator<<(std::ostream& os, const ConsumerKeyCode& v) { return os << v.get(); }
std::ostream& operator<<(std::ostream& os, const PointingButton& v) { return os << v.get(); }
std::ostream& operator<<(std::ostream& os, const Buttons& v) { return os << v.get(); }

Flags operator|(ModifierFlag lhs, ModifierFlag rhs) { return Flags(lhs.getRawBits() | rhs.getRawBits()); }

TEST(Generic, setUp) {
  KeyCodeModifierFlagPairs::clearVirtualModifiers();
}

TEST(FlagStatus, makeFlags) {
  FlagStatus flagStatus;
  EXPECT_EQ(Flags(), flagStatus.makeFlags());

  flagStatus.set();
  EXPECT_EQ(Flags(), flagStatus.makeFlags());

  flagStatus.set(KeyCode::A, Flags(0));
  EXPECT_EQ(Flags(), flagStatus.makeFlags());

  // down SHIFT_L
  flagStatus.set(KeyCode::SHIFT_L, Flags(ModifierFlag::SHIFT_L));
  EXPECT_EQ(Flags(ModifierFlag::SHIFT_L), flagStatus.makeFlags());

  // no effect with ModifierFlag::NONE
  flagStatus.set(KeyCode::A, Flags(ModifierFlag::NONE));
  EXPECT_EQ(Flags(ModifierFlag::SHIFT_L), flagStatus.makeFlags());

  // down CONTROL_
  flagStatus.set(KeyCode::CONTROL_L, ModifierFlag::CONTROL_L | ModifierFlag::SHIFT_L);
  EXPECT_EQ(Flags(ModifierFlag::SHIFT_L | ModifierFlag::CONTROL_L), flagStatus.makeFlags());

  // down A
  flagStatus.set(KeyCode::A, ModifierFlag::CONTROL_L | ModifierFlag::SHIFT_L);
  EXPECT_EQ(Flags(ModifierFlag::SHIFT_L | ModifierFlag::CONTROL_L), flagStatus.makeFlags());

  // up SHIFT_L
  flagStatus.set(KeyCode::SHIFT_L, Flags(ModifierFlag::CONTROL_L));
  EXPECT_EQ(Flags(ModifierFlag::CONTROL_L), flagStatus.makeFlags());

  // up CONTROL_L
  flagStatus.set(KeyCode::CONTROL_L, Flags(0));
  EXPECT_EQ(Flags(), flagStatus.makeFlags());

  // All flags
  flagStatus.reset();
  flagStatus.set(KeyCode::CAPSLOCK, Flags(ModifierFlag::CAPSLOCK));
  EXPECT_EQ(Flags(ModifierFlag::CAPSLOCK), flagStatus.makeFlags());

  flagStatus.set(KeyCode::CAPSLOCK, Flags(0));
  EXPECT_EQ(Flags(0), flagStatus.makeFlags());

  flagStatus.reset();
  flagStatus.set(KeyCode::SHIFT_L, Flags(ModifierFlag::SHIFT_L));
  EXPECT_EQ(Flags(ModifierFlag::SHIFT_L), flagStatus.makeFlags());

  flagStatus.reset();
  flagStatus.set(KeyCode::SHIFT_R, Flags(ModifierFlag::SHIFT_R));
  EXPECT_EQ(Flags(ModifierFlag::SHIFT_R), flagStatus.makeFlags());

  flagStatus.reset();
  flagStatus.set(KeyCode::CONTROL_L, Flags(ModifierFlag::CONTROL_L));
  EXPECT_EQ(Flags(ModifierFlag::CONTROL_L), flagStatus.makeFlags());

  flagStatus.reset();
  flagStatus.set(KeyCode::CONTROL_R, Flags(ModifierFlag::CONTROL_R));
  EXPECT_EQ(Flags(ModifierFlag::CONTROL_R), flagStatus.makeFlags());

  flagStatus.reset();
  flagStatus.set(KeyCode::OPTION_L, Flags(ModifierFlag::OPTION_L));
  EXPECT_EQ(Flags(ModifierFlag::OPTION_L), flagStatus.makeFlags());

  flagStatus.reset();
  flagStatus.set(KeyCode::OPTION_R, Flags(ModifierFlag::OPTION_R));
  EXPECT_EQ(Flags(ModifierFlag::OPTION_R), flagStatus.makeFlags());

  flagStatus.reset();
  flagStatus.set(KeyCode::COMMAND_L, Flags(ModifierFlag::COMMAND_L));
  EXPECT_EQ(Flags(ModifierFlag::COMMAND_L), flagStatus.makeFlags());

  flagStatus.reset();
  flagStatus.set(KeyCode::COMMAND_R, Flags(ModifierFlag::COMMAND_R));
  EXPECT_EQ(Flags(ModifierFlag::COMMAND_R), flagStatus.makeFlags());

  flagStatus.reset();
  flagStatus.set(KeyCode::FN, Flags(ModifierFlag::FN));
  EXPECT_EQ(Flags(ModifierFlag::FN), flagStatus.makeFlags());

  // Virtual modifiers are ignored at makeFlags.
  flagStatus.reset();
  flagStatus.increase(ModifierFlag::EXTRA1);
  EXPECT_EQ(Flags(), flagStatus.makeFlags());
}

TEST(FlagStatus, getFlag) {
  FlagStatus flagStatus;

  EXPECT_EQ(ModifierFlag::CAPSLOCK, flagStatus.getFlag(0));
}

TEST(FlagStatus, increase) {
  {
    FlagStatus flagStatus;

    // Do nothing with ModifierFlag::NONE.
    flagStatus.increase(ModifierFlag::NONE);
    EXPECT_EQ(Flags(0), flagStatus.makeFlags());

    flagStatus.increase(ModifierFlag::SHIFT_L);
    EXPECT_EQ(Flags(ModifierFlag::SHIFT_L), flagStatus.makeFlags());

    {
      Vector_ModifierFlag v;
      v.push_back(ModifierFlag::COMMAND_L);
      v.push_back(ModifierFlag::CONTROL_L);
      flagStatus.increase(v);
      EXPECT_EQ(Flags(ModifierFlag::COMMAND_L | ModifierFlag::CONTROL_L | ModifierFlag::SHIFT_L), flagStatus.makeFlags());
    }

    flagStatus.increase(ModifierFlag::NONE);
    EXPECT_EQ(Flags(ModifierFlag::COMMAND_L | ModifierFlag::CONTROL_L | ModifierFlag::SHIFT_L), flagStatus.makeFlags());
  }

  {
    FlagStatus flagStatus;
    Vector_ModifierFlag v;
    v.push_back(ModifierFlag::COMMAND_L);
    v.push_back(ModifierFlag::CONTROL_L);
    flagStatus.increase(ModifierFlag::SHIFT_L, v);
    EXPECT_EQ(Flags(ModifierFlag::COMMAND_L | ModifierFlag::CONTROL_L | ModifierFlag::SHIFT_L), flagStatus.makeFlags());
  }
  {
    FlagStatus flagStatus;
    Vector_ModifierFlag v;
    v.push_back(ModifierFlag::COMMAND_L);
    v.push_back(ModifierFlag::CONTROL_L);
    flagStatus.increase(ModifierFlag::COMMAND_L, v);
    EXPECT_EQ(Flags(ModifierFlag::COMMAND_L | ModifierFlag::CONTROL_L), flagStatus.makeFlags());
    flagStatus.decrease(v);
    EXPECT_EQ(Flags(0), flagStatus.makeFlags());
  }
}

TEST(FlagStatus, decrease) {
  FlagStatus flagStatus;

  {
    Vector_ModifierFlag v;
    v.push_back(ModifierFlag::COMMAND_L);
    v.push_back(ModifierFlag::CONTROL_L);
    flagStatus.increase(v);
    EXPECT_EQ(Flags(ModifierFlag::COMMAND_L | ModifierFlag::CONTROL_L), flagStatus.makeFlags());
  }

  flagStatus.decrease(ModifierFlag::CONTROL_L);
  EXPECT_EQ(Flags(ModifierFlag::COMMAND_L), flagStatus.makeFlags());
}

TEST(FlagStatus, temporary_increase) {
  FlagStatus flagStatus;

  // Do nothing with ModifierFlag::NONE.
  flagStatus.temporary_increase(ModifierFlag::NONE);
  EXPECT_EQ(Flags(0), flagStatus.makeFlags());

  {
    Vector_ModifierFlag v;
    v.push_back(ModifierFlag::COMMAND_L);
    v.push_back(ModifierFlag::CONTROL_L);
    flagStatus.increase(v);
    EXPECT_EQ(Flags(ModifierFlag::COMMAND_L | ModifierFlag::CONTROL_L), flagStatus.makeFlags());
  }

  flagStatus.temporary_increase(ModifierFlag::OPTION_L);
  EXPECT_EQ(Flags(ModifierFlag::COMMAND_L | ModifierFlag::CONTROL_L | ModifierFlag::OPTION_L), flagStatus.makeFlags());

  // temporary_increase will reset by flagStatus.set
  flagStatus.set(KeyCode::COMMAND_L, Flags(ModifierFlag::COMMAND_L));
  EXPECT_EQ(Flags(ModifierFlag::COMMAND_L | ModifierFlag::CONTROL_L), flagStatus.makeFlags());
}

TEST(FlagStatus, temporary_decrease) {
  FlagStatus flagStatus;

  {
    Vector_ModifierFlag v;
    v.push_back(ModifierFlag::COMMAND_L);
    v.push_back(ModifierFlag::CONTROL_L);
    flagStatus.increase(v);
    EXPECT_EQ(Flags(ModifierFlag::COMMAND_L | ModifierFlag::CONTROL_L), flagStatus.makeFlags());
  }

  flagStatus.temporary_decrease(ModifierFlag::CONTROL_L);
  EXPECT_EQ(Flags(ModifierFlag::COMMAND_L), flagStatus.makeFlags());

  // temporary_increase will reset by flagStatus.set
  flagStatus.set(KeyCode::COMMAND_L, Flags(ModifierFlag::COMMAND_L));
  EXPECT_EQ(Flags(ModifierFlag::COMMAND_L | ModifierFlag::CONTROL_L), flagStatus.makeFlags());
}

TEST(FlagStatus, lock_increase) {
  FlagStatus flagStatus;

  // Do nothing with ModifierFlag::NONE.
  flagStatus.lock_increase(ModifierFlag::NONE);
  EXPECT_EQ(Flags(0), flagStatus.makeFlags());

  flagStatus.lock_increase(ModifierFlag::COMMAND_L);
  EXPECT_EQ(Flags(ModifierFlag::COMMAND_L), flagStatus.makeFlags());

  // lock don't cancel by reset & set.
  flagStatus.reset();
  flagStatus.set(KeyCode::A, Flags(0));
  EXPECT_EQ(Flags(ModifierFlag::COMMAND_L), flagStatus.makeFlags());

  flagStatus.lock_decrease(ModifierFlag::COMMAND_L);
  EXPECT_EQ(Flags(), flagStatus.makeFlags());
}

TEST(FlagStatus, lock_toggle) {
  FlagStatus flagStatus;

  flagStatus.lock_increase(ModifierFlag::COMMAND_L);
  EXPECT_EQ(Flags(ModifierFlag::COMMAND_L), flagStatus.makeFlags());

  flagStatus.lock_toggle(ModifierFlag::COMMAND_L);
  EXPECT_EQ(Flags(0), flagStatus.makeFlags());

  flagStatus.lock_toggle(ModifierFlag::COMMAND_L);
  EXPECT_EQ(Flags(ModifierFlag::COMMAND_L), flagStatus.makeFlags());
}

TEST(FlagStatus, lock_clear) {
  FlagStatus flagStatus;

  {
    Vector_ModifierFlag v;
    v.push_back(ModifierFlag::COMMAND_L);
    v.push_back(ModifierFlag::FN);
    v.push_back(ModifierFlag::SHIFT_L);
    flagStatus.lock_increase(v);
    EXPECT_EQ(ModifierFlag::COMMAND_L | ModifierFlag::FN | ModifierFlag::SHIFT_L, flagStatus.makeFlags());
  }
  {
    flagStatus.increase(ModifierFlag::CAPSLOCK);
    EXPECT_EQ(ModifierFlag::CAPSLOCK | ModifierFlag::COMMAND_L | ModifierFlag::FN | ModifierFlag::SHIFT_L,
              flagStatus.makeFlags());
  }

  flagStatus.lock_clear();
  EXPECT_EQ(Flags(0), flagStatus.makeFlags());
}

TEST(FlagStatus, sticky_increase) {
  FlagStatus flagStatus;

  // Do nothing with ModifierFlag::NONE.
  flagStatus.sticky_increase(ModifierFlag::NONE);
  EXPECT_EQ(Flags(0), flagStatus.makeFlags());

  {
    Vector_ModifierFlag v;
    v.push_back(ModifierFlag::COMMAND_L);
    v.push_back(ModifierFlag::FN);
    flagStatus.sticky_increase(v);
    EXPECT_EQ(Flags(ModifierFlag::COMMAND_L | ModifierFlag::FN), flagStatus.makeFlags());
  }

  flagStatus.sticky_decrease(ModifierFlag::COMMAND_L);
  EXPECT_EQ(Flags(ModifierFlag::FN), flagStatus.makeFlags());
}

TEST(FlagStatus, sticky_toggle) {
  FlagStatus flagStatus;

  flagStatus.sticky_increase(ModifierFlag::COMMAND_L);
  EXPECT_EQ(Flags(ModifierFlag::COMMAND_L), flagStatus.makeFlags());

  flagStatus.sticky_toggle(ModifierFlag::COMMAND_L);
  EXPECT_EQ(Flags(0), flagStatus.makeFlags());

  flagStatus.sticky_toggle(ModifierFlag::COMMAND_L);
  EXPECT_EQ(Flags(ModifierFlag::COMMAND_L), flagStatus.makeFlags());
}

TEST(FlagStatus, sticky_clear) {
  FlagStatus flagStatus;

  {
    Vector_ModifierFlag v;
    v.push_back(ModifierFlag::COMMAND_L);
    v.push_back(ModifierFlag::FN);
    v.push_back(ModifierFlag::SHIFT_L);
    flagStatus.sticky_increase(v);
    EXPECT_EQ(ModifierFlag::COMMAND_L | ModifierFlag::FN | ModifierFlag::SHIFT_L, flagStatus.makeFlags());
  }

  flagStatus.sticky_clear();
  EXPECT_EQ(Flags(0), flagStatus.makeFlags());
}

TEST(FlagStatus, CapsLock) {
  FlagStatus flagStatus;

  flagStatus.set(KeyCode::CAPSLOCK, Flags(ModifierFlag::CAPSLOCK));
  EXPECT_EQ(Flags(ModifierFlag::CAPSLOCK), flagStatus.makeFlags());

  flagStatus.reset();

  flagStatus.set(KeyCode::A, Flags(ModifierFlag::CAPSLOCK));
  EXPECT_EQ(Flags(ModifierFlag::CAPSLOCK), flagStatus.makeFlags());

  // from other keyboard
  flagStatus.set(KeyCode::A, Flags(0));
  EXPECT_EQ(Flags(ModifierFlag::CAPSLOCK), flagStatus.makeFlags());

  flagStatus.set(KeyCode::A, Flags(ModifierFlag::CAPSLOCK));
  EXPECT_EQ(Flags(ModifierFlag::CAPSLOCK), flagStatus.makeFlags());

  // reset
  flagStatus.set(KeyCode::CAPSLOCK, Flags(0));
  EXPECT_EQ(Flags(), flagStatus.makeFlags());

  // soft caps
  flagStatus.lock_increase(ModifierFlag::CAPSLOCK);
  flagStatus.set(KeyCode::A, Flags(0));
  EXPECT_EQ(Flags(ModifierFlag::CAPSLOCK), flagStatus.makeFlags());

  // soft caps will be canceled by hardware caps
  flagStatus.set(KeyCode::CAPSLOCK, Flags(0));
  EXPECT_EQ(Flags(0), flagStatus.makeFlags());
}

TEST(FlagStatus, ScopedTemporaryFlagsChanger) {
  {
    FlagStatus flagStatus;

    flagStatus.increase(ModifierFlag::SHIFT_L);
    flagStatus.increase(ModifierFlag::SHIFT_L);
    flagStatus.increase(ModifierFlag::SHIFT_L);
    flagStatus.increase(ModifierFlag::SHIFT_L);
    flagStatus.increase(ModifierFlag::SHIFT_R);
    flagStatus.temporary_increase(ModifierFlag::CONTROL_L);
    flagStatus.lock_increase(ModifierFlag::COMMAND_R);
    flagStatus.sticky_increase(ModifierFlag::OPTION_R);

    EXPECT_EQ(Flags(ModifierFlag::SHIFT_L | ModifierFlag::SHIFT_R | ModifierFlag::CONTROL_L | ModifierFlag::COMMAND_R | ModifierFlag::OPTION_R), flagStatus.makeFlags());

    {
      FlagStatus::ScopedTemporaryFlagsChanger stfc(flagStatus,
                                                   ModifierFlag::FN | ModifierFlag::OPTION_L | ModifierFlag::SHIFT_R);

      EXPECT_EQ(Flags(ModifierFlag::FN | ModifierFlag::OPTION_L | ModifierFlag::SHIFT_R), flagStatus.makeFlags());
    }

    EXPECT_EQ(Flags(ModifierFlag::SHIFT_L | ModifierFlag::SHIFT_R | ModifierFlag::CONTROL_L | ModifierFlag::COMMAND_R | ModifierFlag::OPTION_R), flagStatus.makeFlags());

    flagStatus.decrease(ModifierFlag::SHIFT_L);
    flagStatus.decrease(ModifierFlag::SHIFT_L);
    flagStatus.decrease(ModifierFlag::SHIFT_L);

    EXPECT_EQ(Flags(ModifierFlag::SHIFT_L | ModifierFlag::SHIFT_R | ModifierFlag::CONTROL_L | ModifierFlag::COMMAND_R | ModifierFlag::OPTION_R), flagStatus.makeFlags());

    flagStatus.decrease(ModifierFlag::SHIFT_L);

    EXPECT_EQ(Flags(ModifierFlag::SHIFT_R | ModifierFlag::CONTROL_L | ModifierFlag::COMMAND_R | ModifierFlag::OPTION_R), flagStatus.makeFlags());
  }

  // ------------------------------------------------------------
  {
    FlagStatus flagStatus;

    flagStatus.decrease(ModifierFlag::SHIFT_L);
    flagStatus.decrease(ModifierFlag::SHIFT_L);

    {
      Flags flags(ModifierFlag::SHIFT_R);
      FlagStatus::ScopedTemporaryFlagsChanger stfc(flagStatus, flags);
      EXPECT_EQ(Flags(ModifierFlag::SHIFT_R), flagStatus.makeFlags());
    }

    flagStatus.increase(ModifierFlag::SHIFT_L);
    flagStatus.increase(ModifierFlag::SHIFT_L);

    EXPECT_EQ(Flags(0), flagStatus.makeFlags());
  }
}

TEST(FlagStatus, isOn) {
  {
    FlagStatus flagStatus;

    {
      Vector_ModifierFlag modifierFlags;
      EXPECT_TRUE(flagStatus.isOn(modifierFlags));
    }
    {
      Vector_ModifierFlag modifierFlags;
      modifierFlags.push_back(ModifierFlag(ModifierFlag::ZERO));
      EXPECT_TRUE(flagStatus.isOn(modifierFlags));
    }
    {
      Vector_ModifierFlag modifierFlags;
      modifierFlags.push_back(ModifierFlag::NONE);
      EXPECT_TRUE(flagStatus.isOn(modifierFlags));
    }
    {
      Vector_ModifierFlag modifierFlags;
      modifierFlags.push_back(ModifierFlag::NONE);
      modifierFlags.push_back(ModifierFlag::ZERO);
      EXPECT_TRUE(flagStatus.isOn(modifierFlags));
    }
  }

  {
    FlagStatus flagStatus;
    flagStatus.increase(ModifierFlag::SHIFT_L);

    {
      Vector_ModifierFlag modifierFlags;
      modifierFlags.push_back(ModifierFlag::SHIFT_L);
      EXPECT_TRUE(flagStatus.isOn(modifierFlags));
    }
    {
      Vector_ModifierFlag modifierFlags;
      modifierFlags.push_back(ModifierFlag::SHIFT_L);
      modifierFlags.push_back(ModifierFlag::NONE);
      EXPECT_TRUE(flagStatus.isOn(modifierFlags));
    }
    {
      Vector_ModifierFlag modifierFlags;
      modifierFlags.push_back(ModifierFlag::SHIFT_R);
      EXPECT_FALSE(flagStatus.isOn(modifierFlags));
    }
    {
      Vector_ModifierFlag modifierFlags;
      modifierFlags.push_back(ModifierFlag::SHIFT_L);
      modifierFlags.push_back(ModifierFlag::ZERO);
      EXPECT_TRUE(flagStatus.isOn(modifierFlags));
    }
  }

  {
    FlagStatus flagStatus;
    flagStatus.increase(ModifierFlag::SHIFT_L);
    flagStatus.increase(ModifierFlag::ZERO);

    {
      Vector_ModifierFlag modifierFlags;
      modifierFlags.push_back(ModifierFlag::SHIFT_L);
      EXPECT_TRUE(flagStatus.isOn(modifierFlags));
    }
    {
      Vector_ModifierFlag modifierFlags;
      modifierFlags.push_back(ModifierFlag::SHIFT_L);
      modifierFlags.push_back(ModifierFlag::NONE);
      EXPECT_TRUE(flagStatus.isOn(modifierFlags));
    }
    {
      Vector_ModifierFlag modifierFlags;
      modifierFlags.push_back(ModifierFlag::SHIFT_R);
      EXPECT_FALSE(flagStatus.isOn(modifierFlags));
    }
    {
      Vector_ModifierFlag modifierFlags;
      modifierFlags.push_back(ModifierFlag::SHIFT_L);
      modifierFlags.push_back(ModifierFlag::ZERO);
      EXPECT_TRUE(flagStatus.isOn(modifierFlags));
    }
    {
      Vector_ModifierFlag modifierFlags;
      modifierFlags.push_back(ModifierFlag::SHIFT_L);
      modifierFlags.push_back(ModifierFlag::ZERO);
      modifierFlags.push_back(ModifierFlag::NONE);
      EXPECT_TRUE(flagStatus.isOn(modifierFlags));
    }
  }

  {
    FlagStatus flagStatus;
    flagStatus.increase(ModifierFlag::SHIFT_L);
    flagStatus.increase(ModifierFlag::CONTROL_R);
    flagStatus.increase(ModifierFlag::COMMAND_R);

    {
      Vector_ModifierFlag modifierFlags;
      modifierFlags.push_back(ModifierFlag::SHIFT_L);
      EXPECT_TRUE(flagStatus.isOn(modifierFlags));
    }
    {
      Vector_ModifierFlag modifierFlags;
      modifierFlags.push_back(ModifierFlag::SHIFT_L);
      modifierFlags.push_back(ModifierFlag::NONE);
      EXPECT_FALSE(flagStatus.isOn(modifierFlags));
    }
    {
      Vector_ModifierFlag modifierFlags;
      modifierFlags.push_back(ModifierFlag::SHIFT_R);
      EXPECT_FALSE(flagStatus.isOn(modifierFlags));
    }
    {
      Vector_ModifierFlag modifierFlags;
      modifierFlags.push_back(ModifierFlag::SHIFT_L);
      modifierFlags.push_back(ModifierFlag::CONTROL_R);
      modifierFlags.push_back(ModifierFlag::COMMAND_R);
      modifierFlags.push_back(ModifierFlag::NONE);
      EXPECT_TRUE(flagStatus.isOn(modifierFlags));
    }
  }
}
