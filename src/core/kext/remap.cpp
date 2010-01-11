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
namespace org_pqrs_KeyRemap4MacBook {
  namespace {
#include "config/output/include.remapcode_info.cpp"

    typedef void (*RemapFunc_key)(RemapParams& remapParams);
    RemapFunc_key listRemapFunc_key[MAXNUM_REMAPFUNC_KEY + 1];
    size_t listRemapFunc_key_size = 0;

    typedef void (*RemapFunc_consumer)(RemapConsumerParams& remapParams);
    RemapFunc_consumer listRemapFunc_consumer[MAXNUM_REMAPFUNC_CONSUMER + 1];
    size_t listRemapFunc_consumer_size = 0;

    typedef void (*RemapFunc_pointing)(RemapPointingParams_relative& remapParams);
    RemapFunc_pointing listRemapFunc_pointing[MAXNUM_REMAPFUNC_POINTING + 1];
    size_t listRemapFunc_pointing_size = 0;
  }
}

void
org_pqrs_KeyRemap4MacBook::refresh_remapfunc(void)
{
  listRemapFunc_key_size = 0;
  listRemapFunc_consumer_size = 0;
  listRemapFunc_pointing_size = 0;

  // ------------------------------------------------------------
#include "config/output/include.remapcode_refresh_remapfunc_key.cpp"

  if (config.option_keypadnumlock_togglekey_clear) {
    listRemapFunc_key[listRemapFunc_key_size] = RemapClass_remap_keypadnumlock_togglekey_clear::remap_key;
    ++listRemapFunc_key_size;
  }

  // ------------------------------------------------------------
#include "config/output/include.remapcode_refresh_remapfunc_consumer.cpp"

  // ------------------------------------------------------------
#include "config/output/include.remapcode_refresh_remapfunc_pointing.cpp"

  if (config.remap_pointing_relative_to_scroll) {
    listRemapFunc_pointing[listRemapFunc_pointing_size] = RemapClass_remap_pointing_relative_to_scroll::remap_pointing;
    ++listRemapFunc_pointing_size;
  }
}

void
org_pqrs_KeyRemap4MacBook::remap_core(RemapParams& remapParams)
{
  FlagStatus::set(remapParams.params.key, remapParams.params.flags);

  for (size_t i = 0; i < listRemapFunc_key_size; ++i) {
    RemapFunc_key func = listRemapFunc_key[i];
    if (func) {
      func(remapParams);
    }
  }
}

void
org_pqrs_KeyRemap4MacBook::remap_consumer(RemapConsumerParams& remapParams)
{
  FlagStatus::set();

  for (size_t i = 0; i < listRemapFunc_consumer_size; ++i) {
    RemapFunc_consumer func = listRemapFunc_consumer[i];
    if (func) {
      func(remapParams);
    }
  }
}

void
org_pqrs_KeyRemap4MacBook::remap_pointing_relative_core(RemapPointingParams_relative& remapParams)
{
  FlagStatus::set();

  for (size_t i = 0; i < listRemapFunc_pointing_size; ++i) {
    RemapFunc_pointing func = listRemapFunc_pointing[i];
    if (func) {
      func(remapParams);
    }
  }
}
