#include <sys/systm.h>

#include "remap.hpp"
#include "RemapUtil.hpp"
#include "Config.hpp"
#include "keycode.hpp"
#include "util/PointingButtonStatus.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace GeneratedCode {
#include "config/output/include.remapcode_func.cpp"
  }

  // ----------------------------------------
  static void
  remap_enter2commandL_enter2controlL_vm(const RemapParams &remapParams)
  {
    if (! config.remap_enter2commandL_enter2controlL_vm) return;

    if (remapParams.workspacedata.type == KeyRemap4MacBook_bridge::GetWorkspaceData::VIRTUALMACHINE) {
      static RemapUtil::KeyToKey keytokey;
      keytokey.remap(remapParams, KeyCode::ENTER, KeyCode::CONTROL_L);
    } else {
      static RemapUtil::KeyToKey keytokey;
      keytokey.remap(remapParams, KeyCode::ENTER, KeyCode::COMMAND_L);
    }
  }

  // ----------------------------------------
  static void
  remap_f6_to_numlock(const RemapParams &remapParams)
  {
    if (! config.remap_f6_to_numlock) return;

    if (! RemapUtil::isKey(remapParams, KeyCode::F6)) return;

    if (RemapUtil::isKeyDown(remapParams, KeyCode::F6)) {
      static bool flag = true;
      if (flag) {
        FlagStatus::lock_increase(ModifierFlag::FN);
      } else {
        FlagStatus::lock_decrease(ModifierFlag::FN);
      }
      flag = ! flag;
    }

    RemapUtil::drop(remapParams);
  }

  // ----------------------------------------
  static void
  remap_tab2option_withControlL(const RemapParams &remapParams)
  {
    if (! config.remap_tab2option_withControlL) return;

    if (! FlagStatus::isHeldDown(ModifierFlag::CONTROL_L)) return;

    static KeyOverlaidModifier kom;
    kom.remap(remapParams, KeyCode::TAB, ModifierFlag::OPTION_L, FireFunc::firefunc_tab);
  }

  // ----------------------------------------
  static void
  remap_keypadnumlock_togglekey_clear(const RemapParams &remapParams)
  {
    if (! config.option_keypadnumlock_togglekey_clear) return;

    if (! RemapUtil::isKey(remapParams, KeyCode::KEYPAD_CLEAR)) return;

    if (RemapUtil::isKeyDown(remapParams, KeyCode::KEYPAD_CLEAR)) {
      config.remap_keypadnumlock = ! config.remap_keypadnumlock;
    }
    RemapUtil::drop(remapParams);
  }

  // ----------------------------------------
  static void
  remap_jis_eisuu2commandL_eisuu_eisuu2optionL_term(const RemapParams &remapParams)
  {
    if (! config.remap_jis_eisuu2commandL_eisuu_eisuu2optionL_term) return;

    FireFunc::FireFunc func = FireFunc::firefunc_jis_eisuu;
    if (remapParams.workspacedata.type == KeyRemap4MacBook_bridge::GetWorkspaceData::VIRTUALMACHINE) func = FireFunc::firefunc_nop;

    static KeyOverlaidModifier kom;
    if (remapParams.workspacedata.type == KeyRemap4MacBook_bridge::GetWorkspaceData::TERMINAL) {
      kom.remap(remapParams, KeyCode::JIS_EISUU, ModifierFlag::OPTION_L, func);
    } else {
      kom.remap(remapParams, KeyCode::JIS_EISUU, ModifierFlag::COMMAND_L, func);
    }
  }

  // ----------------------------------------
  static void
  remap_jis_underscore2backslash(const RemapParams &remapParams)
  {
    if (! config.remap_jis_underscore2backslash) return;

    static RemapUtil::KeyToKey keytokey;
    if (keytokey.remap(remapParams, KeyCode::JIS_UNDERSCORE, ModifierFlag::NONE, KeyCode::BACKSLASH)) {
      (remapParams.params)->keyboardType = KeyboardType::MACBOOK;
    }
  }

  // ----------------------------------------
  static void
  remap_jis_yen2backslash(const RemapParams &remapParams)
  {
    if (! config.remap_jis_yen2backslash) return;

    static RemapUtil::KeyToKey keytokey;
    if (keytokey.remap(remapParams, KeyCode::JIS_YEN, KeyCode::BACKSLASH)) {
      (remapParams.params)->keyboardType = KeyboardType::MACBOOK;
    }
  }

  // ----------------------------------------
  static void
  remap_jis_shiftSpace2toggle_kana_eisuu(const RemapParams &remapParams)
  {
    if (! config.remap_jis_shiftSpace2toggle_kana_eisuu) return;

    if (config.option_jis_shiftSpace2toggle_kana_eisuu_disable_emacs && remapParams.workspacedata.type == KeyRemap4MacBook_bridge::GetWorkspaceData::EMACS) return;

    if (! RemapUtil::isKey(remapParams, KeyCode::SPACE)) return;

    if (FlagStatus::isHeldDown(ModifierFlag::SHIFT_L) || FlagStatus::isHeldDown(ModifierFlag::SHIFT_R)) {
      FlagStatus::temporaryDecrease_shift();

      if (RemapUtil::isEvent_Down(remapParams)) {
        FireFunc::firefunc_jis_toggle_eisuu_kana(remapParams);
      }
      RemapUtil::drop(remapParams);
    }
  }

  static void
  remap_jis_app_vi_eisuu2eisuu_escape(const RemapParams &remapParams)
  {
    if (! config.remap_jis_app_vi_eisuu2eisuu_escape) return;

    if (remapParams.workspacedata.type != KeyRemap4MacBook_bridge::GetWorkspaceData::VI) return;

    if (RemapUtil::isKeyDown(remapParams, KeyCode::JIS_EISUU)) {
      ListFireExtraKey::addKey(0, KeyCode::ESCAPE);
    }
  }

  static void
  remap_jis_jansi(const RemapParams &remapParams)
  {
    if (! config.remap_jis_jansi) return;

    (remapParams.params)->keyboardType = KeyboardType::MACBOOK;

    {
      static RemapUtil::KeyToKey keytokey;
      keytokey.remap(remapParams, KeyCode::JIS_YEN, KeyCode::BACKQUOTE);
    }
    {
      static RemapUtil::KeyToKey keytokey;
      keytokey.remap(remapParams, KeyCode::JIS_UNDERSCORE, KeyCode::BACKQUOTE);
    }
  }

  static void remap_pointing_relative_to_scroll(const RemapPointingParams_relative &remapParams) {
    if (! config.remap_pointing_relative_to_scroll) return;

    RemapUtil::pointingRelativeToScroll(remapParams);
  }
}

// ----------------------------------------------------------------------
void
org_pqrs_KeyRemap4MacBook::remap_core(const RemapParams &remapParams)
{
  FlagStatus::set(remapParams);

  // ======================================================================
  // normal remapping
#include "config/output/include.remapcode_call.cpp"

  remap_enter2commandL_enter2controlL_vm(remapParams);

  remap_f6_to_numlock(remapParams);

  remap_tab2option_withControlL(remapParams);

  // ----------------------------------------
  remap_keypadnumlock_togglekey_clear(remapParams);

  // ------------------------------------------------------------
  // jis
  remap_jis_shiftSpace2toggle_kana_eisuu(remapParams);

  remap_jis_underscore2backslash(remapParams);
  remap_jis_yen2backslash(remapParams);

  remap_jis_app_vi_eisuu2eisuu_escape(remapParams);

  remap_jis_jansi(remapParams);

  // ------------------------------------------------------------
  // *** Note: we need to call remap_space2shift, remap_enter2optionL_commandSpace (has SandS like behavior) as possible late. ***
  // *** If any key2modifier or modifier2key remappings are enabled, miss-cancelling are occured. ***
#include "config/output/include.remapcode_call_kom.cpp"
  remap_jis_eisuu2commandL_eisuu_eisuu2optionL_term(remapParams);

  // ------------------------------------------------------------
#include "config/output/include.remapcode_call_mhkk.cpp"

  // ------------------------------------------------------------
  // *** we need to handle KeyOverlaidModifierCombination at last. ***
#include "config/output/include.remapcode_call_komc.cpp"

  // ------------------------------------------------------------
  (remapParams.params)->flags = FlagStatus::makeFlags(remapParams);
}

void
org_pqrs_KeyRemap4MacBook::remap_consumer(const RemapConsumerParams &remapParams)
{
  FlagStatus::set();

#include "config/output/include.remapcode_call_consumer.cpp"

  (remapParams.params)->flags = FlagStatus::makeFlags(*(remapParams.ex_remapKeyCode));
}

void
org_pqrs_KeyRemap4MacBook::remap_pointing_relative_core(const RemapPointingParams_relative &remapParams)
{
  FlagStatus::set();

  (remapParams.params)->buttons |= PointingButtonStatus::get();

  if ((remapParams.params)->buttons != PointingButton::NONE) {
    ClickWatcher::click();
  }

#include "config/output/include.remapcode_call_pointing_relative.cpp"
  remap_pointing_relative_to_scroll(remapParams);
}
