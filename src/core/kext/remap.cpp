#include <sys/systm.h>

#include "remap.hpp"
#include "RemapUtil.hpp"
#include "Config.hpp"
#include "keycode.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace GeneratedCode {
#include "config/output/include.remapcode_func.cpp"
  }

  // ----------------------------------------
  void
  remap_enter2commandL_enter2controlL_vm(const RemapParams &params)
  {
    if (! config.remap_enter2commandL_enter2controlL_vm) return;

    if (params.appType == KeyRemap4MacBook_bridge::ActiveApplicationInfo::VIRTUALMACHINE) {
      RemapUtil::keyToKey(params, KeyCode::ENTER, KeyCode::CONTROL_L);
    } else {
      RemapUtil::keyToKey(params, KeyCode::ENTER, KeyCode::COMMAND_L);
    }
  }

  // ----------------------------------------
  void
  remap_fn2controlL_commandR2fn(const RemapParams &params)
  {
    if (! config.remap_fn2controlL_commandR2fn) return;

    if (RemapUtil::isModifierOn(params, ModifierFlag::FN)) {
      if (allFlagStatus.commandR.isHeldDown()) {
        allFlagStatus.commandR.temporary_decrease();
        allFlagStatus.controlL.temporary_decrease();
        return;
      }
    }

    RemapUtil::keyToKey(params, KeyCode::FN, KeyCode::CONTROL_L);
  }

  // ----------------------------------------
  void
  remap_semicolon2return_controlsemicolon2semicolon(const RemapParams &params)
  {
    if (! config.remap_semicolon2return_controlsemicolon2semicolon) return;

    if (! allFlagStatus.isHeldDown_control() && ! allFlagStatus.isHeldDown_shift()) {
      RemapUtil::keyToKey(params, KeyCode::SEMICOLON, KeyCode::RETURN);
    } else {
      allFlagStatus.temporaryDecrease_control();
    }
  }

  void
  remap_swapcolons(const RemapParams &params)
  {
    if (! config.remap_swapcolons) return;

    if (! RemapUtil::isKey(params, KeyCode::SEMICOLON)) return;

    if (allFlagStatus.isHeldDown_shift()) {
      allFlagStatus.temporaryDecrease_shift();
    } else {
      allFlagStatus.shiftL.temporary_increase();
    }
  }

  // ----------------------------------------
  void
  remap_shiftRshiftL2space(const RemapParams &params)
  {
    if (! config.remap_shiftRshiftL2space) return;

    if (RemapUtil::isKey(params, KeyCode::SHIFT_L)) {
      if (RemapUtil::isModifierOn(params, ModifierFlag::SHIFT_R)) {
        allFlagStatus.shiftR.temporary_decrease();
        RemapUtil::keyToKey(params, KeyCode::SHIFT_L, KeyCode::SPACE);
      }
    } else if (RemapUtil::isKey(params, KeyCode::SHIFT_R)) {
      if (RemapUtil::isModifierOn(params, ModifierFlag::SHIFT_L)) {
        RemapUtil::keyToKey(params, KeyCode::SHIFT_R, KeyCode::SPACE);
      }
    }
  }

  // ----------------------------------------
  void
  remap_shiftLshiftR2space(const RemapParams &params)
  {
    if (! config.remap_shiftLshiftR2space) return;

    if (RemapUtil::isKey(params, KeyCode::SHIFT_L)) {
      if (RemapUtil::isModifierOn(params, ModifierFlag::SHIFT_R)) {
        RemapUtil::keyToKey(params, KeyCode::SHIFT_L, KeyCode::SPACE);
      }
    } else if (RemapUtil::isKey(params, KeyCode::SHIFT_R)) {
      if (RemapUtil::isModifierOn(params, ModifierFlag::SHIFT_L)) {
        allFlagStatus.shiftL.temporary_decrease();
        RemapUtil::keyToKey(params, KeyCode::SHIFT_R, KeyCode::SPACE);
      }
    }
  }

  // ----------------------------------------
  void
  remap_tab2option_withControlL(const RemapParams &params)
  {
    if (! config.remap_tab2option_withControlL) return;

    if (! allFlagStatus.controlL.isHeldDown()) return;

    static KeyOverlaidModifier kom;
    kom.remap(params, KeyCode::TAB, ModifierFlag::OPTION_L, FireFunc::firefunc_tab);
  }

  // ----------------------------------------
  void
  remap_pc_pause2eject(const RemapParams &params)
  {
    if (! config.remap_pc_pause2eject) return;

    RemapUtil::keyToConsumer(params, KeyCode::PC_PAUSE, ConsumerKeyCode::EJECT);
  }

  void
  remap_keypadnumlock(const RemapParams &params)
  {
    if (! config.remap_keypadnumlock) return;

    if (params.ex_origKey == KeyCode::KEYPAD_0 ||
        params.ex_origKey == KeyCode::KEYPAD_5 ||
        params.ex_origKey == KeyCode::KEYPAD_COMMA) {
      *(params.ex_dropKey) = true;
    }

    if (params.ex_origKey == KeyCode::KEYPAD_2 ||
        params.ex_origKey == KeyCode::KEYPAD_4 ||
        params.ex_origKey == KeyCode::KEYPAD_6 ||
        params.ex_origKey == KeyCode::KEYPAD_8) {
      RemapUtil::keyToKey(params, KeyCode::KEYPAD_2, KeyCode::CURSOR_DOWN);
      RemapUtil::keyToKey(params, KeyCode::KEYPAD_4, KeyCode::CURSOR_LEFT);
      RemapUtil::keyToKey(params, KeyCode::KEYPAD_6, KeyCode::CURSOR_RIGHT);
      RemapUtil::keyToKey(params, KeyCode::KEYPAD_8, KeyCode::CURSOR_UP);
    }

    if (params.ex_origKey == KeyCode::KEYPAD_1 ||
        params.ex_origKey == KeyCode::KEYPAD_3 ||
        params.ex_origKey == KeyCode::KEYPAD_7 ||
        params.ex_origKey == KeyCode::KEYPAD_9) {
      RemapUtil::keyToKey(params, KeyCode::KEYPAD_1, KeyCode::END);
      RemapUtil::keyToKey(params, KeyCode::KEYPAD_3, KeyCode::PAGEDOWN);
      RemapUtil::keyToKey(params, KeyCode::KEYPAD_7, KeyCode::HOME);
      RemapUtil::keyToKey(params, KeyCode::KEYPAD_9, KeyCode::PAGEUP);
    }

    if (params.ex_origKey == KeyCode::KEYPAD_DOT) {
      RemapUtil::keyToKey(params, KeyCode::KEYPAD_DOT, KeyCode::FORWARD_DELETE);
    }
  }

  void
  remap_keypadnumlock_togglekey_clear(const RemapParams &params)
  {
    if (! config.option_keypadnumlock_togglekey_clear) return;

    if (! RemapUtil::isKey(params, KeyCode::KEYPAD_CLEAR)) return;

    if (RemapUtil::isKeyDown(params, KeyCode::KEYPAD_CLEAR)) {
      config.remap_keypadnumlock = ! config.remap_keypadnumlock;
    }
    *(params.ex_dropKey) = true;
  }

  void
  remap_pclikehomeend(const RemapParams &params)
  {
    if (! config.remap_pclikehomeend) return;

    if (params.appType == KeyRemap4MacBook_bridge::ActiveApplicationInfo::VIRTUALMACHINE) return;
    if (params.appType == KeyRemap4MacBook_bridge::ActiveApplicationInfo::REMOTEDESKTOPCONNECTION) return;

    RemapUtil::keyToKey(params, KeyCode::HOME, KeyCode::CURSOR_LEFT, ModifierFlag::COMMAND_L);
    RemapUtil::keyToKey(params, KeyCode::END, KeyCode::CURSOR_RIGHT, ModifierFlag::COMMAND_L);
  }

  // ----------------------------------------
  void
  remap_emacsmode(const RemapParams &params)
  {
    bool is_terminal = false;
    if (params.appType == KeyRemap4MacBook_bridge::ActiveApplicationInfo::EMACS) is_terminal = true;
    if (params.appType == KeyRemap4MacBook_bridge::ActiveApplicationInfo::TERMINAL) is_terminal = true;
    bool is_virtualmachine = false;
    if (params.appType == KeyRemap4MacBook_bridge::ActiveApplicationInfo::VIRTUALMACHINE) is_virtualmachine = true;
    if (params.appType == KeyRemap4MacBook_bridge::ActiveApplicationInfo::REMOTEDESKTOPCONNECTION) is_virtualmachine = true;
    bool is_x11 = (params.appType == KeyRemap4MacBook_bridge::ActiveApplicationInfo::X11);

    bool ignore = is_terminal || is_virtualmachine || is_x11;

    if (allFlagStatus.isHeldDown_control()) {
      // Control+K -> Command+Shift+Right,Command+X
      if (config.option_emacsmode_controlK && ! ignore) {
        static bool firstcall = true;

        if (*(params.key) != KeyCode::K) {
          firstcall = true;

        } else {
          if (*(params.eventType) == KeyEvent::DOWN) {
            FireFunc::firefunc_emacsmode_controlK(params, firstcall);
            firstcall = false;
            *(params.ex_extraRepeatFunc) = ExtraRepeatFunc::extraRepeatFunc_emacsmode_controlK;
            *(params.ex_extraRepeatFlags) = 0;
          }
          *(params.ex_dropKey) = true;
          allFlagStatus.temporaryDecrease_control();
        }
      }
      // Control+U -> Command+Shift+Left,Command+X
      if (config.option_emacsmode_ex_controlU && *(params.key) == KeyCode::U && ! ignore) {
        if (*(params.eventType) == KeyEvent::DOWN) {
          FireFunc::firefunc_emacsmode_ex_controlU(params);
        }
        *(params.ex_dropKey) = true;
        allFlagStatus.temporaryDecrease_control();
      }
    }
  }

  // ----------------------------------------
  void
  remap_app_vm_enter2controlL(const RemapParams &params)
  {
    if (! config.remap_app_vm_enter2controlL) return;

    if (params.appType != KeyRemap4MacBook_bridge::ActiveApplicationInfo::VIRTUALMACHINE) return;

    RemapUtil::keyToKey(params, KeyCode::ENTER, KeyCode::CONTROL_L);
  }

  void
  remap_app_vm_commandspace2optionbackquote(const RemapParams &params)
  {
    if (! config.remap_app_vm_commandspace2optionbackquote) return;

    //  "Remote Desktop Connection" has the self remapping feature.
    // Our targets are VMware, Parallels only.
    if (params.appType != KeyRemap4MacBook_bridge::ActiveApplicationInfo::VIRTUALMACHINE) return;

    if (allFlagStatus.isHeldDown_command()) {
      if (*(params.key) == KeyCode::SPACE) {
        // hack keyboardType to fire backquote on any keyboards.
        *(params.keyboardType) = KeyboardType::MACBOOK;
        *(params.key) = KeyCode::BACKQUOTE;
        allFlagStatus.optionL.temporary_increase();
        allFlagStatus.temporaryDecrease_command();
      }
    }
  }

  // ----------------------------------------
  void
  remap_qwerty2colemak(const RemapParams &params)
  {
    if (! config.remap_qwerty2colemak) return;

    // see http://colemak.com/
    RemapUtil::keyToKey(params, KeyCode::E, KeyCode::F);
    RemapUtil::keyToKey(params, KeyCode::R, KeyCode::P);
    RemapUtil::keyToKey(params, KeyCode::T, KeyCode::G);
    RemapUtil::keyToKey(params, KeyCode::Y, KeyCode::J);
    RemapUtil::keyToKey(params, KeyCode::U, KeyCode::L);
    RemapUtil::keyToKey(params, KeyCode::I, KeyCode::U);
    RemapUtil::keyToKey(params, KeyCode::O, KeyCode::Y);
    RemapUtil::keyToKey(params, KeyCode::P, KeyCode::SEMICOLON);
    RemapUtil::keyToKey(params, KeyCode::S, KeyCode::R);
    RemapUtil::keyToKey(params, KeyCode::D, KeyCode::S);
    RemapUtil::keyToKey(params, KeyCode::F, KeyCode::T);
    RemapUtil::keyToKey(params, KeyCode::G, KeyCode::D);
    RemapUtil::keyToKey(params, KeyCode::J, KeyCode::N);
    RemapUtil::keyToKey(params, KeyCode::K, KeyCode::E);
    RemapUtil::keyToKey(params, KeyCode::L, KeyCode::I);
    RemapUtil::keyToKey(params, KeyCode::SEMICOLON, KeyCode::O);
    RemapUtil::keyToKey(params, KeyCode::N, KeyCode::K);
  }

  void
  remap_qwerty2qwerf(const RemapParams &params)
  {
    if (! config.remap_qwerty2qwerf) return;

    // Q W E R F J Y K L ;
    // A S D T G H U I O P
    // Z X C V B N M , . /
    RemapUtil::keyToKey(params, KeyCode::T, KeyCode::F);
    RemapUtil::keyToKey(params, KeyCode::Y, KeyCode::J);
    RemapUtil::keyToKey(params, KeyCode::U, KeyCode::Y);
    RemapUtil::keyToKey(params, KeyCode::I, KeyCode::K);
    RemapUtil::keyToKey(params, KeyCode::O, KeyCode::L);
    RemapUtil::keyToKey(params, KeyCode::P, KeyCode::SEMICOLON);

    RemapUtil::keyToKey(params, KeyCode::F, KeyCode::T);
    RemapUtil::keyToKey(params, KeyCode::J, KeyCode::U);
    RemapUtil::keyToKey(params, KeyCode::K, KeyCode::I);
    RemapUtil::keyToKey(params, KeyCode::L, KeyCode::O);
    RemapUtil::keyToKey(params, KeyCode::SEMICOLON, KeyCode::P);
  }

  // ----------------------------------------
  void
  remap_jis_eisuu2commandL_eisuu_eisuu2optionL_term(const RemapParams &params)
  {
    if (! config.remap_jis_eisuu2commandL_eisuu_eisuu2optionL_term) return;

    FireFunc::FireFunc func = FireFunc::firefunc_jis_eisuu;
    if (params.appType == KeyRemap4MacBook_bridge::ActiveApplicationInfo::VIRTUALMACHINE) func = FireFunc::firefunc_nop;

    static KeyOverlaidModifier kom;
    if (params.appType == KeyRemap4MacBook_bridge::ActiveApplicationInfo::TERMINAL) {
      kom.remap(params, KeyCode::JIS_EISUU, ModifierFlag::OPTION_L, func);
    } else {
      kom.remap(params, KeyCode::JIS_EISUU, ModifierFlag::COMMAND_L, func);
    }
  }

  // ----------------------------------------
  void
  remap_jis_underscore2backslash(const RemapParams &params)
  {
    if (! config.remap_jis_underscore2backslash) return;

    if (params.ex_origKey != KeyCode::JIS_UNDERSCORE) return;

    if (! allFlagStatus.isHeldDown_shift()) {
      // hack to fire "the true backslash (not yen)" on JIS Keyboard.
      *(params.keyboardType) = KeyboardType::MACBOOK;
      RemapUtil::keyToKey(params, KeyCode::JIS_UNDERSCORE, KeyCode::BACKSLASH);
    }
  }

  // ----------------------------------------
  void
  remap_jis_yen2backslash(const RemapParams &params)
  {
    if (! config.remap_jis_yen2backslash) return;

    if (params.ex_origKey == KeyCode::JIS_YEN) {
      // hack to fire "the true backslash (not yen)" on JIS Keyboard.
      *(params.keyboardType) = KeyboardType::MACBOOK;
      RemapUtil::keyToKey(params, KeyCode::JIS_YEN, KeyCode::BACKSLASH);
    }
  }

  // ----------------------------------------
  void
  remap_jis_shiftKana2eisuu(const RemapParams &params)
  {
    if (! config.remap_jis_shiftKana2eisuu) return;

    if (params.ex_origKey != KeyCode::JIS_KANA) return;

    if (allFlagStatus.isHeldDown_shift()) {
      allFlagStatus.temporaryDecrease_shift();
      RemapUtil::keyToKey(params, KeyCode::JIS_KANA, KeyCode::JIS_EISUU);
    }
  }

  void
  remap_jis_shiftSpace2toggle_kana_eisuu(const RemapParams &params)
  {
    if (! config.remap_jis_shiftSpace2toggle_kana_eisuu) return;

    if (config.option_jis_shiftSpace2toggle_kana_eisuu_disable_emacs && params.appType == KeyRemap4MacBook_bridge::ActiveApplicationInfo::EMACS) return;

    if (! RemapUtil::isKey(params, KeyCode::SPACE)) return;

    if (allFlagStatus.isHeldDown_shift()) {
      allFlagStatus.temporaryDecrease_shift();

      if (*(params.eventType) == KeyEvent::DOWN) {
        FireFunc::firefunc_jis_toggle_eisuu_kana(params);
      }
      *(params.ex_dropKey) = true;
    }
  }

  void
  remap_jis_app_vi_eisuu2eisuu_escape(const RemapParams &params)
  {
    if (! config.remap_jis_app_vi_eisuu2eisuu_escape) return;

    if (params.appType != KeyRemap4MacBook_bridge::ActiveApplicationInfo::VI) return;

    if (params.ex_origKey == KeyCode::JIS_EISUU) {
      if (*(params.eventType) == KeyEvent::DOWN) {
        FireFunc::firefunc_jis_eisuu_escape(params);
      }
      *(params.ex_dropKey) = true;
    }
  }

  void
  remap_jis_jansi(const RemapParams &params)
  {
    if (! config.remap_jis_jansi) return;

    *(params.keyboardType) = KeyboardType::MACBOOK;
    RemapUtil::keyToKey(params, KeyCode::JIS_YEN, KeyCode::BACKQUOTE);
    RemapUtil::keyToKey(params, KeyCode::JIS_UNDERSCORE, KeyCode::BACKQUOTE);
  }

  void
  remap_jis_layout_kawashima(const RemapParams &params)
  {
    if (! config.remap_jis_layout_kawashima) return;

    if (jisKanaMode.getMode() != JISKanaMode::JISKANAMODE_HIRAGANA) return;

    // A -> TA ( Q )
    // A+Shift_L -> NU ( 1 )
    // A+Shift_R -> DA ( Q[ )
    RemapUtil::keyToKey_dependingShift(params, KeyCode::A,
                                       KeyCode::Q, CharCode::Q, KeyCode::NONE, CharCode::NONE,
                                       KeyCode::KEY_1, CharCode::KEY_1, KeyCode::NONE, CharCode::NONE,
                                       KeyCode::Q, CharCode::Q, KeyCode::BRACKET_LEFT, CharCode::BRACKET_LEFT);

    // J -> I ( E )
    // J+Shift_L -> DI ( A[ )
    // J+Shift_R -> TI ( A )
    RemapUtil::keyToKey_dependingShift(params, KeyCode::J,
                                       KeyCode::E, CharCode::E, KeyCode::NONE, CharCode::NONE,
                                       KeyCode::A, CharCode::A, KeyCode::BRACKET_LEFT, CharCode::BRACKET_LEFT,
                                       KeyCode::A, CharCode::A, KeyCode::NONE, CharCode::NONE);
  }

  void
  remap_jis_layout_windowskanainput(const RemapParams &params)
  {
    if (! config.remap_jis_layout_windowskanainput) return;

    if (jisKanaMode.getMode() == JISKanaMode::JISKANAMODE_ASCII) return;

    if (allFlagStatus.isHeldDown_shift()) {
      RemapUtil::keyToKey(params, KeyCode::MINUS, KeyCode::BRACKET_RIGHT);
      RemapUtil::keyToKey(params, KeyCode::BRACKET_RIGHT, KeyCode::BRACKET_LEFT);
      RemapUtil::keyToKey(params, KeyCode::BRACKET_LEFT, KeyCode::EQUAL);
      RemapUtil::keyToKey(params, KeyCode::EQUAL, KeyCode::MINUS);
    } else {
      RemapUtil::keyToKey(params, KeyCode::BRACKET_RIGHT, KeyCode::EQUAL);
      RemapUtil::keyToKey(params, KeyCode::EQUAL, KeyCode::BACKSLASH);
      RemapUtil::keyToKey(params, KeyCode::BACKSLASH, KeyCode::BRACKET_RIGHT);
    }
  }

  // ------------------------------------------------------------
  void
  remap_eject2forwarddelete(const RemapConsumerParams &params)
  {
    if (! config.remap_eject2forwarddelete) return;

    RemapUtil::ejectToKey(params, KeyCode::FORWARD_DELETE);
  }

  void
  remap_eject2pagedown(const RemapConsumerParams &params)
  {
    if (! config.remap_eject2pagedown) return;

    RemapUtil::ejectToKey(params, KeyCode::PAGEDOWN);
  }

  // ------------------------------------------------------------
  void
  remap_pointing_relative_fn_to_scroll(const RemapPointingParams_relative &params)
  {
    if (! config.remap_pointing_relative_fn_to_scroll) return;

    if (! allFlagStatus.fn.isHeldDown()) return;

    RemapUtil::pointingRelativeToScroll(params);
  }
}

// ----------------------------------------------------------------------
void
org_pqrs_KeyRemap4MacBook::remap_core(const RemapParams &params)
{
  if (config.debug) {
    printf("caught hid event type %d flags 0x%x key %d ",  *(params.eventType), *(params.flags), *(params.key));
    printf("charCode %d charSet %d ", *(params.charCode), *(params.charSet));
    printf("origCharCode %d origCharSet %d kbdType %d\n",
           *(params.origCharCode), *(params.origCharSet), *(params.keyboardType));
  }

  allFlagStatus.initialize(params);

  // ======================================================================
  // normal remapping
#include "config/output/include.remapcode_call.cpp"

  remap_enter2commandL_enter2controlL_vm(params);

  remap_fn2controlL_commandR2fn(params);

  remap_semicolon2return_controlsemicolon2semicolon(params);
  remap_swapcolons(params);

  remap_shiftLshiftR2space(params);
  remap_shiftRshiftL2space(params);

  remap_tab2option_withControlL(params);

  // ----------------------------------------
  remap_app_vm_enter2controlL(params);
  remap_app_vm_commandspace2optionbackquote(params);

  // ----------------------------------------
  remap_qwerty2colemak(params);
  remap_qwerty2qwerf(params);

  // ----------------------------------------
  remap_pc_pause2eject(params);
  remap_keypadnumlock(params);
  remap_keypadnumlock_togglekey_clear(params);

  // ------------------------------------------------------------
  // jis
  remap_jis_shiftSpace2toggle_kana_eisuu(params);

  remap_jis_shiftKana2eisuu(params);

  remap_jis_underscore2backslash(params);
  remap_jis_yen2backslash(params);

  remap_jis_app_vi_eisuu2eisuu_escape(params);

  remap_jis_jansi(params);
  remap_jis_layout_kawashima(params);
  remap_jis_layout_windowskanainput(params);

  // ------------------------------------------------------------
  // *** Note: we need to call remap_emacsmode as possible late. ***
  // *** If qwerty2colemak is enabled, Control+H... works with Colemak Keyboard Layout. ***
  remap_emacsmode(params);

  // ------------------------------------------------------------
  // *** Note: we need to call remap_space2shift, remap_enter2optionL_commandSpace (has SandS like behavior) as possible late. ***
  // *** If any key2modifier or modifier2key remappings are enabled, miss-cancelling are occured. ***
#include "config/output/include.remapcode_call_kom.cpp"
  remap_jis_eisuu2commandL_eisuu_eisuu2optionL_term(params);

  // ------------------------------------------------------------
  // *** Note: we need to call remap_pclikehomeend as possible late. ***
  // *** If remap_emacsmode is enable, C-1 & C-2 replaced as HOME, END. ***
  remap_pclikehomeend(params);

  // ------------------------------------------------------------
#include "config/output/include.remapcode_call_mhkk.cpp"

  // ------------------------------------------------------------
  // *** we need to handle KeyOverlaidModifierCombination at last. ***
#include "config/output/include.remapcode_call_komc.cpp"

  // ------------------------------------------------------------
  *(params.flags) = allFlagStatus.makeFlags(params);
}

void
org_pqrs_KeyRemap4MacBook::remap_consumer(const RemapConsumerParams &params)
{
  remap_eject2forwarddelete(params);
  remap_eject2pagedown(params);
#include "config/output/include.remapcode_call_consumer.cpp"
}

void
org_pqrs_KeyRemap4MacBook::remap_pointing_relative_core(const RemapPointingParams_relative &params)
{
  if (pointingButtonStatus.helddown_left) {
    *(params.buttons) |= PointingButton::LEFT;
  }
  if (pointingButtonStatus.helddown_right) {
    *(params.buttons) |= PointingButton::RIGHT;
  }
  if (pointingButtonStatus.helddown_middle) {
    *(params.buttons) |= PointingButton::MIDDLE;
  }

  if (*(params.buttons) != PointingButton::NONE) {
    clickWatcher.click();
  }

  remap_pointing_relative_fn_to_scroll(params);
#include "config/output/include.remapcode_call_pointing_relative.cpp"
}
