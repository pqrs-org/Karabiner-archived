#include <sys/systm.h>

#include "remap.hpp"
#include "RemapUtil.hpp"
#include "Config.hpp"
#include "KeyCode.hpp"
#include "ListHookedKeyboard.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace GeneratedCode {
#include "config/output/include.remapcode_func.cpp"
  }

  // ----------------------------------------
  static void
  remap_keypadnumlock_togglekey_clear(RemapParams& remapParams)
  {
    if (! config.option_keypadnumlock_togglekey_clear) return;

    if (remapParams.isremapped) return;
    if (remapParams.params.key != KeyCode::KEYPAD_CLEAR) return;

    if (remapParams.isKeyDownOrModifierDown()) {
      config.remap_keypadnumlock = ! config.remap_keypadnumlock;
    }

    remapParams.drop();
  }

  // ----------------------------------------
  static void
  remap_pointing_relative_to_scroll(RemapPointingParams_relative& remapParams)
  {
    if (! config.remap_pointing_relative_to_scroll) return;

    static RemapUtil::PointingRelativeToScroll prts;
    prts.remap(remapParams, PointingButton::NONE, Flags(0));
  }
}

// ----------------------------------------------------------------------
void
org_pqrs_KeyRemap4MacBook::remap_core(RemapParams& remapParams)
{
  FlagStatus::set(remapParams.params.key, remapParams.params.flags);

  // ======================================================================
  // normal remapping
#include "config/output/include.remapcode_call.cpp"

  remap_keypadnumlock_togglekey_clear(remapParams);
}

void
org_pqrs_KeyRemap4MacBook::remap_consumer(RemapConsumerParams& remapParams)
{
  FlagStatus::set();

#include "config/output/include.remapcode_call_consumer.cpp"
}

void
org_pqrs_KeyRemap4MacBook::remap_pointing_relative_core(RemapPointingParams_relative& remapParams)
{
  FlagStatus::set();

#include "config/output/include.remapcode_call_pointing_relative.cpp"
  remap_pointing_relative_to_scroll(remapParams);
}
