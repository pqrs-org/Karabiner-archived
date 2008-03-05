#include <sys/systm.h>

#include "remap.hpp"
#include "RemapUtil.hpp"
#include "Config.hpp"
#include "keycode.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  // ----------------------------------------
  void
  remap_backquote2commandL(const RemapParams &params)
  {
    if (! config.remap_backquote2commandL) return;

    RemapUtil::keyToModifier(params, KeyCode::BACKQUOTE, ModifierFlag::COMMAND_L);
  }

  void
  remap_backquote2escape(const RemapParams &params)
  {
    if (! config.remap_backquote2escape) return;

    RemapUtil::keyToKey(params, KeyCode::BACKQUOTE, KeyCode::ESCAPE);
  }

  void
  remap_backquote2escape_shift2tilde(const RemapParams &params)
  {
    if (! config.remap_backquote2escape_shift2tilde) return;

    unsigned int flags = allFlagStatus.makeFlags(params);
    if (flags != ModifierFlag::SHIFT_L && flags != ModifierFlag::SHIFT_R) {
      RemapUtil::keyToKey(params, KeyCode::BACKQUOTE, KeyCode::ESCAPE);
    }
  }

  // ----------------------------------------
  void
  remap_backslash2delete(const RemapParams &params)
  {
    if (! config.remap_backslash2delete) return;

    RemapUtil::keyToKey(params, KeyCode::BACKSLASH, KeyCode::DELETE);
    RemapUtil::toDelete(params);
  }

  // ----------------------------------------
  void
  remap_clear2tab(const RemapParams &params)
  {
    if (! config.remap_clear2tab) return;

    RemapUtil::keyToKey(params, KeyCode::KEYPAD_CLEAR, KeyCode::TAB);
  }

  // ----------------------------------------
  void
  remap_commandL2controlL(const RemapParams &params)
  {
    if (! config.remap_commandL2controlL) return;

    RemapUtil::modifierToModifier(params, ModifierFlag::COMMAND_L, ModifierFlag::CONTROL_L);
  }

  void
  remap_commandL2optionL(const RemapParams &params)
  {
    if (! config.remap_commandL2optionL) return;

    RemapUtil::modifierToModifier(params, ModifierFlag::COMMAND_L, ModifierFlag::OPTION_L);
  }

  void
  remap_commandL2shiftL(const RemapParams &params)
  {
    if (! config.remap_commandL2shiftL) return;

    RemapUtil::modifierToModifier(params, ModifierFlag::COMMAND_L, ModifierFlag::SHIFT_L);
  }

  void
  remap_commandL2backquote(const RemapParams &params)
  {
    if (! config.remap_commandL2backquote) return;

    RemapUtil::modifierToKey(params, ModifierFlag::COMMAND_L, KeyCode::BACKQUOTE);
  }

  // ----------------------------------------
  void
  remap_commandR2controlL(const RemapParams &params)
  {
    if (! config.remap_commandR2controlL) return;

    RemapUtil::modifierToModifier(params, ModifierFlag::COMMAND_R, ModifierFlag::CONTROL_L);
  }

  void
  remap_commandR2optionL(const RemapParams &params)
  {
    if (! config.remap_commandR2optionL) return;

    RemapUtil::modifierToModifier(params, ModifierFlag::COMMAND_R, ModifierFlag::OPTION_L);
  }

  void
  remap_commandR2shiftL(const RemapParams &params)
  {
    if (! config.remap_commandR2shiftL) return;

    RemapUtil::modifierToModifier(params, ModifierFlag::COMMAND_R, ModifierFlag::SHIFT_L);
  }

  // ----------------------------------------
  void
  remap_controlL2commandL(const RemapParams &params)
  {
    if (! config.remap_controlL2commandL) return;

    RemapUtil::modifierToModifier(params, ModifierFlag::CONTROL_L, ModifierFlag::COMMAND_L);
  }

  void
  remap_controlL2fn(const RemapParams &params)
  {
    if (! config.remap_controlL2fn) return;

    RemapUtil::modifierToModifier(params, ModifierFlag::CONTROL_L, ModifierFlag::FN);
    RemapUtil::toFN(params);
  }

  void
  remap_controlL2optionL(const RemapParams &params)
  {
    if (! config.remap_controlL2optionL) return;

    RemapUtil::modifierToModifier(params, ModifierFlag::CONTROL_L, ModifierFlag::OPTION_L);
  }

  void
  remap_controlL2shiftL(const RemapParams &params)
  {
    if (! config.remap_controlL2shiftL) return;

    RemapUtil::modifierToModifier(params, ModifierFlag::CONTROL_L, ModifierFlag::SHIFT_L);
  }

  // ----------------------------------------
  void
  remap_delete2backslash(const RemapParams &params)
  {
    if (! config.remap_delete2backslash) return;

    RemapUtil::keyToKey(params, KeyCode::DELETE, KeyCode::BACKSLASH);
  }

  void
  remap_shiftDelete2tilde(const RemapParams &params)
  {
    if (! config.remap_shiftDelete2tilde) return;

    if (allFlagStatus.shiftL.isHeldDown() || allFlagStatus.shiftR.isHeldDown()) {
      RemapUtil::keyToKey(params, KeyCode::DELETE, KeyCode::BACKQUOTE);
    }
  }

  // ----------------------------------------
  void
  remap_enter2commandL(const RemapParams &params)
  {
    if (! config.remap_enter2commandL) return;

    RemapUtil::keyToModifier(params, RemapUtil::getEnterKeyCode(params), ModifierFlag::COMMAND_L);
  }

  void
  remap_enter2controlL(const RemapParams &params)
  {
    if (! config.remap_enter2controlL) return;

    RemapUtil::keyToModifier(params, RemapUtil::getEnterKeyCode(params), ModifierFlag::CONTROL_L);
  }

  void
  remap_enter2fn(const RemapParams &params)
  {
    if (! config.remap_enter2fn) return;

    RemapUtil::keyToModifier(params, RemapUtil::getEnterKeyCode(params), ModifierFlag::FN);
    RemapUtil::toFN(params);
  }

  void
  remap_enter2optionL(const RemapParams &params)
  {
    if (! config.remap_enter2optionL) return;

    RemapUtil::keyToModifier(params, RemapUtil::getEnterKeyCode(params), ModifierFlag::OPTION_L);
  }

  void
  remap_enter2optionL_commandSpace(const RemapParams &params)
  {
    if (! config.remap_enter2optionL_commandSpace) return;

    static KeyOverlayedModifier kom;
    KeyCode::KeyCode fromKeyCode = RemapUtil::getEnterKeyCode(params);
    kom.remap(params, fromKeyCode, ModifierFlag::OPTION_L, FireFunc::firefunc_commandSpace);
  }

  void
  remap_enter2commandLcontrolL(const RemapParams &params)
  {
    if (! config.remap_enter2commandLcontrolL) return;

    RemapUtil::keyToModifier(params, RemapUtil::getEnterKeyCode(params), ModifierFlag::COMMAND_L, ModifierFlag::CONTROL_L);
  }

  void
  remap_enter2commandLshiftL(const RemapParams &params)
  {
    if (! config.remap_enter2commandLshiftL) return;

    RemapUtil::keyToModifier(params, RemapUtil::getEnterKeyCode(params), ModifierFlag::COMMAND_L, ModifierFlag::SHIFT_L);
  }

  void
  remap_enter2controlLoptionL(const RemapParams &params)
  {
    if (! config.remap_enter2controlLoptionL) return;

    RemapUtil::keyToModifier(params, RemapUtil::getEnterKeyCode(params), ModifierFlag::CONTROL_L, ModifierFlag::OPTION_L);
  }

  void
  remap_enter2semicolon(const RemapParams &params)
  {
    if (! config.remap_enter2semicolon) return;

    RemapUtil::keyToKey(params, RemapUtil::getEnterKeyCode(params), KeyCode::SEMICOLON);
  }

  void
  remap_enter2space(const RemapParams &params)
  {
    if (! config.remap_enter2space) return;

    RemapUtil::keyToKey(params, RemapUtil::getEnterKeyCode(params), KeyCode::SPACE);
  }

  // ----------------------------------------
  void
  remap_escape2backquote(const RemapParams &params)
  {
    if (! config.remap_escape2backquote) return;

    RemapUtil::keyToKey(params, KeyCode::ESCAPE, KeyCode::BACKQUOTE);
  }

  void
  remap_escape2rightclick(const RemapParams &params)
  {
    if (! config.remap_escape2rightclick) return;

    if (params.ex_origKey != KeyCode::ESCAPE) return;

    RemapUtil::keyToKey(params, KeyCode::ESCAPE, KeyCode::KEYPAD_5);

    if (*(params.eventType) == KeyEvent::DOWN) {
      allFlagStatus.controlL.increase();
    } else if (*(params.eventType) == KeyEvent::UP) {
      allFlagStatus.controlL.decrease();
    }
  }

  // ----------------------------------------
  void
  remap_fn2commandL(const RemapParams &params)
  {
    if (! config.remap_fn2commandL) return;

    if (RemapUtil::isModifierOn(params, ModifierFlag::FN)) {
      RemapUtil::fnToNormal(params);
    }
    RemapUtil::modifierToModifier(params, ModifierFlag::FN, ModifierFlag::COMMAND_L);
  }

  void
  remap_fn2controlL(const RemapParams &params)
  {
    if (! config.remap_fn2controlL) return;

    if (RemapUtil::isModifierOn(params, ModifierFlag::FN)) {
      RemapUtil::fnToNormal(params);
    }
    RemapUtil::modifierToModifier(params, ModifierFlag::FN, ModifierFlag::CONTROL_L);
  }

  void
  remap_fn2optionL(const RemapParams &params)
  {
    if (! config.remap_fn2optionL) return;

    if (RemapUtil::isModifierOn(params, ModifierFlag::FN)) {
      RemapUtil::fnToNormal(params);
    }
    RemapUtil::modifierToModifier(params, ModifierFlag::FN, ModifierFlag::OPTION_L);
  }

  void
  remap_fn2shiftL(const RemapParams &params)
  {
    if (! config.remap_fn2shiftL) return;

    if (RemapUtil::isModifierOn(params, ModifierFlag::FN)) {
      RemapUtil::fnToNormal(params);
    }
    RemapUtil::modifierToModifier(params, ModifierFlag::FN, ModifierFlag::SHIFT_L);
  }

  // ----------------------------------------
  void
  remap_f16_to_tab(const RemapParams &params)
  {
    if (! config.remap_f16_to_tab) return;

    RemapUtil::keyToKey(params, KeyCode::F16, KeyCode::TAB);
  }

  // ----------------------------------------
  void
  remap_optionL2commandL(const RemapParams &params)
  {
    if (! config.remap_optionL2commandL) return;

    RemapUtil::modifierToModifier(params, ModifierFlag::OPTION_L, ModifierFlag::COMMAND_L);
  }

  void
  remap_optionL2shiftL(const RemapParams &params)
  {
    if (! config.remap_optionL2shiftL) return;

    RemapUtil::modifierToModifier(params, ModifierFlag::OPTION_L, ModifierFlag::SHIFT_L);
  }

  // ----------------------------------------
  void
  remap_optionR2commandL(const RemapParams &params)
  {
    if (! config.remap_optionR2commandL) return;

    RemapUtil::modifierToModifier(params, ModifierFlag::OPTION_R, ModifierFlag::COMMAND_L);
  }

  void
  remap_optionR2controlL(const RemapParams &params)
  {
    if (! config.remap_optionR2controlL) return;

    RemapUtil::modifierToModifier(params, ModifierFlag::OPTION_R, ModifierFlag::CONTROL_L);
  }

  void
  remap_optionR2fn(const RemapParams &params)
  {
    if (! config.remap_optionR2fn) return;

    RemapUtil::modifierToModifier(params, ModifierFlag::OPTION_R, ModifierFlag::FN);
    RemapUtil::toFN(params);
  }

  void
  remap_optionR2enter(const RemapParams &params)
  {
    if (! config.remap_optionR2enter) return;

    RemapUtil::modifierToKey(params, ModifierFlag::OPTION_R, RemapUtil::getEnterKeyCode(params));
  }

  void
  remap_optionR2semicolon(const RemapParams &params)
  {
    if (! config.remap_optionR2semicolon) return;

    RemapUtil::modifierToKey(params, ModifierFlag::OPTION_R, KeyCode::SEMICOLON);
  }

  void
  remap_optionR2space(const RemapParams &params)
  {
    if (! config.remap_optionR2space) return;

    RemapUtil::modifierToKey(params, ModifierFlag::OPTION_R, KeyCode::SPACE);
  }

  // ----------------------------------------
  void
  remap_return2optionL(const RemapParams &params)
  {
    if (! config.remap_return2optionL) return;

    RemapUtil::keyToModifier(params, KeyCode::RETURN, ModifierFlag::OPTION_L);
  }

  void
  remap_return2optionL_escape(const RemapParams &params)
  {
    if (! config.remap_return2optionL_escape) return;

    static KeyOverlayedModifier kom;
    kom.remap(params, KeyCode::RETURN, ModifierFlag::OPTION_L, FireFunc::firefunc_escape);
  }

  void
  remap_return2semicolon(const RemapParams &params)
  {
    if (! config.remap_return2semicolon) return;

    RemapUtil::keyToKey(params, KeyCode::RETURN, KeyCode::SEMICOLON);
  }

  // ----------------------------------------
  void
  remap_space2controlL(const RemapParams &params)
  {
    if (! config.remap_space2controlL) return;

    RemapUtil::keyToModifier(params, KeyCode::SPACE, ModifierFlag::CONTROL_L);
  }

  void
  remap_space2controlL_space(const RemapParams &params)
  {
    if (! config.remap_space2controlL_space) return;

    static KeyOverlayedModifier kom;
    kom.remap(params, KeyCode::SPACE, ModifierFlag::CONTROL_L, FireFunc::firefunc_space);
  }

  void
  remap_space2shiftL(const RemapParams &params)
  {
    if (! config.remap_space2shiftL) return;

    RemapUtil::keyToModifier(params, KeyCode::SPACE, ModifierFlag::SHIFT_L);
  }

  void
  remap_space2shiftL_space(const RemapParams &params)
  {
    if (! config.remap_space2shiftL_space) return;

    static KeyOverlayedModifier kom;
    kom.remap(params, KeyCode::SPACE, ModifierFlag::SHIFT_L, FireFunc::firefunc_space);
  }

  // ----------------------------------------
  void
  remap_semicolon2return(const RemapParams &params)
  {
    if (! config.remap_semicolon2return) return;

    RemapUtil::keyToKey(params, KeyCode::SEMICOLON, KeyCode::RETURN);
  }

  // ----------------------------------------
  void
  remap_shiftL2commandL(const RemapParams &params)
  {
    if (! config.remap_shiftL2commandL) return;

    RemapUtil::modifierToModifier(params, ModifierFlag::SHIFT_L, ModifierFlag::COMMAND_L);
  }

  void
  remap_shiftL2controlL(const RemapParams &params)
  {
    if (! config.remap_shiftL2controlL) return;

    RemapUtil::modifierToModifier(params, ModifierFlag::SHIFT_L, ModifierFlag::CONTROL_L);
  }

  void
  remap_shiftL2fn(const RemapParams &params)
  {
    if (! config.remap_shiftL2fn) return;

    RemapUtil::modifierToModifier(params, ModifierFlag::SHIFT_L, ModifierFlag::FN);
    RemapUtil::toFN(params);
  }

  void
  remap_shiftL2optionL(const RemapParams &params)
  {
    if (! config.remap_shiftL2optionL) return;

    RemapUtil::modifierToModifier(params, ModifierFlag::SHIFT_L, ModifierFlag::OPTION_L);
  }

  void
  remap_shiftL2escape(const RemapParams &params)
  {
    if (! config.remap_shiftL2escape) return;

    RemapUtil::modifierToKey(params, ModifierFlag::SHIFT_L, KeyCode::ESCAPE);
  }

  void
  remap_shiftL2space(const RemapParams &params)
  {
    if (! config.remap_shiftL2space) return;

    RemapUtil::modifierToKey(params, ModifierFlag::SHIFT_L, KeyCode::SPACE);
  }

  // ----------------------------------------
  void
  remap_shiftR2commandL(const RemapParams &params)
  {
    if (! config.remap_shiftR2commandL) return;

    RemapUtil::modifierToModifier(params, ModifierFlag::SHIFT_R, ModifierFlag::COMMAND_L);
  }

  void
  remap_shiftR2controlL(const RemapParams &params)
  {
    if (! config.remap_shiftR2controlL) return;

    RemapUtil::modifierToModifier(params, ModifierFlag::SHIFT_R, ModifierFlag::CONTROL_L);
  }

  void
  remap_shiftR2fn(const RemapParams &params)
  {
    if (! config.remap_shiftR2fn) return;

    RemapUtil::modifierToModifier(params, ModifierFlag::SHIFT_R, ModifierFlag::FN);
    RemapUtil::toFN(params);
  }

  void
  remap_shiftR2optionL(const RemapParams &params)
  {
    if (! config.remap_shiftR2optionL) return;

    RemapUtil::modifierToModifier(params, ModifierFlag::SHIFT_R, ModifierFlag::OPTION_L);
  }

  void
  remap_shiftR2escape(const RemapParams &params)
  {
    if (! config.remap_shiftR2escape) return;

    RemapUtil::modifierToKey(params, ModifierFlag::SHIFT_R, KeyCode::ESCAPE);
  }

  void
  remap_shiftR2space(const RemapParams &params)
  {
    if (! config.remap_shiftR2space) return;

    RemapUtil::modifierToKey(params, ModifierFlag::SHIFT_R, KeyCode::SPACE);
  }

  // ----------------------------------------
  void
  remap_tab2exposeALL(const RemapParams &params)
  {
    if (! config.remap_tab2exposeALL) return;

    RemapUtil::keyToKey(params, KeyCode::TAB, KeyCode::EXPOSE_ALL);
  }

  // ----------------------------------------
  void
  remap_hhkmode(const RemapParams &params)
  {
    if (! config.remap_hhkmode) return;

    if (! allFlagStatus.fn.isHeldDown()) return;

    allFlagStatus.keypad = false;

    // [ => up
    if (params.ex_origKey == KeyCode::BRACKET_LEFT) {
      *(params.key) = KeyCode::CURSOR_UP;
      allFlagStatus.cursor = true;
      allFlagStatus.fn.temporary_decrease();
    }
    // ; => left
    if (params.ex_origKey == KeyCode::SEMICOLON || params.ex_origKey == KeyCode::KEYPAD_MINUS) {
      *(params.key) = KeyCode::CURSOR_LEFT;
      allFlagStatus.cursor = true;
      allFlagStatus.fn.temporary_decrease();
    }
    // ' => right
    if (params.ex_origKey == KeyCode::QUOTE) {
      *(params.key) = KeyCode::CURSOR_RIGHT;
      allFlagStatus.cursor = true;
      allFlagStatus.fn.temporary_decrease();
    }
    // / => down
    if (params.ex_origKey == KeyCode::SLASH || params.ex_origKey == KeyCode::KEYPAD_PLUS) {
      *(params.key) = KeyCode::CURSOR_DOWN;
      allFlagStatus.cursor = true;
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
      allFlagStatus.keypad = false;
      allFlagStatus.cursor = true;
      RemapUtil::keyToKey(params, KeyCode::KEYPAD_2, KeyCode::CURSOR_DOWN);
      RemapUtil::keyToKey(params, KeyCode::KEYPAD_4, KeyCode::CURSOR_LEFT);
      RemapUtil::keyToKey(params, KeyCode::KEYPAD_6, KeyCode::CURSOR_RIGHT);
      RemapUtil::keyToKey(params, KeyCode::KEYPAD_8, KeyCode::CURSOR_UP);
    }

    if (params.ex_origKey == KeyCode::KEYPAD_1 ||
        params.ex_origKey == KeyCode::KEYPAD_3 ||
        params.ex_origKey == KeyCode::KEYPAD_7 ||
        params.ex_origKey == KeyCode::KEYPAD_9) {
      allFlagStatus.keypad = false;
      RemapUtil::keyToKey(params, KeyCode::KEYPAD_1, KeyCode::END);
      RemapUtil::keyToKey(params, KeyCode::KEYPAD_3, KeyCode::PAGEDOWN);
      RemapUtil::keyToKey(params, KeyCode::KEYPAD_7, KeyCode::HOME);
      RemapUtil::keyToKey(params, KeyCode::KEYPAD_9, KeyCode::PAGEUP);
    }

    if (params.ex_origKey == KeyCode::KEYPAD_DOT) {
      allFlagStatus.keypad = false;
      RemapUtil::keyToKey(params, KeyCode::KEYPAD_DOT, KeyCode::FORWARD_DELETE);
    }
  }

  void
  remap_pclikehomeend(const RemapParams &params)
  {
    if (! config.remap_pclikehomeend) return;

    bool replaced = false;

    if (*(params.key) == KeyCode::HOME) {
      *(params.key) = KeyCode::CURSOR_LEFT;
      replaced = true;
    }
    if (*(params.key) == KeyCode::END) {
      *(params.key) = KeyCode::CURSOR_RIGHT;
      replaced = true;
    }

    if (replaced) {
      allFlagStatus.commandL.temporary_increase();
      allFlagStatus.cursor = true;
    }
  }

  // ----------------------------------------
  void
  remap_emacsmode(const RemapParams &params)
  {
    if (allFlagStatus.controlL.isHeldDown()) {
      bool cancel_control = false;

      // Control+D -> FORWARD_DELETE
      if (config.option_emacsmode_controlD && *(params.key) == KeyCode::D) {
        *(params.key) = KeyCode::FORWARD_DELETE;
        cancel_control = true;
      }
      // Control+H -> DELETE
      if (config.option_emacsmode_controlH && *(params.key) == KeyCode::H) {
        *(params.key) = KeyCode::DELETE;
        cancel_control = true;
      }
      // Control+I -> TAB
      if (config.option_emacsmode_controlI && *(params.key) == KeyCode::I) {
        *(params.key) = KeyCode::TAB;
        cancel_control = true;
      }
      // Control+M -> RETURN
      if (config.option_emacsmode_controlM && *(params.key) == KeyCode::M) {
        *(params.key) = KeyCode::RETURN;
        cancel_control = true;
      }
      // Control+[ -> ESCAPE
      if (config.option_emacsmode_controlLeftbracket && *(params.key) == KeyCode::BRACKET_LEFT) {
        *(params.key) = KeyCode::ESCAPE;
        cancel_control = true;
      }
      // Control+P -> UP
      if (config.option_emacsmode_controlPNBF && *(params.key) == KeyCode::P) {
        *(params.key) = KeyCode::CURSOR_UP;
        allFlagStatus.cursor = true;
        cancel_control = true;
      }
      // Control+N -> DOWN
      if (config.option_emacsmode_controlPNBF && *(params.key) == KeyCode::N) {
        *(params.key) = KeyCode::CURSOR_DOWN;
        allFlagStatus.cursor = true;
        cancel_control = true;
      }
      // Control+B -> LEFT
      if (config.option_emacsmode_controlPNBF && *(params.key) == KeyCode::B) {
        *(params.key) = KeyCode::CURSOR_LEFT;
        allFlagStatus.cursor = true;
        cancel_control = true;
      }
      // Control+F -> RIGHT
      if (config.option_emacsmode_controlPNBF && *(params.key) == KeyCode::F) {
        *(params.key) = KeyCode::CURSOR_RIGHT;
        allFlagStatus.cursor = true;
        cancel_control = true;
      }
      // Control+V -> PAGEDOWN
      if (config.option_emacsmode_controlV && *(params.key) == KeyCode::V) {
        *(params.key) = KeyCode::PAGEDOWN;
        cancel_control = true;
      }
      // Control+A -> Command+LEFT
      if (config.option_emacsmode_controlAE && *(params.key) == KeyCode::A) {
        *(params.key) = KeyCode::CURSOR_LEFT;
        allFlagStatus.cursor = true;
        allFlagStatus.commandL.temporary_increase();
        cancel_control = true;
      }
      // Control+A -> Command+RIGHT
      if (config.option_emacsmode_controlAE && *(params.key) == KeyCode::E) {
        *(params.key) = KeyCode::CURSOR_RIGHT;
        allFlagStatus.cursor = true;
        allFlagStatus.commandL.temporary_increase();
        cancel_control = true;
      }
      // Control+Q -> PAGEUP
      if (config.option_emacsmode_ex_controlQ && *(params.key) == KeyCode::Q) {
        *(params.key) = KeyCode::PAGEUP;
        cancel_control = true;
      }
      // Control+W -> Option+DELETE
      if (config.option_emacsmode_ex_controlW && *(params.key) == KeyCode::W) {
        *(params.key) = KeyCode::DELETE;
        allFlagStatus.optionL.temporary_increase();
        cancel_control = true;
      }
      // Control+1 -> HOME
      if (config.option_emacsmode_ex_control12 && *(params.key) == KeyCode::KEY_1) {
        *(params.key) = KeyCode::HOME;
        cancel_control = true;
      }
      // Control+2 -> END
      if (config.option_emacsmode_ex_control12 && *(params.key) == KeyCode::KEY_2) {
        *(params.key) = KeyCode::END;
        cancel_control = true;
      }

      if (cancel_control) {
        modifierCanceling_control.keyRelease(params);
        return;
      }
    }

    if (allFlagStatus.optionL.isHeldDown()) {
      bool cancel_option = false;

      // Option+V -> PAGEUP
      if (config.option_emacsmode_controlV && *(params.key) == KeyCode::V) {
        *(params.key) = KeyCode::PAGEUP;
        cancel_option = true;
      }
      // Option+B -> Option+LEFT
      if (config.option_emacsmode_optionBF && *(params.key) == KeyCode::B) {
        *(params.key) = KeyCode::CURSOR_LEFT;
        allFlagStatus.cursor = true;
      }
      // Option+F -> Option+RIGHT
      if (config.option_emacsmode_optionBF && *(params.key) == KeyCode::F) {
        *(params.key) = KeyCode::CURSOR_RIGHT;
        allFlagStatus.cursor = true;
      }
      // Option+D -> Option+FORWARD_DELETE
      if (config.option_emacsmode_optionD && *(params.key) == KeyCode::D) {
        *(params.key) = KeyCode::FORWARD_DELETE;
      }

      if (cancel_option) {
        modifierCanceling_option.keyRelease(params);
        return;
      }
    }

    modifierCanceling_control.restore(params);
    modifierCanceling_option.restore(params);
  }

  // ----------------------------------------
  void
  remap_drop_funcshift(const RemapParams &params)
  {
    if (! config.remap_drop_funcshift) return;

    if (allFlagStatus.shiftL.isHeldDown() || allFlagStatus.shiftR.isHeldDown()) {
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

    if (RemapUtil::al2number(params)) {
      allFlagStatus.controlL.temporary_increase();
      allFlagStatus.optionL.temporary_increase();
      allFlagStatus.shiftL.temporary_increase();
    }
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
      allFlagStatus.commandL.temporary_increase();
      allFlagStatus.controlL.temporary_increase();
      allFlagStatus.optionL.temporary_increase();
      allFlagStatus.shiftL.temporary_increase();
      allFlagStatus.fn.temporary_decrease();
      allFlagStatus.keypad = false;
    }
  }

  void
  remap_spaces_special_keypad(const RemapParams &params)
  {
    if (! config.remap_spaces_special_keypad) return;

    if (allFlagStatus.makeFlags(params) != ModifierFlag::KEYPAD &&
        allFlagStatus.makeFlags(params) != (ModifierFlag::FN | ModifierFlag::KEYPAD)) return;

    if (RemapUtil::keypad2spaces(params)) {
      allFlagStatus.commandL.temporary_increase();
      allFlagStatus.controlL.temporary_increase();
      allFlagStatus.optionL.temporary_increase();
      allFlagStatus.shiftL.temporary_increase();
      allFlagStatus.fn.temporary_decrease();
      allFlagStatus.keypad = false;
    }
  }

  void
  remap_keypad2spaces(const RemapParams &params)
  {
    if (! config.remap_keypad2spaces) return;

    if (config.option_keypad2spaces_modifier_command) {
      if (! allFlagStatus.commandL.isHeldDown() &&
          ! allFlagStatus.commandR.isHeldDown()) return;
    }
    if (config.option_keypad2spaces_modifier_control) {
      if (! allFlagStatus.controlL.isHeldDown() &&
          ! allFlagStatus.controlR.isHeldDown()) return;
    }
    if (config.option_keypad2spaces_modifier_option) {
      if (! allFlagStatus.optionL.isHeldDown() &&
          ! allFlagStatus.optionR.isHeldDown()) return;
    }
    if (config.option_keypad2spaces_modifier_shift) {
      if (! allFlagStatus.shiftL.isHeldDown() &&
          ! allFlagStatus.shiftR.isHeldDown()) return;
    }

    RemapUtil::keypad2spaces(params);
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

  // ----------------------------------------
  void
  remap_pc_application2f9(const RemapParams &params)
  {
    if (! config.remap_pc_application2f9) return;

    RemapUtil::keyToKey(params, KeyCode::PC_APPLICATION, KeyCode::F9);
  }

  void
  remap_pc_application2exposeALL(const RemapParams &params)
  {
    if (! config.remap_pc_application2exposeALL) return;

    RemapUtil::keyToKey(params, KeyCode::PC_APPLICATION, KeyCode::EXPOSE_ALL);
  }

  void
  remap_pc_application2f11(const RemapParams &params)
  {
    if (! config.remap_pc_application2f11) return;

    RemapUtil::keyToKey(params, KeyCode::PC_APPLICATION, KeyCode::F11);
  }

  // ----------------------------------------
  void
  remap_jis_eisuu2commandL(const RemapParams &params)
  {
    if (! config.remap_jis_eisuu2commandL) return;

    RemapUtil::keyToModifier(params, KeyCode::JIS_EISUU, ModifierFlag::COMMAND_L);
  }

  void
  remap_jis_eisuu2controlL(const RemapParams &params)
  {
    if (! config.remap_jis_eisuu2controlL) return;

    RemapUtil::keyToModifier(params, KeyCode::JIS_EISUU, ModifierFlag::CONTROL_L);
  }

  void
  remap_jis_eisuu2optionL(const RemapParams &params)
  {
    if (! config.remap_jis_eisuu2optionL) return;

    RemapUtil::keyToModifier(params, KeyCode::JIS_EISUU, ModifierFlag::OPTION_L);
  }

  void
  remap_jis_eisuu2shiftL(const RemapParams &params)
  {
    if (! config.remap_jis_eisuu2shiftL) return;

    RemapUtil::keyToModifier(params, KeyCode::JIS_EISUU, ModifierFlag::SHIFT_L);
  }

  void
  remap_jis_eisuu2escape(const RemapParams &params)
  {
    if (! config.remap_jis_eisuu2escape) return;

    RemapUtil::keyToKey(params, KeyCode::JIS_EISUU, KeyCode::ESCAPE);
  }

  void
  remap_jis_eisuu2tab(const RemapParams &params)
  {
    if (! config.remap_jis_eisuu2tab) return;

    RemapUtil::keyToKey(params, KeyCode::JIS_EISUU, KeyCode::TAB);
  }

  // ----------------------------------------
  void
  remap_jis_kana2commandL(const RemapParams &params)
  {
    if (! config.remap_jis_kana2commandL) return;

    RemapUtil::keyToModifier(params, KeyCode::JIS_KANA, ModifierFlag::COMMAND_L);
  }

  void
  remap_jis_kana2controlL(const RemapParams &params)
  {
    if (! config.remap_jis_kana2controlL) return;

    RemapUtil::keyToModifier(params, KeyCode::JIS_KANA, ModifierFlag::CONTROL_L);
  }

  void
  remap_jis_kana2optionL(const RemapParams &params)
  {
    if (! config.remap_jis_kana2optionL) return;

    RemapUtil::keyToModifier(params, KeyCode::JIS_KANA, ModifierFlag::OPTION_L);
  }

  void
  remap_jis_kana2shiftL(const RemapParams &params)
  {
    if (! config.remap_jis_kana2shiftL) return;

    RemapUtil::keyToModifier(params, KeyCode::JIS_KANA, ModifierFlag::SHIFT_L);
  }

  void
  remap_jis_kana2return(const RemapParams &params)
  {
    if (! config.remap_jis_kana2return) return;

    RemapUtil::keyToKey(params, KeyCode::JIS_KANA, KeyCode::RETURN);
  }

  // ----------------------------------------
  void
  remap_jis_commandR2eisuu(const RemapParams &params)
  {
    if (! config.remap_jis_commandR2eisuu) return;

    RemapUtil::modifierToKey(params, ModifierFlag::COMMAND_R, KeyCode::JIS_EISUU);
  }

  void
  remap_jis_commandR2kana(const RemapParams &params)
  {
    if (! config.remap_jis_commandR2kana) return;

    RemapUtil::modifierToKey(params, ModifierFlag::COMMAND_R, KeyCode::JIS_KANA);
  }

  void
  remap_jis_optionR2eisuu(const RemapParams &params)
  {
    if (! config.remap_jis_optionR2eisuu) return;

    RemapUtil::modifierToKey(params, ModifierFlag::OPTION_R, KeyCode::JIS_EISUU);
  }

  void
  remap_jis_optionR2kana(const RemapParams &params)
  {
    if (! config.remap_jis_optionR2kana) return;

    RemapUtil::modifierToKey(params, ModifierFlag::OPTION_R, KeyCode::JIS_KANA);
  }

  // ----------------------------------------
  void
  remap_jis_shiftKana2eisuu(const RemapParams &params)
  {
    if (! config.remap_jis_shiftKana2eisuu) return;

    if (params.ex_origKey != KeyCode::JIS_KANA) return;

    if (allFlagStatus.shiftL.isHeldDown() || allFlagStatus.shiftR.isHeldDown()) {
      RemapUtil::keyToKey(params, KeyCode::JIS_KANA, KeyCode::JIS_EISUU);

      if (allFlagStatus.shiftL.isHeldDown()) {
        allFlagStatus.shiftL.temporary_decrease();
      } else {
        allFlagStatus.shiftR.temporary_decrease();
      }
    }
  }

  void
  remap_jis_unify_eisuu_to_kana(const RemapParams &params)
  {
    if (! config.remap_jis_unify_eisuu_to_kana) return;

    RemapUtil::jis_toggle_eisuu_kana(params, KeyCode::JIS_KANA);
  }

  void
  remap_jis_unify_kana_to_eisuu(const RemapParams &params)
  {
    if (! config.remap_jis_unify_kana_to_eisuu) return;

    RemapUtil::jis_toggle_eisuu_kana(params, KeyCode::JIS_EISUU);
  }

  void
  remap_jis_unify_kana_eisuu_to_commandL(const RemapParams &params)
  {
    if (! config.remap_jis_unify_kana_eisuu_to_commandL) return;

    RemapUtil::jis_toggle_eisuu_kana(params, ModifierFlag::COMMAND_L);
  }

  void
  remap_jis_unify_kana_eisuu_to_optionR(const RemapParams &params)
  {
    if (! config.remap_jis_unify_kana_eisuu_to_optionR) return;

    RemapUtil::jis_toggle_eisuu_kana(params, ModifierFlag::OPTION_R);
  }

  void
  remap_jis_commandR2commandR_kana(const RemapParams &params)
  {
    if (! config.remap_jis_commandR2commandR_kana) return;

    static KeyOverlayedModifier kom;
    kom.remap(params, KeyCode::COMMAND_R, ModifierFlag::COMMAND_R, FireFunc::firefunc_jis_kana);
  }

  void
  remap_jis_commandL2commandL_eisuu(const RemapParams &params)
  {
    if (! config.remap_jis_commandL2commandL_eisuu) return;

    static KeyOverlayedModifier kom;
    kom.remap(params, KeyCode::COMMAND_L, ModifierFlag::COMMAND_L, FireFunc::firefunc_jis_eisuu);
  }

  void
  remap_jis_commandL2controlL_eisuu(const RemapParams &params)
  {
    if (! config.remap_jis_commandL2controlL_eisuu) return;

    static KeyOverlayedModifier kom;
    kom.remap(params, KeyCode::COMMAND_L, ModifierFlag::CONTROL_L, FireFunc::firefunc_jis_eisuu);
  }

  void
  remap_jis_commandR_x2_to_kana(const RemapParams &params)
  {
    if (! config.remap_jis_commandR_x2_to_kana) return;

    static DoublePressModifier dpm;
    dpm.remap(params, KeyCode::COMMAND_R, ModifierFlag::COMMAND_R, FireFunc::firefunc_jis_kana);
  }

  void
  remap_jis_commandR_x2_to_kana_x2(const RemapParams &params)
  {
    if (! config.remap_jis_commandR_x2_to_kana_x2) return;

    static DoublePressModifier dpm;
    dpm.remap(params, KeyCode::COMMAND_R, ModifierFlag::COMMAND_R, FireFunc::firefunc_jis_kana_x2);
  }

  void
  remap_jis_commandL_x2_to_eisuu(const RemapParams &params)
  {
    if (! config.remap_jis_commandL_x2_to_eisuu) return;

    static DoublePressModifier dpm;
    dpm.remap(params, KeyCode::COMMAND_L, ModifierFlag::COMMAND_L, FireFunc::firefunc_jis_eisuu);
  }

  void
  remap_jis_commandL_x2_to_eisuu_x2(const RemapParams &params)
  {
    if (! config.remap_jis_commandL_x2_to_eisuu_x2) return;

    static DoublePressModifier dpm;
    dpm.remap(params, KeyCode::COMMAND_L, ModifierFlag::COMMAND_L, FireFunc::firefunc_jis_eisuu_x2);
  }
}

// ----------------------------------------------------------------------
void
org_pqrs_KeyRemap4MacBook::remap_core(const RemapParams &params)
{
  if (config.debug) {
    printf("caught  hid event type %d flags 0x%x key %d ",  *(params.eventType), *(params.flags), *(params.key));
    printf("charCode %d charSet %d ", *(params.charCode), *(params.charSet));
    printf("origCharCode %d origCharSet %d kbdType %d\n",
           *(params.origCharCode), *(params.origCharSet), *(params.keyboardType));
  }

  allFlagStatus.initialize(params);

  // ------------------------------------------------------------
  // normal remapping
  remap_backquote2commandL(params);
  remap_backquote2escape(params);
  remap_backquote2escape_shift2tilde(params);

  remap_backslash2delete(params);

  remap_clear2tab(params);

  remap_controlL2commandL(params);
  remap_controlL2fn(params);
  remap_controlL2optionL(params);
  remap_controlL2shiftL(params);

  remap_commandL2controlL(params);
  remap_commandL2optionL(params);
  remap_commandL2shiftL(params);
  remap_commandL2backquote(params);

  remap_commandR2controlL(params);
  remap_commandR2optionL(params);
  remap_commandR2shiftL(params);

  remap_delete2backslash(params);
  remap_shiftDelete2tilde(params);

  remap_enter2commandL(params);
  remap_enter2controlL(params);
  remap_enter2fn(params);
  remap_enter2optionL(params);
  remap_enter2commandLcontrolL(params);
  remap_enter2commandLshiftL(params);
  remap_enter2controlLoptionL(params);
  remap_enter2semicolon(params);
  remap_enter2space(params);

  remap_escape2backquote(params);
  remap_escape2rightclick(params);

  remap_fn2commandL(params);
  remap_fn2controlL(params);
  remap_fn2optionL(params);
  remap_fn2shiftL(params);

  remap_f16_to_tab(params);

  remap_optionL2commandL(params);
  remap_optionL2shiftL(params);

  remap_optionR2commandL(params);
  remap_optionR2controlL(params);
  remap_optionR2fn(params);
  remap_optionR2enter(params);
  remap_optionR2semicolon(params);
  remap_optionR2space(params);

  remap_return2optionL(params);
  remap_return2optionL_escape(params);
  remap_return2semicolon(params);

  remap_space2controlL(params);
  remap_space2shiftL(params);

  remap_semicolon2return(params);

  remap_shiftL2commandL(params);
  remap_shiftL2controlL(params);
  remap_shiftL2fn(params);
  remap_shiftL2optionL(params);
  remap_shiftL2escape(params);
  remap_shiftL2space(params);

  remap_shiftR2commandL(params);
  remap_shiftR2controlL(params);
  remap_shiftR2fn(params);
  remap_shiftR2optionL(params);
  remap_shiftR2escape(params);
  remap_shiftR2space(params);

  remap_tab2exposeALL(params);

  // ----------------------------------------
  remap_qwerty2colemak(params);

  // ----------------------------------------
  remap_pc_application2exposeALL(params);
  remap_pc_application2f9(params);
  remap_pc_application2f11(params);

  remap_keypadnumlock(params);

  // ----------------------------------------
  remap_hhkmode(params);

  // ------------------------------------------------------------
  // jis
  remap_jis_eisuu2commandL(params);
  remap_jis_eisuu2controlL(params);
  remap_jis_eisuu2optionL(params);
  remap_jis_eisuu2shiftL(params);
  remap_jis_eisuu2escape(params);
  remap_jis_eisuu2tab(params);

  remap_jis_kana2commandL(params);
  remap_jis_kana2controlL(params);
  remap_jis_kana2optionL(params);
  remap_jis_kana2shiftL(params);
  remap_jis_kana2return(params);

  remap_jis_commandR2eisuu(params);
  remap_jis_commandR2kana(params);
  remap_jis_optionR2eisuu(params);
  remap_jis_optionR2kana(params);

  remap_jis_shiftKana2eisuu(params);
  remap_jis_unify_eisuu_to_kana(params);
  remap_jis_unify_kana_to_eisuu(params);
  remap_jis_unify_kana_eisuu_to_commandL(params);
  remap_jis_unify_kana_eisuu_to_optionR(params);
  remap_jis_commandR_x2_to_kana(params);
  remap_jis_commandR_x2_to_kana_x2(params);
  remap_jis_commandL_x2_to_eisuu(params);
  remap_jis_commandL_x2_to_eisuu_x2(params);

  // ------------------------------------------------------------
  // *** Note: we need to call remap_drop_funcshift after tab2f9, pc_application2f11, ... ***
  remap_drop_funcshift(params);

  // ------------------------------------------------------------
  // *** Note: we need to call remap_emacsmode as possible late. ***
  // *** If qwerty2colemak is enabled, Control+H... works with Colemak Keyboard Layout. ***
  remap_emacsmode(params);

  // ----------------------------------------
  // *** Note: we need to call remap_spaces_* after emacsmode. ***
  // *** If spaces_special is enabled, emacsmode_ex_control12 make wrong remappings, . ***
  remap_spaces_special(params);
  remap_spaces_special_fn(params);
  remap_spaces_special_keypad(params);
  remap_keypad2spaces(params);

  // ------------------------------------------------------------
  // *** Note: we need to call remap_space2shift, remap_enter2optionL_commandSpace (has SandS like behavior) as possible late. ***
  // *** If any keyToModifier or modifierToKey remappings are enabled, miss-cancelling are occured. ***
  remap_enter2optionL_commandSpace(params);
  remap_space2controlL_space(params);
  remap_space2shiftL_space(params);
  remap_jis_commandR2commandR_kana(params);
  remap_jis_commandL2commandL_eisuu(params);
  remap_jis_commandL2controlL_eisuu(params);

  // ------------------------------------------------------------
  // *** Note: we need to call remap_pclikehomeend as possible late. ***
  // *** If remap_emacsmode is enable, C-1 & C-2 replaced as HOME, END. ***
  remap_pclikehomeend(params);

  // ------------------------------------------------------------
  *(params.flags) = allFlagStatus.makeFlags(params);

  if (config.debug) {
    if (! *(params.ex_dropKey)) {
      printf("sending hid event type %d flags 0x%x key %d ", *(params.eventType), *(params.flags), *(params.key));
      printf("charCode %d charSet %d ", *(params.charCode), *(params.charSet));
      printf("origCharCode %d origCharSet %d kbdType %d\n",
             *(params.origCharCode), *(params.origCharSet), *(params.keyboardType));
    }
  }
}
