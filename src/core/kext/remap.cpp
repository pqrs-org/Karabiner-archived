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

    if (remapParams.appType == KeyRemap4MacBook_bridge::GetWorkspaceData::VIRTUALMACHINE) {
      RemapUtil::keyToKey(remapParams, KeyCode::ENTER, KeyCode::CONTROL_L);
    } else {
      RemapUtil::keyToKey(remapParams, KeyCode::ENTER, KeyCode::COMMAND_L);
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

    (remapParams.params)->key = KeyCode::NONE;
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
  remap_emacsmode(const RemapParams &remapParams)
  {
    bool is_terminal = false;
    if (remapParams.appType == KeyRemap4MacBook_bridge::GetWorkspaceData::EMACS) is_terminal = true;
    if (remapParams.appType == KeyRemap4MacBook_bridge::GetWorkspaceData::TERMINAL) is_terminal = true;
    bool is_virtualmachine = false;
    if (remapParams.appType == KeyRemap4MacBook_bridge::GetWorkspaceData::VIRTUALMACHINE) is_virtualmachine = true;
    if (remapParams.appType == KeyRemap4MacBook_bridge::GetWorkspaceData::REMOTEDESKTOPCONNECTION) is_virtualmachine = true;
    bool is_x11 = (remapParams.appType == KeyRemap4MacBook_bridge::GetWorkspaceData::X11);

    bool ignore = is_terminal || is_virtualmachine || is_x11;

    if (config.option_emacsmode_controlK && ! ignore) {
      static bool firstcall = true;

      if ((remapParams.params)->key != KeyCode::K) {
        firstcall = true;

      } else if (FlagStatus::isHeldDown_control()) {
        if (RemapUtil::isEvent_Down(remapParams)) {
          FireFunc::firefunc_emacsmode_controlK(remapParams, firstcall);
          firstcall = false;
          *(remapParams.ex_extraRepeatFunc) = ExtraRepeatFunc::extraRepeatFunc_emacsmode_controlK;
          *(remapParams.ex_extraRepeatFlags) = 0;
        }
        RemapUtil::drop(remapParams);
      }
    }
  }

  // ----------------------------------------
  static void
  remap_jis_eisuu2commandL_eisuu_eisuu2optionL_term(const RemapParams &remapParams)
  {
    if (! config.remap_jis_eisuu2commandL_eisuu_eisuu2optionL_term) return;

    FireFunc::FireFunc func = FireFunc::firefunc_jis_eisuu;
    if (remapParams.appType == KeyRemap4MacBook_bridge::GetWorkspaceData::VIRTUALMACHINE) func = FireFunc::firefunc_nop;

    static KeyOverlaidModifier kom;
    if (remapParams.appType == KeyRemap4MacBook_bridge::GetWorkspaceData::TERMINAL) {
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

    if (remapParams.ex_origKey != KeyCode::JIS_UNDERSCORE) return;

    if (! FlagStatus::isHeldDown_shift()) {
      // hack to fire "the true backslash (not yen)" on JIS Keyboard.
      (remapParams.params)->keyboardType = KeyboardType::MACBOOK;
      RemapUtil::keyToKey(remapParams, KeyCode::JIS_UNDERSCORE, KeyCode::BACKSLASH);
    }
  }

  // ----------------------------------------
  static void
  remap_jis_yen2backslash(const RemapParams &remapParams)
  {
    if (! config.remap_jis_yen2backslash) return;

    if (remapParams.ex_origKey == KeyCode::JIS_YEN) {
      // hack to fire "the true backslash (not yen)" on JIS Keyboard.
      (remapParams.params)->keyboardType = KeyboardType::MACBOOK;
      RemapUtil::keyToKey(remapParams, KeyCode::JIS_YEN, KeyCode::BACKSLASH);
    }
  }

  // ----------------------------------------
  static void
  remap_jis_shiftSpace2toggle_kana_eisuu(const RemapParams &remapParams)
  {
    if (! config.remap_jis_shiftSpace2toggle_kana_eisuu) return;

    if (config.option_jis_shiftSpace2toggle_kana_eisuu_disable_emacs && remapParams.appType == KeyRemap4MacBook_bridge::GetWorkspaceData::EMACS) return;

    if (! RemapUtil::isKey(remapParams, KeyCode::SPACE)) return;

    if (FlagStatus::isHeldDown_shift()) {
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

    if (remapParams.appType != KeyRemap4MacBook_bridge::GetWorkspaceData::VI) return;

    if (RemapUtil::isKeyDown(remapParams, KeyCode::JIS_EISUU)) {
      ListFireExtraKey::addKey(0, KeyCode::ESCAPE);
    }
  }

  static void
  remap_jis_jansi(const RemapParams &remapParams)
  {
    if (! config.remap_jis_jansi) return;

    (remapParams.params)->keyboardType = KeyboardType::MACBOOK;
    RemapUtil::keyToKey(remapParams, KeyCode::JIS_YEN, KeyCode::BACKQUOTE);
    RemapUtil::keyToKey(remapParams, KeyCode::JIS_UNDERSCORE, KeyCode::BACKQUOTE);
  }

  static void
  remap_jis_layout_kawashima(const RemapParams &remapParams)
  {
    if (! config.remap_jis_layout_kawashima) return;

    if (JISKanaMode::getMode() != JISKanaMode::JISKANAMODE_HIRAGANA) return;

    // A -> TA ( Q )
    // A+Shift_L -> NU ( 1 )
    // A+Shift_R -> DA ( Q[ )
    RemapUtil::keyToKey_dependingShift(remapParams, KeyCode::A,
                                       KeyCode::Q, CharCode::Q, KeyCode::NONE, CharCode::NONE,
                                       KeyCode::KEY_1, CharCode::KEY_1, KeyCode::NONE, CharCode::NONE,
                                       KeyCode::Q, CharCode::Q, KeyCode::BRACKET_LEFT, CharCode::BRACKET_LEFT);

    // J -> I ( E )
    // J+Shift_L -> DI ( A[ )
    // J+Shift_R -> TI ( A )
    RemapUtil::keyToKey_dependingShift(remapParams, KeyCode::J,
                                       KeyCode::E, CharCode::E, KeyCode::NONE, CharCode::NONE,
                                       KeyCode::A, CharCode::A, KeyCode::BRACKET_LEFT, CharCode::BRACKET_LEFT,
                                       KeyCode::A, CharCode::A, KeyCode::NONE, CharCode::NONE);
  }

  // ------------------------------------------------------------
  static void
  remap_pointing_relative_fn_to_scroll(const RemapPointingParams_relative &remapParams)
  {
    if (! config.remap_pointing_relative_fn_to_scroll) return;

    if (! FlagStatus::isHeldDown(ModifierFlag::FN)) return;

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
  remap_jis_layout_kawashima(remapParams);

  // ------------------------------------------------------------
  // *** Note: we need to call remap_emacsmode as possible late. ***
  // *** If qwerty2colemak is enabled, Control+H... works with Colemak Keyboard Layout. ***
  remap_emacsmode(remapParams);

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
  (remapParams.params)->buttons |= PointingButtonStatus::get();

  if ((remapParams.params)->buttons != PointingButton::NONE) {
    ClickWatcher::click();
  }

  remap_pointing_relative_fn_to_scroll(remapParams);
#include "config/output/include.remapcode_call_pointing_relative.cpp"
}
