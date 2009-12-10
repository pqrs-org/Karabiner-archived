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
  general_capslock_led_hack(const RemapParams& /*remapParams*/)
  {
    if (! config.general_capslock_led_hack) return;

    HookedKeyboard* hk = ListHookedKeyboard::instance().get();
    if (! hk) return;

    IOHIKeyboard* kbd = hk->get();
    if (! kbd) return;

    int led = kbd->getLEDStatus();
    if (led == 0) {
      kbd->setAlphaLockFeedback(true);
    }
  }

  // ----------------------------------------
  static void
  remap_keypadnumlock_togglekey_clear(const RemapParams& remapParams)
  {
    if (! config.option_keypadnumlock_togglekey_clear) return;

    if (remapParams.isremapped) return;
    if (remapParams.params.key != KeyCode::KEYPAD_CLEAR) return;

    if (remapParams.params.eventType.isKeyDownOrModifierDown(remapParams.params.key, remapParams.params.flags)) {
      config.remap_keypadnumlock = ! config.remap_keypadnumlock;
    }

    remapParams.drop();
  }

  // ----------------------------------------
  static void
  remap_jis_jansi(const RemapParams& remapParams)
  {
    if (! config.remap_jis_jansi) return;

    remapParams.params.keyboardType = KeyboardType::MACBOOK;

    {
      static RemapUtil::KeyToKey keytokey;
      if (keytokey.remap(remapParams, KeyCode::JIS_YEN, KeyCode::BACKQUOTE)) return;
    }
    {
      static RemapUtil::KeyToKey keytokey;
      if (keytokey.remap(remapParams, KeyCode::JIS_UNDERSCORE, KeyCode::BACKQUOTE)) return;
    }
  }

  static void
  remap_pointing_relative_to_scroll(const RemapPointingParams_relative& remapParams)
  {
    if (! config.remap_pointing_relative_to_scroll) return;

    RemapUtil::pointingRelativeToScroll(remapParams);
  }
}

// ----------------------------------------------------------------------
void
org_pqrs_KeyRemap4MacBook::remap_core(const RemapParams& remapParams)
{
  FlagStatus::set(remapParams.params.key, remapParams.params.flags);

  // ======================================================================
  // normal remapping
#include "config/output/include.remapcode_call.cpp"

  general_capslock_led_hack(remapParams);
  remap_keypadnumlock_togglekey_clear(remapParams);
  remap_jis_jansi(remapParams);

  remapParams.params.flags = FlagStatus::makeFlags();
}

void
org_pqrs_KeyRemap4MacBook::remap_consumer(const RemapConsumerParams& remapParams)
{
  FlagStatus::set();

#include "config/output/include.remapcode_call_consumer.cpp"

  remapParams.params.flags = FlagStatus::makeFlags();
}

void
org_pqrs_KeyRemap4MacBook::remap_pointing_relative_core(const RemapPointingParams_relative& remapParams)
{
  FlagStatus::set();

  remapParams.params.buttons.add(RemapUtil::getRemappedButtons());

#include "config/output/include.remapcode_call_pointing_relative.cpp"
  remap_pointing_relative_to_scroll(remapParams);
}
