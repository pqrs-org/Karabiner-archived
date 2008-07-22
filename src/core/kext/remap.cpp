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
  remap_backquote2escape_withoutmodifiers(const RemapParams &params)
  {
    if (! config.remap_backquote2escape_withoutmodifiers) return;

    unsigned int flags = allFlagStatus.makeFlags(params);
    if (flags == 0) {
      RemapUtil::keyToKey(params, KeyCode::BACKQUOTE, KeyCode::ESCAPE);
    }
  }

  // ----------------------------------------
  void
  remap_delete2f13_shift2tilde(const RemapParams &params)
  {
    if (! config.remap_delete2f13_shift2tilde) return;

    if (allFlagStatus.isHeldDown_shift()) {
      RemapUtil::keyToKey(params, KeyCode::DELETE, KeyCode::BACKQUOTE);
    } else {
      RemapUtil::keyToKey(params, KeyCode::DELETE, KeyCode::F13);
    }
  }

  void
  remap_shiftDelete2tilde(const RemapParams &params)
  {
    if (! config.remap_shiftDelete2tilde) return;

    if (allFlagStatus.isHeldDown_shift()) {
      RemapUtil::keyToKey(params, KeyCode::DELETE, KeyCode::BACKQUOTE);
    }
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

  void
  remap_fn2fn(const RemapParams &params)
  {
    if (! config.remap_fn2fn) return;

    RemapUtil::toFN(params);
  }

  // ----------------------------------------
  namespace {
    void
    handle_f1_f5_to_consumer(const RemapParams &params, KeyCode::KeyCode fromKeyCode, ConsumerKeyCode::ConsumerKeyCode toKeyCode)
    {
      unsigned int flags = allFlagStatus.makeFlags(params);
      if (flags != 0) return;

      bool is_virtualmachine = false;
      if (params.appType == KeyRemap4MacBook_bridge::ActiveApplicationInfo::VIRTUALMACHINE) is_virtualmachine = true;
      if (params.appType == KeyRemap4MacBook_bridge::ActiveApplicationInfo::REMOTEDESKTOPCONNECTION) is_virtualmachine = true;

      if (is_virtualmachine && ! config.option_f1_f5_force_vm) return;

      RemapUtil::keyToConsumer(params, fromKeyCode, toKeyCode);
    }
  }

  void
  remap_f1_to_brightnessdown(const RemapParams &params)
  {
    if (! config.remap_f1_to_brightnessdown) return;

    handle_f1_f5_to_consumer(params, KeyCode::F1, ConsumerKeyCode::BRIGHTNESS_DOWN);
  }

  void
  remap_f2_to_brightnessup(const RemapParams &params)
  {
    if (! config.remap_f2_to_brightnessup) return;

    handle_f1_f5_to_consumer(params, KeyCode::F2, ConsumerKeyCode::BRIGHTNESS_UP);
  }

  void
  remap_f3_to_volumemute(const RemapParams &params)
  {
    if (! config.remap_f3_to_volumemute) return;

    handle_f1_f5_to_consumer(params, KeyCode::F3, ConsumerKeyCode::VOLUME_MUTE);
  }

  void
  remap_f4_to_volumedown(const RemapParams &params)
  {
    if (! config.remap_f4_to_volumedown) return;

    handle_f1_f5_to_consumer(params, KeyCode::F4, ConsumerKeyCode::VOLUME_DOWN);
  }

  void
  remap_f5_to_volumeup(const RemapParams &params)
  {
    if (! config.remap_f5_to_volumeup) return;

    handle_f1_f5_to_consumer(params, KeyCode::F5, ConsumerKeyCode::VOLUME_UP);
  }

  // ----------------------------------------
  void
  remap_optionR2allF1(const RemapParams &params)
  {
    if (! config.remap_optionR2allF1) return;

    if (! RemapUtil::isKey(params, KeyCode::OPTION_R)) return;

    KeyEvent::KeyEvent eventType = KeyEvent::DOWN;
    if (! allFlagStatus.optionR.isHeldDown()) {
      eventType = KeyEvent::UP;
    }

    RemapUtil::fireKeyWithModifiers(params,
                                    (ModifierFlag::COMMAND_L | ModifierFlag::CONTROL_L | ModifierFlag::OPTION_L | ModifierFlag::SHIFT_L),
                                    eventType, KeyCode::F1, CharCode::F1);
    *(params.ex_dropKey) = true;
  }

  // ----------------------------------------
  void
  remap_semicolon2return_controlsemicolon2semicolon(const RemapParams &params)
  {
    if (! config.remap_semicolon2return_controlsemicolon2semicolon) return;

    static ModifierCanceling mc_controlL;
    if (! allFlagStatus.isHeldDown_control() && ! allFlagStatus.isHeldDown_shift()) {
      RemapUtil::keyToKey(params, KeyCode::SEMICOLON, KeyCode::RETURN);

    } else {
      bool cancel_control = false;
      if (*(params.key) == KeyCode::SEMICOLON) {
        *(params.key) = KeyCode::SEMICOLON;
        cancel_control = true;
      }

      if (cancel_control) {
        if (allFlagStatus.controlL.isHeldDown()) {
          mc_controlL.keyRelease(params, ModifierFlag::CONTROL_L);
        }
      }
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

  void
  remap_commandTab2optionTab(const RemapParams &params)
  {
    if (! config.remap_commandTab2optionTab) return;

    if (params.ex_origKey != KeyCode::TAB) return;

    if (allFlagStatus.isHeldDown_command()) {
      allFlagStatus.temporaryDecrease_command();
      allFlagStatus.optionL.temporary_increase();
    }
  }

  void
  remap_commandTab2f5(const RemapParams &params)
  {
    if (! config.remap_commandTab2f5) return;

    static KeyWithModifierToKey kwmk_l;
    static KeyWithModifierToKey kwmk_r;

    if (kwmk_l.remap(params, KeyCode::TAB, ModifierFlag::COMMAND_L, KeyCode::F5)) return;
    if (kwmk_r.remap(params, KeyCode::TAB, ModifierFlag::COMMAND_R, KeyCode::F5)) return;
  }

  void
  remap_optionTab2f5(const RemapParams &params)
  {
    if (! config.remap_optionTab2f5) return;

    static KeyWithModifierToKey kwmk_l;
    static KeyWithModifierToKey kwmk_r;

    if (kwmk_l.remap(params, KeyCode::TAB, ModifierFlag::OPTION_L, KeyCode::F5)) return;
    if (kwmk_r.remap(params, KeyCode::TAB, ModifierFlag::OPTION_R, KeyCode::F5)) return;
  }

  void
  remap_optionTab2commandTab(const RemapParams &params)
  {
    if (! config.remap_optionTab2commandTab) return;

    if (params.ex_origKey != KeyCode::TAB) return;

    if (allFlagStatus.isHeldDown_option()) {
      allFlagStatus.temporaryDecrease_option();
      allFlagStatus.commandL.temporary_increase();
    }
  }

  // ----------------------------------------
  void
  remap_hhkmode(const RemapParams &params)
  {
    if (! config.remap_hhkmode) return;

    if (! allFlagStatus.fn.isHeldDown()) return;

    // [ => up
    if (params.ex_origKey == KeyCode::BRACKET_LEFT) {
      *(params.key) = KeyCode::CURSOR_UP;
      allFlagStatus.fn.temporary_decrease();
    }
    // ; => left
    if (params.ex_origKey == KeyCode::SEMICOLON || params.ex_origKey == KeyCode::KEYPAD_MINUS) {
      *(params.key) = KeyCode::CURSOR_LEFT;
      allFlagStatus.fn.temporary_decrease();
    }
    // ' => right
    if (params.ex_origKey == KeyCode::QUOTE) {
      *(params.key) = KeyCode::CURSOR_RIGHT;
      allFlagStatus.fn.temporary_decrease();
    }
    // / => down
    if (params.ex_origKey == KeyCode::SLASH || params.ex_origKey == KeyCode::KEYPAD_PLUS) {
      *(params.key) = KeyCode::CURSOR_DOWN;
      allFlagStatus.fn.temporary_decrease();
    }
    // L => PageUp
    if (params.ex_origKey == KeyCode::L || params.ex_origKey == KeyCode::KEYPAD_3) {
      *(params.key) = KeyCode::PAGEUP;
    }
    // . => PageDown
    if (params.ex_origKey == KeyCode::DOT || params.ex_origKey == KeyCode::KEYPAD_DOT) {
      *(params.key) = KeyCode::PAGEDOWN;
    }
    // K => HOME
    if (params.ex_origKey == KeyCode::K || params.ex_origKey == KeyCode::KEYPAD_2) {
      *(params.key) = KeyCode::HOME;
    }
    // , => End
    if (params.ex_origKey == KeyCode::COMMA) {
      *(params.key) = KeyCode::END;
    }

    // 1..90-= => F1..F12
    if (params.ex_origKey == KeyCode::KEY_1) {
      *(params.key) = KeyCode::F1;
    }
    if (params.ex_origKey == KeyCode::KEY_2) {
      *(params.key) = KeyCode::F2;
    }
    if (params.ex_origKey == KeyCode::KEY_3) {
      *(params.key) = KeyCode::F3;
    }
    if (params.ex_origKey == KeyCode::KEY_4) {
      *(params.key) = KeyCode::F4;
    }
    if (params.ex_origKey == KeyCode::KEY_5) {
      *(params.key) = KeyCode::F5;
    }
    if (params.ex_origKey == KeyCode::KEY_6 || params.ex_origKey == KeyCode::KEYPAD_CLEAR) {
      *(params.key) = KeyCode::F6;
    }
    if (params.ex_origKey == KeyCode::KEY_7 || params.ex_origKey == KeyCode::KEYPAD_7) {
      *(params.key) = KeyCode::F7;
    }
    if (params.ex_origKey == KeyCode::KEY_8 || params.ex_origKey == KeyCode::KEYPAD_8) {
      *(params.key) = KeyCode::F8;
    }
    if (params.ex_origKey == KeyCode::KEY_9 || params.ex_origKey == KeyCode::KEYPAD_9) {
      *(params.key) = KeyCode::F9;
    }
    if (params.ex_origKey == KeyCode::KEY_0 || params.ex_origKey == KeyCode::KEYPAD_SLASH) {
      *(params.key) = KeyCode::F10;
    }
    if (params.ex_origKey == KeyCode::MINUS || params.ex_origKey == KeyCode::KEYPAD_EQUAL) {
      *(params.key) = KeyCode::F11;
    }
    if (params.ex_origKey == KeyCode::EQUAL) {
      *(params.key) = KeyCode::F12;
    }
  }

  void
  remap_hhkmode_vi_cursor(const RemapParams &params)
  {
    if (! config.remap_hhkmode_vi_cursor) return;

    RemapUtil::keyToKey(params, KeyCode::KEYPAD_MULTIPLY, KeyCode::CURSOR_LEFT);
    RemapUtil::keyToKey(params, KeyCode::KEYPAD_SLASH, KeyCode::CURSOR_DOWN);
    RemapUtil::keyToKey(params, KeyCode::HOME, KeyCode::CURSOR_UP);
    RemapUtil::keyToKey(params, KeyCode::PAGEUP, KeyCode::CURSOR_RIGHT);
  }

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

    RemapUtil::keyToKeyWithModifier(params, KeyCode::HOME, KeyCode::CURSOR_LEFT, ModifierFlag::COMMAND_L);
    RemapUtil::keyToKeyWithModifier(params, KeyCode::END, KeyCode::CURSOR_RIGHT, ModifierFlag::COMMAND_L);
  }

  // ----------------------------------------
  void
  remap_emacsmode(const RemapParams &params)
  {
    static ModifierCanceling mc_commandL;
    static ModifierCanceling mc_commandR;
    static ModifierCanceling mc_controlL;
    static ModifierCanceling mc_controlR;
    static ModifierCanceling mc_optionL;
    static ModifierCanceling mc_optionR;

    bool is_terminal = false;
    if (params.appType == KeyRemap4MacBook_bridge::ActiveApplicationInfo::EMACS) is_terminal = true;
    if (params.appType == KeyRemap4MacBook_bridge::ActiveApplicationInfo::TERMINAL) is_terminal = true;
    bool is_virtualmachine = false;
    if (params.appType == KeyRemap4MacBook_bridge::ActiveApplicationInfo::VIRTUALMACHINE) is_virtualmachine = true;
    if (params.appType == KeyRemap4MacBook_bridge::ActiveApplicationInfo::REMOTEDESKTOPCONNECTION) is_virtualmachine = true;
    bool is_x11 = (params.appType == KeyRemap4MacBook_bridge::ActiveApplicationInfo::X11);

    bool ignore = is_terminal || is_virtualmachine || is_x11;

    if (allFlagStatus.isHeldDown_control()) {
      bool cancel_control = false;

      // Control+D -> FORWARD_DELETE
      if (config.option_emacsmode_controlD && *(params.key) == KeyCode::D) {
        bool doremap = ! ignore;
        if (is_terminal && config.option_emacsmode_force_controlD_term) doremap = true;
        if (is_x11 && config.option_emacsmode_force_controlD_x11) doremap = true;
        if (is_virtualmachine && config.option_emacsmode_force_controlD_vm) doremap = true;

        if (doremap) {
          *(params.key) = KeyCode::FORWARD_DELETE;
          cancel_control = true;
        }
      }
      // Control+H -> DELETE
      if (config.option_emacsmode_controlH && *(params.key) == KeyCode::H) {
        bool doremap = ! ignore;
        if (is_terminal && config.option_emacsmode_force_controlH_term) doremap = true;
        if (is_x11 && config.option_emacsmode_force_controlH_x11) doremap = true;
        if (is_virtualmachine && config.option_emacsmode_force_controlH_vm) doremap = true;

        if (doremap) {
          *(params.key) = KeyCode::DELETE;
          cancel_control = true;
        }
      }
      // Control+I -> TAB
      if (config.option_emacsmode_controlI && *(params.key) == KeyCode::I) {
        bool doremap = ! ignore;
        if (is_terminal && config.option_emacsmode_force_controlI_term) doremap = true;
        if (is_x11 && config.option_emacsmode_force_controlI_x11) doremap = true;
        if (is_virtualmachine && config.option_emacsmode_force_controlI_vm) doremap = true;

        if (doremap) {
          *(params.key) = KeyCode::TAB;
          cancel_control = true;
        }
      }
      // Control+M -> RETURN
      if (config.option_emacsmode_controlM && *(params.key) == KeyCode::M) {
        bool doremap = ! ignore;
        if (is_terminal && config.option_emacsmode_force_controlM_term) doremap = true;
        if (is_x11 && config.option_emacsmode_force_controlM_x11) doremap = true;
        if (is_virtualmachine && config.option_emacsmode_force_controlM_vm) doremap = true;

        if (doremap) {
          *(params.key) = KeyCode::RETURN;
          cancel_control = true;
        }
      }
      // Control+[ -> ESCAPE
      if (config.option_emacsmode_controlLeftbracket && *(params.key) == KeyCode::BRACKET_LEFT) {
        bool doremap = ! ignore;
        if (is_terminal && config.option_emacsmode_force_controlLeftbracket_term) doremap = true;
        if (is_x11 && config.option_emacsmode_force_controlLeftbracket_x11) doremap = true;
        if (is_virtualmachine && config.option_emacsmode_force_controlLeftbracket_vm) doremap = true;

        if (doremap) {
          *(params.key) = KeyCode::ESCAPE;
          cancel_control = true;
        }
      }
      // --JIS-- Control+[ -> ESCAPE
      if (config.option_jis_emacsmode_controlLeftbracket && *(params.key) == KeyCode::JIS_BRACKET_LEFT) {
        bool doremap = ! ignore;
        if (is_terminal && config.option_jis_emacsmode_force_controlLeftbracket_term) doremap = true;
        if (is_x11 && config.option_jis_emacsmode_force_controlLeftbracket_x11) doremap = true;
        if (is_virtualmachine && config.option_jis_emacsmode_force_controlLeftbracket_vm) doremap = true;

        if (doremap) {
          *(params.key) = KeyCode::ESCAPE;
          cancel_control = true;
        }
      }
      // Control+PNBF -> UP/Down/Left/Right
      if (config.option_emacsmode_controlPNBF) {
        bool doremap = ! ignore;
        if (is_terminal && config.option_emacsmode_force_controlPNBF_term) doremap = true;
        if (is_x11 && config.option_emacsmode_force_controlPNBF_x11) doremap = true;
        if (is_virtualmachine && config.option_emacsmode_force_controlPNBF_vm) doremap = true;

        if (doremap) {
          if (*(params.key) == KeyCode::P) {
            *(params.key) = KeyCode::CURSOR_UP;
            cancel_control = true;
          }
          if (*(params.key) == KeyCode::N) {
            *(params.key) = KeyCode::CURSOR_DOWN;
            cancel_control = true;
          }
          if (*(params.key) == KeyCode::B) {
            *(params.key) = KeyCode::CURSOR_LEFT;
            cancel_control = true;
          }
          if (*(params.key) == KeyCode::F) {
            *(params.key) = KeyCode::CURSOR_RIGHT;
            cancel_control = true;
          }
        }
      }
      // Control+V -> PAGEDOWN
      if (config.option_emacsmode_controlV && *(params.key) == KeyCode::V && ! ignore) {
        *(params.key) = KeyCode::PAGEDOWN;
        cancel_control = true;
      }
      // Control+Y -> Command+V
      // *** Note: You need to handle option_emacsmode_controlY after option_emacsmode_controlV ***
      if (config.option_emacsmode_controlY && *(params.key) == KeyCode::Y && ! ignore) {
        *(params.key) = KeyCode::V;
        allFlagStatus.commandL.temporary_increase();
        cancel_control = true;
      }
      // Control+AE -> Command+LEFT/Right
      if (config.option_emacsmode_controlAE) {
        if (! ignore) {
          if (*(params.key) == KeyCode::A) {
            *(params.key) = KeyCode::CURSOR_LEFT;
            allFlagStatus.commandL.temporary_increase();
            cancel_control = true;
          }
          if (*(params.key) == KeyCode::E) {
            *(params.key) = KeyCode::CURSOR_RIGHT;
            allFlagStatus.commandL.temporary_increase();
            cancel_control = true;
          }
        }
      }
      // Control+AE -> HOME/END (in VirtualMachine)
      if (is_virtualmachine && config.option_emacsmode_controlAE_vm) {
        if (*(params.key) == KeyCode::A) {
          *(params.key) = KeyCode::HOME;
          cancel_control = true;
        }
        if (*(params.key) == KeyCode::E) {
          *(params.key) = KeyCode::END;
          cancel_control = true;
        }
      }
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
          cancel_control = true;
        }
      }
      // Control+Q -> PAGEUP
      if (config.option_emacsmode_ex_controlQ && *(params.key) == KeyCode::Q && ! ignore) {
        *(params.key) = KeyCode::PAGEUP;
        cancel_control = true;
      }
      // Control+W -> Option+DELETE
      if (config.option_emacsmode_ex_controlW && *(params.key) == KeyCode::W && ! ignore) {
        *(params.key) = KeyCode::DELETE;
        allFlagStatus.optionL.temporary_increase();
        cancel_control = true;
      }
      // Control+U -> Command+Shift+Left,Command+X
      if (config.option_emacsmode_ex_controlU && *(params.key) == KeyCode::U && ! ignore) {
        if (*(params.eventType) == KeyEvent::DOWN) {
          FireFunc::firefunc_emacsmode_ex_controlU(params);
        }
        *(params.ex_dropKey) = true;
        cancel_control = true;
      }
      // Control+G -> Escape
      if (config.option_emacsmode_ex_controlG && *(params.key) == KeyCode::G && ! ignore) {
        *(params.key) = KeyCode::ESCAPE;
        cancel_control = true;
      }
      // Control+12 -> HOME/END
      if (config.option_emacsmode_ex_control12) {
        if (! ignore) {
          if (*(params.key) == KeyCode::KEY_1) {
            *(params.key) = KeyCode::HOME;
            cancel_control = true;
          }
          if (*(params.key) == KeyCode::KEY_2) {
            *(params.key) = KeyCode::END;
            cancel_control = true;
          }
        }
      }
      // Control+AE -> HOME/END
      if (config.option_emacsmode_ex_controlAE) {
        if (! ignore) {
          if (*(params.key) == KeyCode::A) {
            *(params.key) = KeyCode::HOME;
            cancel_control = true;
          }
          if (*(params.key) == KeyCode::E) {
            *(params.key) = KeyCode::END;
            cancel_control = true;
          }
        }
      }

      if (cancel_control) {
        if (allFlagStatus.controlL.isHeldDown()) {
          mc_controlL.keyRelease(params, ModifierFlag::CONTROL_L);
        } else {
          mc_controlR.keyRelease(params, ModifierFlag::CONTROL_R);
        }
        return;
      }
    }

    if (allFlagStatus.isHeldDown_option()) {
      bool cancel_option = false;

      // Option+V -> PAGEUP
      if (config.option_emacsmode_optionV && *(params.key) == KeyCode::V && ! ignore) {
        *(params.key) = KeyCode::PAGEUP;
        cancel_option = true;
      }
      // Option+B -> Option+LEFT
      if (config.option_emacsmode_optionBF && *(params.key) == KeyCode::B && ! ignore) {
        *(params.key) = KeyCode::CURSOR_LEFT;
      }
      // Option+F -> Option+RIGHT
      if (config.option_emacsmode_optionBF && *(params.key) == KeyCode::F && ! ignore) {
        *(params.key) = KeyCode::CURSOR_RIGHT;
      }
      // Option+D -> Option+FORWARD_DELETE
      if (config.option_emacsmode_optionD && *(params.key) == KeyCode::D && ! ignore) {
        *(params.key) = KeyCode::FORWARD_DELETE;
      }
      // Option+< -> Home
      if (config.option_emacsmode_optionLtGt) {
        if (! ignore) {
          if (allFlagStatus.isHeldDown_shift()) {
            bool isremap = false;
            if (*(params.key) == KeyCode::COMMA) {
              *(params.key) = KeyCode::HOME;
              cancel_option = true;
              isremap = true;
            }
            if (*(params.key) == KeyCode::DOT) {
              *(params.key) = KeyCode::END;
              cancel_option = true;
              isremap = true;
            }
            if (isremap) {
              allFlagStatus.temporaryDecrease_shift();
            }
          }
        }
      }

      if (cancel_option) {
        if (allFlagStatus.optionL.isHeldDown()) {
          mc_optionL.keyRelease(params, ModifierFlag::OPTION_L);
        } else {
          mc_optionR.keyRelease(params, ModifierFlag::OPTION_R);
        }
        return;
      }
    }

    if (allFlagStatus.isHeldDown_command()) {
      bool cancel_command = false;

      // Command+V -> PageUp
      if (config.option_emacsmode_commandV && *(params.key) == KeyCode::V && ! ignore) {
        *(params.key) = KeyCode::PAGEUP;
        cancel_command = true;
      }

      if (cancel_command) {
        if (allFlagStatus.commandL.isHeldDown()) {
          mc_commandL.keyRelease(params, ModifierFlag::COMMAND_L);
        } else {
          mc_commandR.keyRelease(params, ModifierFlag::COMMAND_R);
        }
        return;
      }
    }

    mc_commandL.restore(params, ModifierFlag::COMMAND_L);
    mc_commandR.restore(params, ModifierFlag::COMMAND_R);
    mc_controlL.restore(params, ModifierFlag::CONTROL_L);
    mc_controlR.restore(params, ModifierFlag::CONTROL_R);
    mc_optionL.restore(params, ModifierFlag::OPTION_L);
    mc_optionR.restore(params, ModifierFlag::OPTION_R);
  }

  // ----------------------------------------
  void
  remap_vimode(const RemapParams &params)
  {
    static ModifierCanceling mc_commandR;

    if (allFlagStatus.commandR.isHeldDown()) {
      bool cancel_command = false;

      // Command_R+hjkl -> Up/Down/Left/Right
      if (config.option_vimode_hjkl) {
        if (*(params.key) == KeyCode::H) {
          *(params.key) = KeyCode::CURSOR_LEFT;
          cancel_command = true;
        }
        if (*(params.key) == KeyCode::J) {
          *(params.key) = KeyCode::CURSOR_DOWN;
          cancel_command = true;
        }
        if (*(params.key) == KeyCode::K) {
          *(params.key) = KeyCode::CURSOR_UP;
          cancel_command = true;
        }
        if (*(params.key) == KeyCode::L) {
          *(params.key) = KeyCode::CURSOR_RIGHT;
          cancel_command = true;
        }
      }

      // Command_R+gG -> Home/End
      if (config.option_vimode_gG) {
        if (*(params.key) == KeyCode::G) {
          if (allFlagStatus.isHeldDown_shift()) {
            *(params.key) = KeyCode::END;
            allFlagStatus.temporaryDecrease_shift();

          } else {
            *(params.key) = KeyCode::HOME;
          }

          cancel_command = true;
        }
      }

      // Command_R+bf -> PageUp/PageDown
      if (config.option_vimode_bf) {
        if (*(params.key) == KeyCode::B) {
          *(params.key) = KeyCode::PAGEUP;
          cancel_command = true;
        }
        if (*(params.key) == KeyCode::F) {
          *(params.key) = KeyCode::PAGEDOWN;
          cancel_command = true;
        }
      }

      // Command_R+0$ -> Command+Left/Right
      if (config.option_vimode_0dollar) {
        if (*(params.key) == KeyCode::KEY_0) {
          *(params.key) = KeyCode::CURSOR_LEFT;
        }
        if (*(params.key) == KeyCode::KEY_4) {
          if (allFlagStatus.isHeldDown_shift()) {
            *(params.key) = KeyCode::CURSOR_RIGHT;
            allFlagStatus.temporaryDecrease_shift();
          }
        }
      }

      if (cancel_command) {
        mc_commandR.keyRelease(params, ModifierFlag::COMMAND_R);
        return;
      }
    }
    mc_commandR.restore(params, ModifierFlag::COMMAND_R);
  }

  // ----------------------------------------
  void
  remap_drop_funcshift(const RemapParams &params)
  {
    if (! config.remap_drop_funcshift) return;

    if (allFlagStatus.isHeldDown_shift()) {
      if (*(params.key) == KeyCode::F1 ||
          *(params.key) == KeyCode::F2 ||
          *(params.key) == KeyCode::F3 ||
          *(params.key) == KeyCode::F4 ||
          *(params.key) == KeyCode::F5 ||
          *(params.key) == KeyCode::F6 ||
          *(params.key) == KeyCode::F7 ||
          *(params.key) == KeyCode::F8 ||
          *(params.key) == KeyCode::F9 ||
          *(params.key) == KeyCode::F10 ||
          *(params.key) == KeyCode::F11 ||
          *(params.key) == KeyCode::F12 ||
          *(params.key) == KeyCode::EXPOSE_ALL) {
        *(params.ex_dropKey) = true;
      }
    }
  }

  void
  remap_spaces_special(const RemapParams &params)
  {
    if (! config.remap_spaces_special) return;

    if (allFlagStatus.makeFlags(params) != ModifierFlag::COMMAND_R) return;

    RemapUtil::key2spaces(params, ModifierFlag::COMMAND_L | ModifierFlag::CONTROL_L | ModifierFlag::OPTION_L | ModifierFlag::SHIFT_L,
                          KeyCode::A, KeyCode::S, KeyCode::D,
                          KeyCode::F, KeyCode::G, KeyCode::H,
                          KeyCode::J, KeyCode::K, KeyCode::L);
  }

  void
  remap_spaces_special_123qweasd(const RemapParams &params)
  {
    if (! config.remap_spaces_special_123qweasd) return;

    if (allFlagStatus.makeFlags(params) != ModifierFlag::COMMAND_R) return;

    RemapUtil::key2spaces(params, ModifierFlag::COMMAND_L | ModifierFlag::CONTROL_L | ModifierFlag::OPTION_L | ModifierFlag::SHIFT_L,
                          KeyCode::KEY_1, KeyCode::KEY_2, KeyCode::KEY_3,
                          KeyCode::Q, KeyCode::W, KeyCode::E,
                          KeyCode::A, KeyCode::S, KeyCode::D);
  }

  void
  remap_spaces_special_123qweasd_4x4(const RemapParams &params)
  {
    if (! config.remap_spaces_special_123qweasd_4x4) return;

    if (allFlagStatus.makeFlags(params) != ModifierFlag::COMMAND_R) return;

    RemapUtil::key2spaces(params, ModifierFlag::COMMAND_L | ModifierFlag::CONTROL_L | ModifierFlag::SHIFT_L,
                          KeyCode::KEY_1, KeyCode::KEY_2, KeyCode::KEY_3, KeyCode::NONE,
                          KeyCode::Q, KeyCode::W, KeyCode::E, KeyCode::NONE,
                          KeyCode::A, KeyCode::S, KeyCode::D, KeyCode::NONE);
  }

  void
  remap_spaces_special_qweasdzxc(const RemapParams &params)
  {
    if (! config.remap_spaces_special_qweasdzxc) return;

    if (allFlagStatus.makeFlags(params) != ModifierFlag::COMMAND_R) return;

    RemapUtil::key2spaces(params, ModifierFlag::COMMAND_L | ModifierFlag::CONTROL_L | ModifierFlag::OPTION_L | ModifierFlag::SHIFT_L,
                          KeyCode::Q, KeyCode::W, KeyCode::E,
                          KeyCode::A, KeyCode::S, KeyCode::D,
                          KeyCode::Z, KeyCode::X, KeyCode::C);
  }

  void
  remap_spaces_special_1234qwerasdfzxcv(const RemapParams &params)
  {
    if (! config.remap_spaces_special_1234qwerasdfzxcv) return;

    if (allFlagStatus.makeFlags(params) != ModifierFlag::FN) return;

    RemapUtil::key2spaces(params, ModifierFlag::COMMAND_L | ModifierFlag::CONTROL_L | ModifierFlag::SHIFT_L,
                          KeyCode::KEY_1, KeyCode::KEY_2, KeyCode::KEY_3, KeyCode::KEY_4,
                          KeyCode::Q, KeyCode::W, KeyCode::E, KeyCode::R,
                          KeyCode::A, KeyCode::S, KeyCode::D, KeyCode::F,
                          KeyCode::Z, KeyCode::X, KeyCode::C, KeyCode::V);
  }

  void
  remap_spaces_special_fn(const RemapParams &params)
  {
    if (! config.remap_spaces_special_fn) return;

    if (allFlagStatus.makeFlags(params) != ModifierFlag::FN &&
        allFlagStatus.makeFlags(params) != (ModifierFlag::FN | ModifierFlag::KEYPAD)) return;

    RemapUtil::keyToKey(params, KeyCode::KEYPAD_CLEAR, KeyCode::KEY_6);
    RemapUtil::keyToKey(params, KeyCode::KEYPAD_7, KeyCode::KEY_7);
    RemapUtil::keyToKey(params, KeyCode::KEYPAD_8, KeyCode::KEY_8);
    RemapUtil::keyToKey(params, KeyCode::KEYPAD_9, KeyCode::KEY_9);

    if (*(params.key) == KeyCode::KEY_1 ||
        *(params.key) == KeyCode::KEY_2 ||
        *(params.key) == KeyCode::KEY_3 ||
        *(params.key) == KeyCode::KEY_4 ||
        *(params.key) == KeyCode::KEY_5 ||
        *(params.key) == KeyCode::KEY_6 ||
        *(params.key) == KeyCode::KEY_7 ||
        *(params.key) == KeyCode::KEY_8 ||
        *(params.key) == KeyCode::KEY_9) {
      allFlagStatus.fn.temporary_decrease();
      RemapUtil::fireKeyWithModifiers(params,
                                      (ModifierFlag::COMMAND_L | ModifierFlag::CONTROL_L | ModifierFlag::OPTION_L | ModifierFlag::SHIFT_L),
                                      *(params.eventType), *(params.key), *(params.charCode));
    }
  }

  void
  remap_spaces_special_keypad(const RemapParams &params)
  {
    if (! config.remap_spaces_special_keypad) return;

    if (allFlagStatus.makeFlags(params) != ModifierFlag::KEYPAD &&
        allFlagStatus.makeFlags(params) != (ModifierFlag::FN | ModifierFlag::KEYPAD)) return;

    if (RemapUtil::keypad2spaces(params)) {
      allFlagStatus.fn.temporary_decrease();

      RemapUtil::fireKeyWithModifiers(params,
                                      (ModifierFlag::COMMAND_L | ModifierFlag::CONTROL_L | ModifierFlag::OPTION_L | ModifierFlag::SHIFT_L),
                                      *(params.eventType), *(params.key), *(params.charCode));
    }
  }

  void
  remap_keypad2spaces(const RemapParams &params)
  {
    if (! config.remap_keypad2spaces) return;

    if (config.option_keypad2spaces_modifier_command) {
      if (! allFlagStatus.isHeldDown_command()) return;
    }
    if (config.option_keypad2spaces_modifier_control) {
      if (! allFlagStatus.isHeldDown_control()) return;
    }
    if (config.option_keypad2spaces_modifier_option) {
      if (! allFlagStatus.isHeldDown_option()) return;
    }
    if (config.option_keypad2spaces_modifier_shift) {
      if (! allFlagStatus.isHeldDown_shift()) return;
    }

    RemapUtil::keypad2spaces(params);
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
    static ModifierCanceling mc_commandL;
    static ModifierCanceling mc_commandR;

    if (! config.remap_app_vm_commandspace2optionbackquote) return;

    //  "Remote Desktop Connection" has the self remapping feature.
    // Our targets are VMware, Parallels only.
    if (params.appType != KeyRemap4MacBook_bridge::ActiveApplicationInfo::VIRTUALMACHINE) return;

    if (allFlagStatus.isHeldDown_command()) {
      bool cancel_command = false;

      if (*(params.key) == KeyCode::SPACE) {
        // hack keyboardType to fire backquote on any keyboards.
        *(params.keyboardType) = KeyboardType::MACBOOK;
        *(params.key) = KeyCode::BACKQUOTE;
        allFlagStatus.optionL.temporary_increase();
        cancel_command = true;
      }
      if (cancel_command) {
        if (allFlagStatus.commandL.isHeldDown()) {
          mc_commandL.keyRelease(params, ModifierFlag::COMMAND_L);
        } else {
          mc_commandR.keyRelease(params, ModifierFlag::COMMAND_R);
        }
        return;
      }
    }
    mc_commandL.restore(params, ModifierFlag::COMMAND_L);
    mc_commandR.restore(params, ModifierFlag::COMMAND_R);
  }

  void
  remap_app_finder_command_R_to_return(const RemapParams &params)
  {
    if (! config.remap_app_finder_command_R_to_return) return;

    if (params.appType != KeyRemap4MacBook_bridge::ActiveApplicationInfo::FINDER) return;

    static KeyWithModifierToKey kwmk_l;
    static KeyWithModifierToKey kwmk_r;

    if (kwmk_l.remap(params, KeyCode::R, ModifierFlag::COMMAND_L, KeyCode::RETURN)) return;
    if (kwmk_r.remap(params, KeyCode::R, ModifierFlag::COMMAND_R, KeyCode::RETURN)) return;
  }

  void
  remap_app_finder_f2_to_return(const RemapParams &params)
  {
    if (! config.remap_app_finder_f2_to_return) return;

    if (params.appType != KeyRemap4MacBook_bridge::ActiveApplicationInfo::FINDER) return;

    RemapUtil::keyToKey(params, KeyCode::F2, KeyCode::RETURN);
  }

  void
  remap_app_finder_return2commandO(const RemapParams &params)
  {
    if (! config.remap_app_finder_return2commandO) return;

    if (params.appType != KeyRemap4MacBook_bridge::ActiveApplicationInfo::FINDER) return;

    if (*(params.key) == KeyCode::RETURN) {
      if (*(params.eventType) == KeyEvent::DOWN) {
        FireFunc::firefunc_commandO(params);
      }
      *(params.ex_dropKey) = true;
    }
  }

  void
  remap_app_term_commandL2optionL(const RemapParams &params)
  {
    if (! config.remap_app_term_commandL2optionL) return;

    if (params.appType != KeyRemap4MacBook_bridge::ActiveApplicationInfo::TERMINAL) return;

    if (*(params.key) == KeyCode::COMMAND_L) {
      unsigned int flags = allFlagStatus.makeFlags(params);
      if (RemapUtil::isModifierOn(flags, ModifierFlag::COMMAND_L)) {
        allFlagStatus.optionL.increase();
        allFlagStatus.commandL.decrease();
      } else {
        allFlagStatus.optionL.decrease();
        allFlagStatus.commandL.increase();
      }
      *(params.key) = KeyCode::OPTION_L;
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

  remap_backquote2escape_withoutmodifiers(params);

  remap_delete2f13_shift2tilde(params);
  remap_shiftDelete2tilde(params);

  remap_enter2commandL_enter2controlL_vm(params);

  remap_fn2controlL_commandR2fn(params);
  remap_fn2fn(params);

  remap_f1_to_brightnessdown(params);
  remap_f2_to_brightnessup(params);
  remap_f3_to_volumemute(params);
  remap_f4_to_volumedown(params);
  remap_f5_to_volumeup(params);

  remap_optionR2allF1(params);

  remap_semicolon2return_controlsemicolon2semicolon(params);
  remap_swapcolons(params);

  remap_shiftLshiftR2space(params);
  remap_shiftRshiftL2space(params);

  remap_tab2option_withControlL(params);
  remap_commandTab2optionTab(params);
  remap_commandTab2f5(params);
  remap_optionTab2f5(params);
  remap_optionTab2commandTab(params);

  // ----------------------------------------
  remap_app_vm_enter2controlL(params);
  remap_app_vm_commandspace2optionbackquote(params);
  // *** Note: we need to call return2commandO before command_R_to_return, f2_to_return ***
  remap_app_finder_return2commandO(params);
  remap_app_finder_command_R_to_return(params);
  remap_app_finder_f2_to_return(params);

  // ----------------------------------------
  remap_qwerty2colemak(params);
  remap_qwerty2qwerf(params);

  // ----------------------------------------
  remap_pc_pause2eject(params);
  remap_keypadnumlock(params);
  remap_keypadnumlock_togglekey_clear(params);

  // ----------------------------------------
  remap_hhkmode(params);
  remap_hhkmode_vi_cursor(params);

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
  // *** Note: we need to call remap_drop_funcshift after tab2f9, pc_application2f11, ... ***
  remap_drop_funcshift(params);

  // ------------------------------------------------------------
  // *** Note: we need to call remap_emacsmode as possible late. ***
  // *** If qwerty2colemak is enabled, Control+H... works with Colemak Keyboard Layout. ***
  remap_emacsmode(params);
  remap_vimode(params);

  // ----------------------------------------
  // *** Note: we need to call remap_spaces_* after emacsmode. ***
  // *** If spaces_special is enabled, emacsmode_ex_control12 make wrong remappings, . ***
  remap_spaces_special(params);
  remap_spaces_special_123qweasd(params);
  remap_spaces_special_123qweasd_4x4(params);
  remap_spaces_special_1234qwerasdfzxcv(params);
  remap_spaces_special_qweasdzxc(params);
  remap_spaces_special_fn(params);
  remap_spaces_special_keypad(params);
  remap_keypad2spaces(params);

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
  // *** Note: we need to call remap_app_term_commandL2optionL as possible late. ***
  // *** If any *2commandL remappings is enable, remap_app_term_commandL2optionL needs to handle it ***
  remap_app_term_commandL2optionL(params);

  // ------------------------------------------------------------
#include "config/output/include.remapcode_call_mhkk.cpp"

  // ------------------------------------------------------------
  // *** we need to handle KeyOverlaidModifierCombination at last. ***
#include "config/output/include.remapcode_call_komc.cpp"

  // ------------------------------------------------------------
  *(params.flags) = allFlagStatus.makeFlags(params);

  // ======================================================================
  // post actions
  if (*(params.ex_dropKey)) return;

  jisKanaMode.setMode(*(params.eventType), *(params.key), *(params.flags));

  if (config.debug) {
    printf("sending hid event type %d flags 0x%x key %d ", *(params.eventType), *(params.flags), *(params.key));
    printf("charCode %d charSet %d ", *(params.charCode), *(params.charSet));
    printf("origCharCode %d origCharSet %d kbdType %d\n",
           *(params.origCharCode), *(params.origCharSet), *(params.keyboardType));
  }
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
