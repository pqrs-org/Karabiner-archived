#define CATCH_CONFIG_MAIN
#include "../../include/catch.hpp"

#include <ostream>

#include "Config.hpp"
#include "FlagStatus.hpp"
#include "KeyCode.hpp"
#include "KeyCodeModifierFlagPairs.hpp"

using namespace org_pqrs_Karabiner;
Config config;

Flags operator|(ModifierFlag lhs, ModifierFlag rhs) { return Flags(lhs.getRawBits() | rhs.getRawBits()); }

TEST_CASE("setUp", "[Generic]") {
  KeyCodeModifierFlagPairs::clearVirtualModifiers();
}

TEST_CASE("makeFlags", "[FlagStatus]") {
  FlagStatus flagStatus;
  REQUIRE(flagStatus.makeFlags() == Flags());

  flagStatus.set();
  REQUIRE(flagStatus.makeFlags() == Flags());

  flagStatus.set(KeyCode::A, Flags(0));
  REQUIRE(flagStatus.makeFlags() == Flags());

  // down SHIFT_L
  flagStatus.set(KeyCode::SHIFT_L, Flags(ModifierFlag::SHIFT_L));
  REQUIRE(flagStatus.makeFlags() == Flags(ModifierFlag::SHIFT_L));

  // no effect with ModifierFlag::NONE
  flagStatus.set(KeyCode::A, Flags(ModifierFlag::NONE));
  REQUIRE(flagStatus.makeFlags() == Flags(ModifierFlag::SHIFT_L));

  // down CONTROL_
  flagStatus.set(KeyCode::CONTROL_L, ModifierFlag::CONTROL_L | ModifierFlag::SHIFT_L);
  REQUIRE(flagStatus.makeFlags() == Flags(ModifierFlag::SHIFT_L | ModifierFlag::CONTROL_L));

  // down A
  flagStatus.set(KeyCode::A, ModifierFlag::CONTROL_L | ModifierFlag::SHIFT_L);
  REQUIRE(flagStatus.makeFlags() == Flags(ModifierFlag::SHIFT_L | ModifierFlag::CONTROL_L));

  // up SHIFT_L
  flagStatus.set(KeyCode::SHIFT_L, Flags(ModifierFlag::CONTROL_L));
  REQUIRE(flagStatus.makeFlags() == Flags(ModifierFlag::CONTROL_L));

  // up CONTROL_L
  flagStatus.set(KeyCode::CONTROL_L, Flags(0));
  REQUIRE(flagStatus.makeFlags() == Flags());

  // All flags
  flagStatus.reset();
  flagStatus.set(KeyCode::CAPSLOCK, Flags(ModifierFlag::CAPSLOCK));
  REQUIRE(flagStatus.makeFlags() == Flags(ModifierFlag::CAPSLOCK));

  flagStatus.set(KeyCode::CAPSLOCK, Flags(0));
  REQUIRE(flagStatus.makeFlags() == Flags(0));

  flagStatus.reset();
  flagStatus.set(KeyCode::SHIFT_L, Flags(ModifierFlag::SHIFT_L));
  REQUIRE(flagStatus.makeFlags() == Flags(ModifierFlag::SHIFT_L));

  flagStatus.reset();
  flagStatus.set(KeyCode::SHIFT_R, Flags(ModifierFlag::SHIFT_R));
  REQUIRE(flagStatus.makeFlags() == Flags(ModifierFlag::SHIFT_R));

  flagStatus.reset();
  flagStatus.set(KeyCode::CONTROL_L, Flags(ModifierFlag::CONTROL_L));
  REQUIRE(flagStatus.makeFlags() == Flags(ModifierFlag::CONTROL_L));

  flagStatus.reset();
  flagStatus.set(KeyCode::CONTROL_R, Flags(ModifierFlag::CONTROL_R));
  REQUIRE(flagStatus.makeFlags() == Flags(ModifierFlag::CONTROL_R));

  flagStatus.reset();
  flagStatus.set(KeyCode::OPTION_L, Flags(ModifierFlag::OPTION_L));
  REQUIRE(flagStatus.makeFlags() == Flags(ModifierFlag::OPTION_L));

  flagStatus.reset();
  flagStatus.set(KeyCode::OPTION_R, Flags(ModifierFlag::OPTION_R));
  REQUIRE(flagStatus.makeFlags() == Flags(ModifierFlag::OPTION_R));

  flagStatus.reset();
  flagStatus.set(KeyCode::COMMAND_L, Flags(ModifierFlag::COMMAND_L));
  REQUIRE(flagStatus.makeFlags() == Flags(ModifierFlag::COMMAND_L));

  flagStatus.reset();
  flagStatus.set(KeyCode::COMMAND_R, Flags(ModifierFlag::COMMAND_R));
  REQUIRE(flagStatus.makeFlags() == Flags(ModifierFlag::COMMAND_R));

  flagStatus.reset();
  flagStatus.set(KeyCode::FN, Flags(ModifierFlag::FN));
  REQUIRE(flagStatus.makeFlags() == Flags(ModifierFlag::FN));
}

TEST_CASE("getFlag", "[FlagStatus]") {
  FlagStatus flagStatus;

  REQUIRE(flagStatus.getFlag(0) == ModifierFlag::CAPSLOCK);
}

TEST_CASE("increase", "[FlagStatus]") {
  {
    FlagStatus flagStatus;

    // Do nothing with ModifierFlag::NONE.
    flagStatus.increase(ModifierFlag::NONE);
    REQUIRE(flagStatus.makeFlags() == Flags(0));

    flagStatus.increase(ModifierFlag::SHIFT_L);
    REQUIRE(flagStatus.makeFlags() == Flags(ModifierFlag::SHIFT_L));

    {
      Vector_ModifierFlag v;
      v.push_back(ModifierFlag::COMMAND_L);
      v.push_back(ModifierFlag::CONTROL_L);
      flagStatus.increase(v);
      REQUIRE(flagStatus.makeFlags() == Flags(ModifierFlag::COMMAND_L | ModifierFlag::CONTROL_L | ModifierFlag::SHIFT_L));
    }

    flagStatus.increase(ModifierFlag::NONE);
    REQUIRE(flagStatus.makeFlags() == Flags(ModifierFlag::COMMAND_L | ModifierFlag::CONTROL_L | ModifierFlag::SHIFT_L));
  }

  {
    FlagStatus flagStatus;
    Vector_ModifierFlag v;
    v.push_back(ModifierFlag::COMMAND_L);
    v.push_back(ModifierFlag::CONTROL_L);
    flagStatus.increase(ModifierFlag::SHIFT_L, v);
    REQUIRE(flagStatus.makeFlags() == Flags(ModifierFlag::COMMAND_L | ModifierFlag::CONTROL_L | ModifierFlag::SHIFT_L));
  }
  {
    FlagStatus flagStatus;
    Vector_ModifierFlag v;
    v.push_back(ModifierFlag::COMMAND_L);
    v.push_back(ModifierFlag::CONTROL_L);
    flagStatus.increase(ModifierFlag::COMMAND_L, v);
    REQUIRE(flagStatus.makeFlags() == Flags(ModifierFlag::COMMAND_L | ModifierFlag::CONTROL_L));
    flagStatus.decrease(v);
    REQUIRE(flagStatus.makeFlags() == Flags(0));
  }
}

TEST_CASE("decrease", "[FlagStatus]") {
  FlagStatus flagStatus;

  {
    Vector_ModifierFlag v;
    v.push_back(ModifierFlag::COMMAND_L);
    v.push_back(ModifierFlag::CONTROL_L);
    flagStatus.increase(v);
    REQUIRE(flagStatus.makeFlags() == Flags(ModifierFlag::COMMAND_L | ModifierFlag::CONTROL_L));
  }

  flagStatus.decrease(ModifierFlag::CONTROL_L);
  REQUIRE(flagStatus.makeFlags() == Flags(ModifierFlag::COMMAND_L));
}

TEST_CASE("temporary_increase", "[FlagStatus]") {
  FlagStatus flagStatus;

  // Do nothing with ModifierFlag::NONE.
  flagStatus.temporary_increase(ModifierFlag::NONE);
  REQUIRE(flagStatus.makeFlags() == Flags(0));

  {
    Vector_ModifierFlag v;
    v.push_back(ModifierFlag::COMMAND_L);
    v.push_back(ModifierFlag::CONTROL_L);
    flagStatus.increase(v);
    REQUIRE(flagStatus.makeFlags() == Flags(ModifierFlag::COMMAND_L | ModifierFlag::CONTROL_L));
  }

  flagStatus.temporary_increase(ModifierFlag::OPTION_L);
  REQUIRE(flagStatus.makeFlags() == Flags(ModifierFlag::COMMAND_L | ModifierFlag::CONTROL_L | ModifierFlag::OPTION_L));

  // temporary_increase will reset by flagStatus.set
  flagStatus.set(KeyCode::COMMAND_L, Flags(ModifierFlag::COMMAND_L));
  REQUIRE(flagStatus.makeFlags() == Flags(ModifierFlag::COMMAND_L | ModifierFlag::CONTROL_L));
}

TEST_CASE("temporary_decrease", "[FlagStatus]") {
  FlagStatus flagStatus;

  {
    Vector_ModifierFlag v;
    v.push_back(ModifierFlag::COMMAND_L);
    v.push_back(ModifierFlag::CONTROL_L);
    flagStatus.increase(v);
    REQUIRE(flagStatus.makeFlags() == Flags(ModifierFlag::COMMAND_L | ModifierFlag::CONTROL_L));
  }

  flagStatus.temporary_decrease(ModifierFlag::CONTROL_L);
  REQUIRE(flagStatus.makeFlags() == Flags(ModifierFlag::COMMAND_L));

  // temporary_increase will reset by flagStatus.set
  flagStatus.set(KeyCode::COMMAND_L, Flags(ModifierFlag::COMMAND_L));
  REQUIRE(flagStatus.makeFlags() == Flags(ModifierFlag::COMMAND_L | ModifierFlag::CONTROL_L));
}

TEST_CASE("lock_increase", "[FlagStatus]") {
  FlagStatus flagStatus;

  // Do nothing with ModifierFlag::NONE.
  flagStatus.lock_increase(ModifierFlag::NONE);
  REQUIRE(flagStatus.makeFlags() == Flags(0));

  flagStatus.lock_increase(ModifierFlag::COMMAND_L);
  REQUIRE(flagStatus.makeFlags() == Flags(ModifierFlag::COMMAND_L));

  // lock don't cancel by reset & set.
  flagStatus.reset();
  flagStatus.set(KeyCode::A, Flags(0));
  REQUIRE(flagStatus.makeFlags() == Flags(ModifierFlag::COMMAND_L));

  flagStatus.lock_decrease(ModifierFlag::COMMAND_L);
  REQUIRE(flagStatus.makeFlags() == Flags());
}

TEST_CASE("negative_lock_increase", "[FlagStatus]") {
  FlagStatus flagStatus;

  // ----------------------------------------
  flagStatus.negative_lock_increase(ModifierFlag::COMMAND_L);
  REQUIRE(flagStatus.makeFlags() == Flags(0));

  flagStatus.increase(ModifierFlag::COMMAND_L);
  REQUIRE(flagStatus.makeFlags() == Flags(0));

  flagStatus.increase(ModifierFlag::COMMAND_L);
  REQUIRE(flagStatus.makeFlags() == Flags(ModifierFlag::COMMAND_L));

  // ----------------------------------------
  // lock don't cancel by reset & set.
  flagStatus.reset();
  flagStatus.set(KeyCode::A, Flags(0));
  REQUIRE(flagStatus.makeFlags() == Flags(0));

  flagStatus.increase(ModifierFlag::COMMAND_L);
  REQUIRE(flagStatus.makeFlags() == Flags(0));

  flagStatus.increase(ModifierFlag::COMMAND_L);
  REQUIRE(flagStatus.makeFlags() == Flags(ModifierFlag::COMMAND_L));

  // ----------------------------------------
  flagStatus.reset();
  flagStatus.negative_lock_decrease(ModifierFlag::COMMAND_L);
  REQUIRE(flagStatus.makeFlags() == Flags(0));

  flagStatus.increase(ModifierFlag::COMMAND_L);
  REQUIRE(flagStatus.makeFlags() == Flags(ModifierFlag::COMMAND_L));
}

TEST_CASE("lock_toggle", "[FlagStatus]") {
  FlagStatus flagStatus;

  flagStatus.lock_increase(ModifierFlag::COMMAND_L);
  REQUIRE(flagStatus.makeFlags() == Flags(ModifierFlag::COMMAND_L));

  flagStatus.lock_toggle(ModifierFlag::COMMAND_L);
  REQUIRE(flagStatus.makeFlags() == Flags(0));

  flagStatus.lock_toggle(ModifierFlag::COMMAND_L);
  REQUIRE(flagStatus.makeFlags() == Flags(ModifierFlag::COMMAND_L));
}

TEST_CASE("lock_clear", "[FlagStatus]") {
  FlagStatus flagStatus;

  {
    Vector_ModifierFlag v;
    v.push_back(ModifierFlag::COMMAND_L);
    v.push_back(ModifierFlag::FN);
    v.push_back(ModifierFlag::SHIFT_L);
    flagStatus.lock_increase(v);
    REQUIRE(flagStatus.makeFlags() == (ModifierFlag::COMMAND_L | ModifierFlag::FN | ModifierFlag::SHIFT_L));
  }
  {
    flagStatus.increase(ModifierFlag::CAPSLOCK);
    REQUIRE(flagStatus.makeFlags() == (ModifierFlag::CAPSLOCK | ModifierFlag::COMMAND_L | ModifierFlag::FN | ModifierFlag::SHIFT_L));
  }

  flagStatus.lock_clear();
  REQUIRE(flagStatus.makeFlags() == Flags(0));
}

TEST_CASE("negative_lock_clear", "[FlagStatus]") {
  FlagStatus flagStatus;

  {
    Vector_ModifierFlag v;
    v.push_back(ModifierFlag::COMMAND_L);
    v.push_back(ModifierFlag::FN);
    v.push_back(ModifierFlag::SHIFT_L);
    flagStatus.negative_lock_increase(v);
    REQUIRE(flagStatus.makeFlags() == Flags(0));

    flagStatus.increase(v);
    REQUIRE(flagStatus.makeFlags() == Flags(0));

    flagStatus.increase(v);
    REQUIRE(flagStatus.makeFlags() == (ModifierFlag::COMMAND_L | ModifierFlag::FN | ModifierFlag::SHIFT_L));

    flagStatus.reset();
    REQUIRE(flagStatus.makeFlags() == Flags(0));

    flagStatus.increase(v);
    REQUIRE(flagStatus.makeFlags() == Flags(0));

    flagStatus.negative_lock_clear();
    REQUIRE(flagStatus.makeFlags() == (ModifierFlag::COMMAND_L | ModifierFlag::FN | ModifierFlag::SHIFT_L));
  }
}

TEST_CASE("sticky_increase", "[FlagStatus]") {
  FlagStatus flagStatus;

  // Do nothing with ModifierFlag::NONE.
  flagStatus.sticky_increase(ModifierFlag::NONE);
  REQUIRE(flagStatus.makeFlags() == Flags(0));

  {
    Vector_ModifierFlag v;
    v.push_back(ModifierFlag::COMMAND_L);
    v.push_back(ModifierFlag::FN);
    flagStatus.sticky_increase(v);
    REQUIRE(flagStatus.makeFlags() == Flags(ModifierFlag::COMMAND_L | ModifierFlag::FN));
  }

  flagStatus.sticky_decrease(ModifierFlag::COMMAND_L);
  REQUIRE(flagStatus.makeFlags() == Flags(ModifierFlag::FN));
}

TEST_CASE("sticky_toggle", "[FlagStatus]") {
  FlagStatus flagStatus;

  flagStatus.sticky_increase(ModifierFlag::COMMAND_L);
  REQUIRE(flagStatus.makeFlags() == Flags(ModifierFlag::COMMAND_L));

  flagStatus.sticky_toggle(ModifierFlag::COMMAND_L);
  REQUIRE(flagStatus.makeFlags() == Flags(0));

  flagStatus.sticky_toggle(ModifierFlag::COMMAND_L);
  REQUIRE(flagStatus.makeFlags() == Flags(ModifierFlag::COMMAND_L));
}

TEST_CASE("sticky_clear", "[FlagStatus]") {
  FlagStatus flagStatus;

  {
    Vector_ModifierFlag v;
    v.push_back(ModifierFlag::COMMAND_L);
    v.push_back(ModifierFlag::FN);
    v.push_back(ModifierFlag::SHIFT_L);
    flagStatus.sticky_increase(v);
    REQUIRE(flagStatus.makeFlags() == (ModifierFlag::COMMAND_L | ModifierFlag::FN | ModifierFlag::SHIFT_L));
  }

  flagStatus.sticky_clear();
  REQUIRE(flagStatus.makeFlags() == Flags(0));
}

TEST_CASE("lazy_increase", "[FlagStatus]") {
  FlagStatus flagStatus;

  // +1 (total 1)
  flagStatus.lazy_increase(ModifierFlag::SHIFT_L);
  REQUIRE(flagStatus.makeFlags() == Flags(0));

  // +0 (total 1)
  flagStatus.lazy_enable();
  REQUIRE(flagStatus.makeFlags() == Flags(ModifierFlag::SHIFT_L));

  // -1 (total 0)
  flagStatus.lazy_decrease(ModifierFlag::SHIFT_L);
  REQUIRE(flagStatus.makeFlags() == Flags(0));

  // +1 (total 1)
  flagStatus.lazy_increase(ModifierFlag::SHIFT_L);
  REQUIRE(flagStatus.makeFlags() == Flags(ModifierFlag::SHIFT_L));

  flagStatus.lazy_disable_if_off();
  REQUIRE(flagStatus.makeFlags() == Flags(ModifierFlag::SHIFT_L));

  // +2 (total 2)
  flagStatus.lazy_increase(ModifierFlag::SHIFT_L);
  REQUIRE(flagStatus.makeFlags() == Flags(ModifierFlag::SHIFT_L));

  // -1 (total 1)
  flagStatus.lazy_decrease(ModifierFlag::SHIFT_L);
  REQUIRE(flagStatus.makeFlags() == Flags(ModifierFlag::SHIFT_L));

  // => 0 (lazy modifier is disabled when reset.)
  flagStatus.reset();

  // +1 (total 1)
  flagStatus.lazy_increase(ModifierFlag::SHIFT_L);
  REQUIRE(flagStatus.makeFlags() == Flags(0));
}

TEST_CASE("CapsLock", "[FlagStatus]") {
  FlagStatus flagStatus;

  flagStatus.set(KeyCode::CAPSLOCK, Flags(ModifierFlag::CAPSLOCK));
  REQUIRE(flagStatus.makeFlags() == Flags(ModifierFlag::CAPSLOCK));

  flagStatus.reset();

  flagStatus.set(KeyCode::A, Flags(ModifierFlag::CAPSLOCK));
  REQUIRE(flagStatus.makeFlags() == Flags(ModifierFlag::CAPSLOCK));

  // from other keyboard
  flagStatus.set(KeyCode::A, Flags(0));
  REQUIRE(flagStatus.makeFlags() == Flags(ModifierFlag::CAPSLOCK));

  flagStatus.set(KeyCode::A, Flags(ModifierFlag::CAPSLOCK));
  REQUIRE(flagStatus.makeFlags() == Flags(ModifierFlag::CAPSLOCK));

  // reset
  flagStatus.set(KeyCode::CAPSLOCK, Flags(0));
  REQUIRE(flagStatus.makeFlags() == Flags());

  // soft caps
  flagStatus.lock_increase(ModifierFlag::CAPSLOCK);
  flagStatus.set(KeyCode::A, Flags(0));
  REQUIRE(flagStatus.makeFlags() == Flags(ModifierFlag::CAPSLOCK));

  // soft caps will be canceled by hardware caps
  flagStatus.set(KeyCode::CAPSLOCK, Flags(0));
  REQUIRE(flagStatus.makeFlags() == Flags(0));
}

TEST_CASE("isOn", "[FlagStatus]") {
  {
    FlagStatus flagStatus;

    {
      Vector_ModifierFlag modifierFlags;
      REQUIRE(flagStatus.isOn(modifierFlags) == true);
    }
    {
      Vector_ModifierFlag modifierFlags;
      modifierFlags.push_back(ModifierFlag(ModifierFlag::ZERO));
      REQUIRE(flagStatus.isOn(modifierFlags) == true);
    }
    {
      Vector_ModifierFlag modifierFlags;
      modifierFlags.push_back(ModifierFlag::NONE);
      REQUIRE(flagStatus.isOn(modifierFlags) == true);
    }
    {
      Vector_ModifierFlag modifierFlags;
      modifierFlags.push_back(ModifierFlag::NONE);
      modifierFlags.push_back(ModifierFlag::ZERO);
      REQUIRE(flagStatus.isOn(modifierFlags) == true);
    }
  }

  {
    FlagStatus flagStatus;
    flagStatus.increase(ModifierFlag::SHIFT_L);

    {
      Vector_ModifierFlag modifierFlags;
      modifierFlags.push_back(ModifierFlag::SHIFT_L);
      REQUIRE(flagStatus.isOn(modifierFlags) == true);
    }
    {
      Vector_ModifierFlag modifierFlags;
      modifierFlags.push_back(ModifierFlag::SHIFT_L);
      modifierFlags.push_back(ModifierFlag::NONE);
      REQUIRE(flagStatus.isOn(modifierFlags) == true);
    }
    {
      Vector_ModifierFlag modifierFlags;
      modifierFlags.push_back(ModifierFlag::SHIFT_R);
      REQUIRE(flagStatus.isOn(modifierFlags) == false);
    }
    {
      Vector_ModifierFlag modifierFlags;
      modifierFlags.push_back(ModifierFlag::SHIFT_L);
      modifierFlags.push_back(ModifierFlag::ZERO);
      REQUIRE(flagStatus.isOn(modifierFlags) == true);
    }
  }

  {
    FlagStatus flagStatus;
    flagStatus.increase(ModifierFlag::SHIFT_L);
    flagStatus.increase(ModifierFlag::ZERO);

    {
      Vector_ModifierFlag modifierFlags;
      modifierFlags.push_back(ModifierFlag::SHIFT_L);
      REQUIRE(flagStatus.isOn(modifierFlags) == true);
    }
    {
      Vector_ModifierFlag modifierFlags;
      modifierFlags.push_back(ModifierFlag::SHIFT_L);
      modifierFlags.push_back(ModifierFlag::NONE);
      REQUIRE(flagStatus.isOn(modifierFlags) == true);
    }
    {
      Vector_ModifierFlag modifierFlags;
      modifierFlags.push_back(ModifierFlag::SHIFT_R);
      REQUIRE(flagStatus.isOn(modifierFlags) == false);
    }
    {
      Vector_ModifierFlag modifierFlags;
      modifierFlags.push_back(ModifierFlag::SHIFT_L);
      modifierFlags.push_back(ModifierFlag::ZERO);
      REQUIRE(flagStatus.isOn(modifierFlags) == true);
    }
    {
      Vector_ModifierFlag modifierFlags;
      modifierFlags.push_back(ModifierFlag::SHIFT_L);
      modifierFlags.push_back(ModifierFlag::ZERO);
      modifierFlags.push_back(ModifierFlag::NONE);
      REQUIRE(flagStatus.isOn(modifierFlags) == true);
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
      REQUIRE(flagStatus.isOn(modifierFlags) == true);
    }
    {
      Vector_ModifierFlag modifierFlags;
      modifierFlags.push_back(ModifierFlag::SHIFT_L);
      modifierFlags.push_back(ModifierFlag::NONE);
      REQUIRE(flagStatus.isOn(modifierFlags) == false);
    }
    {
      Vector_ModifierFlag modifierFlags;
      modifierFlags.push_back(ModifierFlag::SHIFT_R);
      REQUIRE(flagStatus.isOn(modifierFlags) == false);
    }
    {
      Vector_ModifierFlag modifierFlags;
      modifierFlags.push_back(ModifierFlag::SHIFT_L);
      modifierFlags.push_back(ModifierFlag::CONTROL_R);
      modifierFlags.push_back(ModifierFlag::COMMAND_R);
      modifierFlags.push_back(ModifierFlag::NONE);
      REQUIRE(flagStatus.isOn(modifierFlags) == true);
    }
  }
}

TEST_CASE("isLocked", "[FlagStatus]") {
  {
    FlagStatus flagStatus;

    {
      Vector_ModifierFlag modifierFlags;
      REQUIRE(flagStatus.isLocked(modifierFlags) == true);
    }
    {
      Vector_ModifierFlag modifierFlags;
      modifierFlags.push_back(ModifierFlag(ModifierFlag::ZERO));
      REQUIRE(flagStatus.isLocked(modifierFlags) == true);
    }
    {
      Vector_ModifierFlag modifierFlags;
      modifierFlags.push_back(ModifierFlag::NONE);
      REQUIRE(flagStatus.isLocked(modifierFlags) == true);
    }
    {
      Vector_ModifierFlag modifierFlags;
      modifierFlags.push_back(ModifierFlag::NONE);
      modifierFlags.push_back(ModifierFlag::ZERO);
      REQUIRE(flagStatus.isLocked(modifierFlags) == true);
    }

    {
      Vector_ModifierFlag modifierFlags;
      modifierFlags.push_back(ModifierFlag::SHIFT_L);
      REQUIRE(flagStatus.isLocked(modifierFlags) == false);

      flagStatus.increase(ModifierFlag::SHIFT_L);
      REQUIRE(flagStatus.isLocked(modifierFlags) == false);

      flagStatus.lock_increase(ModifierFlag::SHIFT_L);
      REQUIRE(flagStatus.isLocked(modifierFlags) == true);
    }
  }
}

TEST_CASE("isStuck", "[FlagStatus]") {
  {
    FlagStatus flagStatus;

    {
      Vector_ModifierFlag modifierFlags;
      REQUIRE(flagStatus.isStuck(modifierFlags) == true);
    }
    {
      Vector_ModifierFlag modifierFlags;
      modifierFlags.push_back(ModifierFlag(ModifierFlag::ZERO));
      REQUIRE(flagStatus.isStuck(modifierFlags) == true);
    }
    {
      Vector_ModifierFlag modifierFlags;
      modifierFlags.push_back(ModifierFlag::NONE);
      REQUIRE(flagStatus.isStuck(modifierFlags) == true);
    }
    {
      Vector_ModifierFlag modifierFlags;
      modifierFlags.push_back(ModifierFlag::NONE);
      modifierFlags.push_back(ModifierFlag::ZERO);
      REQUIRE(flagStatus.isStuck(modifierFlags) == true);
    }

    {
      Vector_ModifierFlag modifierFlags;
      modifierFlags.push_back(ModifierFlag::SHIFT_L);
      REQUIRE(flagStatus.isStuck(modifierFlags) == false);

      flagStatus.increase(ModifierFlag::SHIFT_L);
      REQUIRE(flagStatus.isStuck(modifierFlags) == false);

      flagStatus.sticky_increase(ModifierFlag::SHIFT_L);
      REQUIRE(flagStatus.isStuck(modifierFlags) == true);

      flagStatus.sticky_clear();
      REQUIRE(flagStatus.isStuck(modifierFlags) == false);
    }
  }
}

TEST_CASE("subtract", "[FlagStatus]") {
  FlagStatus flagStatus1;
  FlagStatus flagStatus2;

  flagStatus1.increase(ModifierFlag::CONTROL_L);
  flagStatus1.increase(ModifierFlag::OPTION_L);
  flagStatus1.increase(ModifierFlag::SHIFT_L);
  flagStatus1.increase(ModifierFlag::SHIFT_L);
  flagStatus1.decrease(ModifierFlag::COMMAND_R);

  flagStatus2.increase(ModifierFlag::CONTROL_L);
  flagStatus2.increase(ModifierFlag::FN);

  Vector_ModifierFlag v;
  flagStatus1.subtract(flagStatus2, v);
  REQUIRE(v.size() == 3);
  REQUIRE(v[0] == ModifierFlag::OPTION_L);
  REQUIRE(v[1] == ModifierFlag::SHIFT_L);
  REQUIRE(v[2] == ModifierFlag::SHIFT_L);

  flagStatus2.subtract(flagStatus1, v);
  REQUIRE(v.size() == 2);
  REQUIRE(v[0] == ModifierFlag::COMMAND_R);
  REQUIRE(v[1] == ModifierFlag::FN);
}

TEST_CASE("ScopedSetter", "[FlagStatus]") {
  FlagStatus flagStatus1;
  FlagStatus flagStatus2;

  flagStatus1.increase(ModifierFlag::CONTROL_L);
  flagStatus1.increase(ModifierFlag::OPTION_L);
  flagStatus1.increase(ModifierFlag::SHIFT_L);
  flagStatus1.increase(ModifierFlag::SHIFT_L);
  flagStatus1.decrease(ModifierFlag::COMMAND_R);

  flagStatus2.increase(ModifierFlag::COMMAND_R);
  flagStatus2.increase(ModifierFlag::CONTROL_L);
  flagStatus2.increase(ModifierFlag::FN);

  {
    REQUIRE(flagStatus1.makeFlags() == (ModifierFlag::CONTROL_L | ModifierFlag::OPTION_L | ModifierFlag::SHIFT_L));
    REQUIRE(flagStatus2.makeFlags() == (ModifierFlag::COMMAND_R | ModifierFlag::CONTROL_L | ModifierFlag::FN));

    {
      FlagStatus::ScopedSetter scopedSetter(flagStatus1, flagStatus2);

      REQUIRE(flagStatus1.makeFlags() == (ModifierFlag::COMMAND_R | ModifierFlag::CONTROL_L | ModifierFlag::FN));
      REQUIRE(flagStatus2.makeFlags() == (ModifierFlag::COMMAND_R | ModifierFlag::CONTROL_L | ModifierFlag::FN));
    }

    REQUIRE(flagStatus1.makeFlags() == (ModifierFlag::CONTROL_L | ModifierFlag::OPTION_L | ModifierFlag::SHIFT_L));
    REQUIRE(flagStatus2.makeFlags() == (ModifierFlag::COMMAND_R | ModifierFlag::CONTROL_L | ModifierFlag::FN));
  }
}
