#include <sys/systm.h>

#include "remap.hpp"
#include "RemapUtil.hpp"
#include "Config.hpp"
#include "KeyCode.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace GeneratedCode {
#include "config/output/include.remapcode_func.cpp"
  }

  // ----------------------------------------
  static bool
  remap_keypadnumlock_togglekey_clear(const RemapParams &remapParams)
  {
    if (! config.option_keypadnumlock_togglekey_clear) return false;

    if (remapParams.isremapped) return false;
    if (remapParams.params.key != KeyCode::KEYPAD_CLEAR) return false;

    if (remapParams.params.eventType.isKeyDownOrModifierDown(remapParams.params.key, remapParams.params.flags)) {
      config.remap_keypadnumlock = ! config.remap_keypadnumlock;
    }

    remapParams.drop();
    return true;
  }

  // ----------------------------------------
  static bool
  remap_jis_jansi(const RemapParams &remapParams)
  {
    if (! config.remap_jis_jansi) return false;

    remapParams.params.keyboardType = KeyboardType::MACBOOK;

    {
      static RemapUtil::KeyToKey keytokey;
      if (keytokey.remap(remapParams, KeyCode::JIS_YEN, KeyCode::BACKQUOTE)) return true;
    }
    {
      static RemapUtil::KeyToKey keytokey;
      if (keytokey.remap(remapParams, KeyCode::JIS_UNDERSCORE, KeyCode::BACKQUOTE)) return true;
    }

    // Note: remap_jis_jansi return false if changed keyboardType only.
    return false;
  }

  static bool
  remap_pointing_relative_to_scroll(const RemapPointingParams_relative &remapParams)
  {
    if (! config.remap_pointing_relative_to_scroll) return false;

    RemapUtil::pointingRelativeToScroll(remapParams);
    return true;
  }
}

// ----------------------------------------------------------------------
void
org_pqrs_KeyRemap4MacBook::remap_core(const RemapParams &remapParams)
{
  FlagStatus::set(remapParams.params.key, remapParams.params.flags);

  // ======================================================================
  // normal remapping
#include "config/output/include.remapcode_call.cpp"

  remap_keypadnumlock_togglekey_clear(remapParams);
  remap_jis_jansi(remapParams);

  // ------------------------------------------------------------
  // *** Note: we need to call remap_space2shift, remap_enter2optionL_commandSpace (has SandS like behavior) as possible late. ***
  // *** If any key2modifier or modifier2key remappings are enabled, miss-cancelling are occured. ***
#include "config/output/include.remapcode_call_kom.cpp"

  // ------------------------------------------------------------
#include "config/output/include.remapcode_call_mhkk.cpp"

  // ------------------------------------------------------------
  remapParams.params.flags = FlagStatus::makeFlags();
}

void
org_pqrs_KeyRemap4MacBook::remap_consumer(const RemapConsumerParams &remapParams)
{
  FlagStatus::set();

#include "config/output/include.remapcode_call_consumer.cpp"

  remapParams.params.flags = FlagStatus::makeFlags();
}

void
org_pqrs_KeyRemap4MacBook::remap_pointing_relative_core(const RemapPointingParams_relative &remapParams)
{
  FlagStatus::set();

  remapParams.params.buttons.add(RemapUtil::getRemappedButtons());

#include "config/output/include.remapcode_call_pointing_relative.cpp"
  remap_pointing_relative_to_scroll(remapParams);
}
