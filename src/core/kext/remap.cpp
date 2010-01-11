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
  class RemapClass_remap_keypadnumlock_togglekey_clear {
  public:
    static void remap_key(RemapParams& remapParams) {
      if (remapParams.isremapped) return;
      if (remapParams.params.key != KeyCode::KEYPAD_CLEAR) return;

      if (remapParams.isKeyDownOrModifierDown()) {
        config.remap_keypadnumlock = ! config.remap_keypadnumlock;
      }

      remapParams.drop();
    }
  };

  // ----------------------------------------
  class RemapClass_remap_pointing_relative_to_scroll {
  public:
    static void remap_pointing(RemapPointingParams_relative& remapParams) {
      prts_.remap(remapParams, PointingButton::NONE, Flags(0));
    }

  private:
    static RemapUtil::PointingRelativeToScroll prts_;
  };
  RemapUtil::PointingRelativeToScroll RemapClass_remap_pointing_relative_to_scroll::prts_;
}

// ----------------------------------------------------------------------
void
org_pqrs_KeyRemap4MacBook::remap_core(RemapParams& remapParams)
{
  FlagStatus::set(remapParams.params.key, remapParams.params.flags);

  // ======================================================================
  // normal remapping
#include "config/output/include.remapcode_call.cpp"

  if (config.option_keypadnumlock_togglekey_clear) {
    RemapClass_remap_keypadnumlock_togglekey_clear::remap_key(remapParams);
  }
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
  if (config.remap_pointing_relative_to_scroll) {
    RemapClass_remap_pointing_relative_to_scroll::remap_pointing(remapParams);
  }
}
