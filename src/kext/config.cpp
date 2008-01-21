#include <sys/types.h>
#include <sys/sysctl.h>

#include "config.hpp"
#include "version.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  Config config;
  int config_changed = 0;

  int sysctlFunc SYSCTL_HANDLER_ARGS
  {
    int error = sysctl_handle_int(oidp, oidp->oid_arg1, oidp->oid_arg2,  req);
    if (! error && req->newptr) {
      config_changed = 1;
    }
    return error;
  }

  // ----------------------------------------------------------------------
  // SYSCTL staff
  // http://developer.apple.com/documentation/Darwin/Conceptual/KernelProgramming/boundaries/chapter_14_section_7.html#//apple_ref/doc/uid/TP30000905-CH217-TPXREF116
  SYSCTL_DECL(_keyremap4macbook);
  SYSCTL_NODE(, OID_AUTO, keyremap4macbook, CTLFLAG_RW, 0, "KeyRemap4MacBook");

  SYSCTL_DECL(_keyremap4macbook_remap);
  SYSCTL_NODE(_keyremap4macbook, OID_AUTO, remap, CTLFLAG_RW, 0, "remap");

  SYSCTL_DECL(_keyremap4macbook_option);
  SYSCTL_NODE(_keyremap4macbook, OID_AUTO, option, CTLFLAG_RW, 0, "option");

  SYSCTL_DECL(_keyremap4macbook_repeat);
  SYSCTL_NODE(_keyremap4macbook, OID_AUTO, repeat, CTLFLAG_RW, 0, "repeat");

  SYSCTL_PROC(_keyremap4macbook_remap, OID_AUTO, spaces_special, CTLTYPE_INT|CTLFLAG_RW,
              &(config.remap_spaces_special),
              0, &sysctlFunc, "I",
              "Remap Cmd+A..Cmd+L as 'Cmd+Control+Shift+Option+1 .. Cmd+Control+Shift+Option+9'");

  SYSCTL_PROC(_keyremap4macbook_remap, OID_AUTO, homeposition_functionkey, CTLTYPE_INT|CTLFLAG_RW,
              &(config.remap_homeposition_functionkey),
              0, &sysctlFunc, "I",
              "Remap Cmd+A..Cmd+L as 'F1..F9'");

  SYSCTL_PROC(_keyremap4macbook_option, OID_AUTO, homeposition_functionkey_add_cmd, CTLTYPE_INT|CTLFLAG_RW,
              &(config.option_homeposition_functionkey_add_cmd),
              0, &sysctlFunc, "I",
              "Remap Cmd+A..Cmd+L as 'Cmd+F1..Cmd+F9' (use with homeposition_functionkey) ");

  SYSCTL_PROC(_keyremap4macbook_remap, OID_AUTO, deleteshift2tilde, CTLTYPE_INT|CTLFLAG_RW,
              &(config.remap_deleteshift2tilde),
              0, &sysctlFunc, "I",
              "Remap Shift+Delete as '~'");

  SYSCTL_PROC(_keyremap4macbook_remap, OID_AUTO, qwerty2colemak, CTLTYPE_INT|CTLFLAG_RW,
              &(config.remap_qwerty2colemak),
              0, &sysctlFunc, "I",
              "Turn QWERTY layout to Colemak layout");

  SYSCTL_PROC(_keyremap4macbook_remap, OID_AUTO, return2option, CTLTYPE_INT|CTLFLAG_RW,
              &(config.remap_return2option),
              0, &sysctlFunc, "I",
              "Remap 'Return Key' as 'Command Key'");

  SYSCTL_PROC(_keyremap4macbook_remap, OID_AUTO, return2semicolon, CTLTYPE_INT|CTLFLAG_RW,
              &(config.remap_return2semicolon),
              0, &sysctlFunc, "I",
              "Remap 'Return Key' as 'Semicolon Key'");

  SYSCTL_PROC(_keyremap4macbook_remap, OID_AUTO, shift2escape, CTLTYPE_INT|CTLFLAG_RW,
              &(config.remap_shift2escape),
              0, &sysctlFunc, "I",
              "Remap 'ShiftR Key' as 'Escape Key'");

  SYSCTL_PROC(_keyremap4macbook_remap, OID_AUTO, shift2fn, CTLTYPE_INT|CTLFLAG_RW,
              &(config.remap_shift2fn),
              0, &sysctlFunc, "I",
              "Remap 'ShiftR Key' as 'FN Key'");

  SYSCTL_PROC(_keyremap4macbook_remap, OID_AUTO, space2shift, CTLTYPE_INT|CTLFLAG_RW,
              &(config.remap_space2shift),
              0, &sysctlFunc, "I",
              "Use 'Space Key' as 'Shift Key'");

  SYSCTL_PROC(_keyremap4macbook_option, OID_AUTO, space2shift_shift2space, CTLTYPE_INT|CTLFLAG_RW,
              &(config.option_space2shift_shift2space),
              0, &sysctlFunc, "I",
              "Use ShiftL Key as 'Space Key' (use with space2shift)");

  SYSCTL_PROC(_keyremap4macbook_remap, OID_AUTO, enter2command, CTLTYPE_INT|CTLFLAG_RW,
              &(config.remap_enter2command),
              0, &sysctlFunc, "I",
              "Use 'Enter Key' as 'Command Key'");

  SYSCTL_PROC(_keyremap4macbook_remap, OID_AUTO, enter2control, CTLTYPE_INT|CTLFLAG_RW,
              &(config.remap_enter2control),
              0, &sysctlFunc, "I",
              "Use 'Enter Key' as 'Control Key'");

  SYSCTL_PROC(_keyremap4macbook_remap, OID_AUTO, enter2option, CTLTYPE_INT|CTLFLAG_RW,
              &(config.remap_enter2option),
              0, &sysctlFunc, "I",
              "Use 'Enter Key' as 'Control Key'");

  SYSCTL_PROC(_keyremap4macbook_remap, OID_AUTO, enter2fn, CTLTYPE_INT|CTLFLAG_RW,
              &(config.remap_enter2fn),
              0, &sysctlFunc, "I",
              "Use 'Enter Key' as 'FN Key'");

  SYSCTL_PROC(_keyremap4macbook_remap, OID_AUTO, enter2return, CTLTYPE_INT|CTLFLAG_RW,
              &(config.remap_enter2return),
              0, &sysctlFunc, "I",
              "Use 'Enter Key' as 'Return Key'");

  SYSCTL_PROC(_keyremap4macbook_remap, OID_AUTO, enter2space, CTLTYPE_INT|CTLFLAG_RW,
              &(config.remap_enter2space),
              0, &sysctlFunc, "I",
              "Use 'Enter Key' as 'Space Key'");

  SYSCTL_PROC(_keyremap4macbook_remap, OID_AUTO, enter2semicolon, CTLTYPE_INT|CTLFLAG_RW,
              &(config.remap_enter2semicolon),
              0, &sysctlFunc, "I",
              "Use 'Enter Key' as 'Semicolon Key'");

  SYSCTL_PROC(_keyremap4macbook_remap, OID_AUTO, option2command, CTLTYPE_INT|CTLFLAG_RW,
              &(config.remap_option2command),
              0, &sysctlFunc, "I",
              "Use 'Option Key' as 'Command Key'");

  SYSCTL_PROC(_keyremap4macbook_remap, OID_AUTO, option2shift, CTLTYPE_INT|CTLFLAG_RW,
              &(config.remap_option2shift),
              0, &sysctlFunc, "I",
              "Use 'Option Key' as 'Shift Key'");

  SYSCTL_PROC(_keyremap4macbook_remap, OID_AUTO, optionr2command, CTLTYPE_INT|CTLFLAG_RW,
              &(config.remap_optionr2command),
              0, &sysctlFunc, "I",
              "Use 'Option_R Key' as 'Command Key'");

  SYSCTL_PROC(_keyremap4macbook_remap, OID_AUTO, optionr2control, CTLTYPE_INT|CTLFLAG_RW,
              &(config.remap_optionr2control),
              0, &sysctlFunc, "I",
              "Use 'Option_R Key' as 'Control Key'");

  SYSCTL_PROC(_keyremap4macbook_remap, OID_AUTO, optionr2fn, CTLTYPE_INT|CTLFLAG_RW,
              &(config.remap_optionr2fn),
              0, &sysctlFunc, "I",
              "Use 'Option_R Key' as 'FN Key'");

  SYSCTL_PROC(_keyremap4macbook_remap, OID_AUTO, optionr2space, CTLTYPE_INT|CTLFLAG_RW,
              &(config.remap_optionr2space),
              0, &sysctlFunc, "I",
              "Use 'Option_R Key' as 'Space Key'");

  SYSCTL_PROC(_keyremap4macbook_remap, OID_AUTO, optionr2semicolon, CTLTYPE_INT|CTLFLAG_RW,
              &(config.remap_optionr2semicolon),
              0, &sysctlFunc, "I",
              "Use 'Option_R Key' as 'Semicolon Key'");

  SYSCTL_PROC(_keyremap4macbook_remap, OID_AUTO, optionr2enter, CTLTYPE_INT|CTLFLAG_RW,
              &(config.remap_optionr2enter),
              0, &sysctlFunc, "I",
              "Use 'Option_R Key' as 'Enter Key'");

  SYSCTL_PROC(_keyremap4macbook_remap, OID_AUTO, fn2command, CTLTYPE_INT|CTLFLAG_RW,
              &(config.remap_fn2command),
              0, &sysctlFunc, "I",
              "Use 'FN Key' as 'Command Key'");

  SYSCTL_PROC(_keyremap4macbook_remap, OID_AUTO, fn2control, CTLTYPE_INT|CTLFLAG_RW,
              &(config.remap_fn2control),
              0, &sysctlFunc, "I",
              "Use 'FN Key' as 'Control Key'");

  SYSCTL_PROC(_keyremap4macbook_remap, OID_AUTO, fn2option, CTLTYPE_INT|CTLFLAG_RW,
              &(config.remap_fn2option),
              0, &sysctlFunc, "I",
              "Use 'FN Key' as 'Option Key'");

  SYSCTL_PROC(_keyremap4macbook_remap, OID_AUTO, fn2shift, CTLTYPE_INT|CTLFLAG_RW,
              &(config.remap_fn2shift),
              0, &sysctlFunc, "I",
              "Use 'FN Key' as 'Shift Key'");

  SYSCTL_PROC(_keyremap4macbook_remap, OID_AUTO, backquote2command, CTLTYPE_INT|CTLFLAG_RW,
              &(config.remap_backquote2command),
              0, &sysctlFunc, "I",
              "Use 'Backquote as Command Key'");

  SYSCTL_PROC(_keyremap4macbook_remap, OID_AUTO, escape2tilde, CTLTYPE_INT|CTLFLAG_RW,
              &(config.remap_escape2tilde),
              0, &sysctlFunc, "I",
              "Remap 'Escape Key' as '~'");

  SYSCTL_PROC(_keyremap4macbook_remap, OID_AUTO, escape2return, CTLTYPE_INT|CTLFLAG_RW,
              &(config.remap_escape2return),
              0, &sysctlFunc, "I",
              "Remap 'Escape Key' as 'return'");

  SYSCTL_PROC(_keyremap4macbook_remap, OID_AUTO, escape2rightclick, CTLTYPE_INT|CTLFLAG_RW,
              &(config.remap_escape2rightclick),
              0, &sysctlFunc, "I",
              "Use 'Escape Key' as 'Right Click'");

  SYSCTL_PROC(_keyremap4macbook_remap, OID_AUTO, semicolon2return, CTLTYPE_INT|CTLFLAG_RW,
              &(config.remap_semicolon2return),
              0, &sysctlFunc, "I",
              "Use 'semicolon' as 'return'");

  SYSCTL_PROC(_keyremap4macbook_remap, OID_AUTO, commandl2control, CTLTYPE_INT|CTLFLAG_RW,
              &(config.remap_commandl2control),
              0, &sysctlFunc, "I",
              "Use 'Command_L as Control_L'");

  SYSCTL_PROC(_keyremap4macbook_remap, OID_AUTO, commandl2option, CTLTYPE_INT|CTLFLAG_RW,
              &(config.remap_commandl2option),
              0, &sysctlFunc, "I",
              "Use 'Command_L as Option_L'");

  SYSCTL_PROC(_keyremap4macbook_remap, OID_AUTO, shiftl2control, CTLTYPE_INT|CTLFLAG_RW,
              &(config.remap_shiftl2control),
              0, &sysctlFunc, "I",
              "Use 'Shift_L as Control_L'");

  SYSCTL_PROC(_keyremap4macbook_remap, OID_AUTO, controll2command, CTLTYPE_INT|CTLFLAG_RW,
              &(config.remap_controll2command),
              0, &sysctlFunc, "I",
              "Use 'Control_L as Command_L'");

  SYSCTL_PROC(_keyremap4macbook_remap, OID_AUTO, drop_funcshift, CTLTYPE_INT|CTLFLAG_RW,
              &(config.remap_drop_funcshift),
              0, &sysctlFunc, "I",
              "Disable 'Shift + F1..F12'");

  SYSCTL_PROC(_keyremap4macbook_remap, OID_AUTO, tab2expose, CTLTYPE_INT|CTLFLAG_RW,
              &(config.remap_tab2expose),
              0, &sysctlFunc, "I",
              "Use 'Tab' as F9");

  // ----------------------------------------------------------------------
  SYSCTL_PROC(_keyremap4macbook_remap, OID_AUTO, keypad2spaces, CTLTYPE_INT|CTLFLAG_RW,
              &(config.remap_keypad2spaces),
              0, &sysctlFunc, "I",
              "Use 'KeyPad' for Spaces");

  SYSCTL_PROC(_keyremap4macbook_option, OID_AUTO, keypad2spaces_modifier_command, CTLTYPE_INT|CTLFLAG_RW,
              &(config.option_keypad2spaces_modifier_command),
              0, &sysctlFunc, "I",
              "Use 'KeyPad' for Spaces");

  SYSCTL_PROC(_keyremap4macbook_option, OID_AUTO, keypad2spaces_modifier_control, CTLTYPE_INT|CTLFLAG_RW,
              &(config.option_keypad2spaces_modifier_control),
              0, &sysctlFunc, "I",
              "Use 'KeyPad' for Spaces");

  SYSCTL_PROC(_keyremap4macbook_option, OID_AUTO, keypad2spaces_modifier_option, CTLTYPE_INT|CTLFLAG_RW,
              &(config.option_keypad2spaces_modifier_option),
              0, &sysctlFunc, "I",
              "Use 'KeyPad' for Spaces");

  SYSCTL_PROC(_keyremap4macbook_option, OID_AUTO, keypad2spaces_modifier_shift, CTLTYPE_INT|CTLFLAG_RW,
              &(config.option_keypad2spaces_modifier_shift),
              0, &sysctlFunc, "I",
              "Use 'KeyPad' for Spaces");

  // ----------------------------------------------------------------------
  SYSCTL_PROC(_keyremap4macbook_remap, OID_AUTO, hhkmode, CTLTYPE_INT|CTLFLAG_RW,
              &(config.remap_hhkmode),
              0, &sysctlFunc, "I",
              "Use 'FH +[/;'' as 'up/down/left/right'");

  // ----------------------------------------------------------------------
  SYSCTL_PROC(_keyremap4macbook_remap, OID_AUTO, emacsmode, CTLTYPE_INT|CTLFLAG_RW,
              &(config.remap_emacsmode),
              0, &sysctlFunc, "I",
              "Emacs keybind");

  SYSCTL_PROC(_keyremap4macbook_option, OID_AUTO, emacsmode_controlD, CTLTYPE_INT|CTLFLAG_RW,
              &(config.option_emacsmode_controlD),
              0, &sysctlFunc, "I",
              "Emacs keybind 'Control + D' as 'forward delete'");

  SYSCTL_PROC(_keyremap4macbook_option, OID_AUTO, emacsmode_controlH, CTLTYPE_INT|CTLFLAG_RW,
              &(config.option_emacsmode_controlH),
              0, &sysctlFunc, "I",
              "Emacs keybind 'Control + H' as 'delete'");

  SYSCTL_PROC(_keyremap4macbook_option, OID_AUTO, emacsmode_controlI, CTLTYPE_INT|CTLFLAG_RW,
              &(config.option_emacsmode_controlI),
              0, &sysctlFunc, "I",
              "Emacs keybind 'Control + I' as 'tab'");

  SYSCTL_PROC(_keyremap4macbook_option, OID_AUTO, emacsmode_controlM, CTLTYPE_INT|CTLFLAG_RW,
              &(config.option_emacsmode_controlM),
              0, &sysctlFunc, "I",
              "Emacs keybind 'Control + M' as 'return'");

  SYSCTL_PROC(_keyremap4macbook_option, OID_AUTO, emacsmode_controlPNBF, CTLTYPE_INT|CTLFLAG_RW,
              &(config.option_emacsmode_controlPNBF),
              0, &sysctlFunc, "I",
              "Emacs keybind 'Control + P, N, B, F' as 'up, down, left, right'");

  SYSCTL_PROC(_keyremap4macbook_option, OID_AUTO, emacsmode_controlV, CTLTYPE_INT|CTLFLAG_RW,
              &(config.option_emacsmode_controlV),
              0, &sysctlFunc, "I",
              "Emacs keybind 'Control + V' as 'PageDown', 'Option + V' as 'PageUp'");

  SYSCTL_PROC(_keyremap4macbook_option, OID_AUTO, emacsmode_controlLeftbracket, CTLTYPE_INT|CTLFLAG_RW,
              &(config.option_emacsmode_controlLeftbracket),
              0, &sysctlFunc, "I",
              "Emacs keybind 'Control + [' as 'Escape'");

  SYSCTL_PROC(_keyremap4macbook_option, OID_AUTO, emacsmode_optionBF, CTLTYPE_INT|CTLFLAG_RW,
              &(config.option_emacsmode_optionBF),
              0, &sysctlFunc, "I",
              "Emacs keybind 'Option + B, F' as 'Option + left, right'");

  SYSCTL_PROC(_keyremap4macbook_option, OID_AUTO, emacsmode_optionD, CTLTYPE_INT|CTLFLAG_RW,
              &(config.option_emacsmode_optionD),
              0, &sysctlFunc, "I",
              "Emacs keybind 'Option + D' as 'Option + FORWARD_DELETE'");

  SYSCTL_PROC(_keyremap4macbook_option, OID_AUTO, emacsmode_ex_controlQ, CTLTYPE_INT|CTLFLAG_RW,
              &(config.option_emacsmode_ex_controlQ),
              0, &sysctlFunc, "I",
              "Emacs keybind 'Control + Q' as 'PageUp'");

  SYSCTL_PROC(_keyremap4macbook_option, OID_AUTO, emacsmode_ex_controlW, CTLTYPE_INT|CTLFLAG_RW,
              &(config.option_emacsmode_ex_controlW),
              0, &sysctlFunc, "I",
              "Emacs keybind 'Control + W' as 'Option + DELETE'");

  SYSCTL_PROC(_keyremap4macbook_option, OID_AUTO, emacsmode_ex_control12, CTLTYPE_INT|CTLFLAG_RW,
              &(config.option_emacsmode_ex_control12),
              0, &sysctlFunc, "I",
              "Emacs keybind 'Control + 1' as 'HOME', 'Control + 2' as 'END'");

  // ----------------------------------------------------------------------
  SYSCTL_PROC(_keyremap4macbook_remap, OID_AUTO, jis_eisuu2control, CTLTYPE_INT|CTLFLAG_RW,
              &(config.remap_jis_eisuu2control),
              0, &sysctlFunc, "I",
              "Use 'JIS EISUU Key' as 'Control Key'");

  SYSCTL_PROC(_keyremap4macbook_remap, OID_AUTO, jis_eisuu2command, CTLTYPE_INT|CTLFLAG_RW,
              &(config.remap_jis_eisuu2command),
              0, &sysctlFunc, "I",
              "Use 'JIS EISUU Key' as 'Command Key'");

  SYSCTL_PROC(_keyremap4macbook_remap, OID_AUTO, jis_eisuu2option, CTLTYPE_INT|CTLFLAG_RW,
              &(config.remap_jis_eisuu2option),
              0, &sysctlFunc, "I",
              "Use 'JIS EISUU Key' as 'Option Key'");

  SYSCTL_PROC(_keyremap4macbook_remap, OID_AUTO, jis_eisuu2shift, CTLTYPE_INT|CTLFLAG_RW,
              &(config.remap_jis_eisuu2shift),
              0, &sysctlFunc, "I",
              "Use 'JIS EISUU Key' as 'Shift Key'");

  SYSCTL_PROC(_keyremap4macbook_remap, OID_AUTO, jis_eisuu2tab, CTLTYPE_INT|CTLFLAG_RW,
              &(config.remap_jis_eisuu2tab),
              0, &sysctlFunc, "I",
              "Use 'JIS EISUU Key' as 'Tab Key'");

  SYSCTL_PROC(_keyremap4macbook_remap, OID_AUTO, jis_kana2control, CTLTYPE_INT|CTLFLAG_RW,
              &(config.remap_jis_kana2control),
              0, &sysctlFunc, "I",
              "Use 'JIS KANA Key' as 'Control Key'");

  SYSCTL_PROC(_keyremap4macbook_remap, OID_AUTO, jis_kana2command, CTLTYPE_INT|CTLFLAG_RW,
              &(config.remap_jis_kana2command),
              0, &sysctlFunc, "I",
              "Use 'JIS KANA Key' as 'Command Key'");

  SYSCTL_PROC(_keyremap4macbook_remap, OID_AUTO, jis_kana2option, CTLTYPE_INT|CTLFLAG_RW,
              &(config.remap_jis_kana2option),
              0, &sysctlFunc, "I",
              "Use 'JIS KANA Key' as 'Option Key'");

  SYSCTL_PROC(_keyremap4macbook_remap, OID_AUTO, jis_kana2shift, CTLTYPE_INT|CTLFLAG_RW,
              &(config.remap_jis_kana2shift),
              0, &sysctlFunc, "I",
              "Use 'JIS KANA Key' as 'Shift Key'");

  SYSCTL_PROC(_keyremap4macbook_remap, OID_AUTO, jis_kana2return, CTLTYPE_INT|CTLFLAG_RW,
              &(config.remap_jis_kana2return),
              0, &sysctlFunc, "I",
              "Use 'JIS KANA Key' as 'Return Key'");

  SYSCTL_PROC(_keyremap4macbook_remap, OID_AUTO, jis_kanashift2eisuu, CTLTYPE_INT|CTLFLAG_RW,
              &(config.remap_jis_kanashift2eisuu),
              0, &sysctlFunc, "I",
              "Remap 'Shift + JIS KANA' as 'JIS EISUU'");

  SYSCTL_PROC(_keyremap4macbook_remap, OID_AUTO, jis_unify_kana_eisuu, CTLTYPE_INT|CTLFLAG_RW,
              &(config.remap_jis_unify_kana_eisuu),
              0, &sysctlFunc, "I",
              "Use 'JIS KANA Key' as 'KANA / EISUU'");

  SYSCTL_PROC(_keyremap4macbook_repeat, OID_AUTO, initial, CTLTYPE_INT|CTLFLAG_RW,
              &(config.repeat_initial),
              0, &sysctlFunc, "I",
              "Initial Time to Start Key Repeat");

  SYSCTL_PROC(_keyremap4macbook_repeat, OID_AUTO, speed, CTLTYPE_INT|CTLFLAG_RW,
              &(config.repeat_speed),
              0, &sysctlFunc, "I",
              "Key Repeat Speed");

  SYSCTL_INT(_keyremap4macbook, OID_AUTO, debug, CTLTYPE_INT|CTLFLAG_RW,
             &(config.debug),
             0,
             "Output Debug Messages");

  SYSCTL_INT(_keyremap4macbook, OID_AUTO, changed, CTLTYPE_INT|CTLFLAG_RW,
             &config_changed,
             0,
             "Setting Changed Flag");

  SYSCTL_STRING(_keyremap4macbook, OID_AUTO, version, CTLFLAG_RD,
                config_version,
                0,
                "Output Version");

  // ----------------------------------------------------------------------
  void
  sysctl_register(void)
  {
    sysctl_register_oid(&sysctl__keyremap4macbook);
    sysctl_register_oid(&sysctl__keyremap4macbook_remap);
    sysctl_register_oid(&sysctl__keyremap4macbook_option);
    sysctl_register_oid(&sysctl__keyremap4macbook_repeat);
    sysctl_register_oid(&sysctl__keyremap4macbook_remap_commandl2control);
    sysctl_register_oid(&sysctl__keyremap4macbook_remap_commandl2option);
    sysctl_register_oid(&sysctl__keyremap4macbook_remap_shiftl2control);
    sysctl_register_oid(&sysctl__keyremap4macbook_remap_controll2command);
    sysctl_register_oid(&sysctl__keyremap4macbook_remap_option2command);
    sysctl_register_oid(&sysctl__keyremap4macbook_remap_option2shift);
    sysctl_register_oid(&sysctl__keyremap4macbook_remap_optionr2command);
    sysctl_register_oid(&sysctl__keyremap4macbook_remap_optionr2control);
    sysctl_register_oid(&sysctl__keyremap4macbook_remap_optionr2fn);
    sysctl_register_oid(&sysctl__keyremap4macbook_remap_optionr2space);
    sysctl_register_oid(&sysctl__keyremap4macbook_remap_optionr2semicolon);
    sysctl_register_oid(&sysctl__keyremap4macbook_remap_optionr2enter);
    sysctl_register_oid(&sysctl__keyremap4macbook_remap_fn2command);
    sysctl_register_oid(&sysctl__keyremap4macbook_remap_fn2control);
    sysctl_register_oid(&sysctl__keyremap4macbook_remap_fn2option);
    sysctl_register_oid(&sysctl__keyremap4macbook_remap_fn2shift);
    sysctl_register_oid(&sysctl__keyremap4macbook_remap_spaces_special);
    sysctl_register_oid(&sysctl__keyremap4macbook_remap_homeposition_functionkey);
    sysctl_register_oid(&sysctl__keyremap4macbook_option_homeposition_functionkey_add_cmd);
    sysctl_register_oid(&sysctl__keyremap4macbook_remap_deleteshift2tilde);
    sysctl_register_oid(&sysctl__keyremap4macbook_remap_qwerty2colemak);
    sysctl_register_oid(&sysctl__keyremap4macbook_remap_return2option);
    sysctl_register_oid(&sysctl__keyremap4macbook_remap_return2semicolon);
    sysctl_register_oid(&sysctl__keyremap4macbook_remap_shift2escape);
    sysctl_register_oid(&sysctl__keyremap4macbook_remap_shift2fn);
    sysctl_register_oid(&sysctl__keyremap4macbook_remap_space2shift);
    sysctl_register_oid(&sysctl__keyremap4macbook_option_space2shift_shift2space);
    sysctl_register_oid(&sysctl__keyremap4macbook_remap_enter2command);
    sysctl_register_oid(&sysctl__keyremap4macbook_remap_enter2control);
    sysctl_register_oid(&sysctl__keyremap4macbook_remap_enter2option);
    sysctl_register_oid(&sysctl__keyremap4macbook_remap_enter2fn);
    sysctl_register_oid(&sysctl__keyremap4macbook_remap_enter2return);
    sysctl_register_oid(&sysctl__keyremap4macbook_remap_enter2space);
    sysctl_register_oid(&sysctl__keyremap4macbook_remap_enter2semicolon);
    sysctl_register_oid(&sysctl__keyremap4macbook_remap_backquote2command);
    sysctl_register_oid(&sysctl__keyremap4macbook_remap_escape2tilde);
    sysctl_register_oid(&sysctl__keyremap4macbook_remap_escape2return);
    sysctl_register_oid(&sysctl__keyremap4macbook_remap_escape2rightclick);
    sysctl_register_oid(&sysctl__keyremap4macbook_remap_semicolon2return);
    sysctl_register_oid(&sysctl__keyremap4macbook_remap_drop_funcshift);
    sysctl_register_oid(&sysctl__keyremap4macbook_remap_tab2expose);
    sysctl_register_oid(&sysctl__keyremap4macbook_remap_keypad2spaces);
    sysctl_register_oid(&sysctl__keyremap4macbook_option_keypad2spaces_modifier_command);
    sysctl_register_oid(&sysctl__keyremap4macbook_option_keypad2spaces_modifier_control);
    sysctl_register_oid(&sysctl__keyremap4macbook_option_keypad2spaces_modifier_option);
    sysctl_register_oid(&sysctl__keyremap4macbook_option_keypad2spaces_modifier_shift);
    sysctl_register_oid(&sysctl__keyremap4macbook_remap_hhkmode);
    sysctl_register_oid(&sysctl__keyremap4macbook_remap_emacsmode);
    sysctl_register_oid(&sysctl__keyremap4macbook_option_emacsmode_controlD);
    sysctl_register_oid(&sysctl__keyremap4macbook_option_emacsmode_controlH);
    sysctl_register_oid(&sysctl__keyremap4macbook_option_emacsmode_controlI);
    sysctl_register_oid(&sysctl__keyremap4macbook_option_emacsmode_controlM);
    sysctl_register_oid(&sysctl__keyremap4macbook_option_emacsmode_controlPNBF);
    sysctl_register_oid(&sysctl__keyremap4macbook_option_emacsmode_controlV);
    sysctl_register_oid(&sysctl__keyremap4macbook_option_emacsmode_controlLeftbracket);
    sysctl_register_oid(&sysctl__keyremap4macbook_option_emacsmode_optionBF);
    sysctl_register_oid(&sysctl__keyremap4macbook_option_emacsmode_optionD);
    sysctl_register_oid(&sysctl__keyremap4macbook_option_emacsmode_ex_controlQ);
    sysctl_register_oid(&sysctl__keyremap4macbook_option_emacsmode_ex_controlW);
    sysctl_register_oid(&sysctl__keyremap4macbook_option_emacsmode_ex_control12);
    sysctl_register_oid(&sysctl__keyremap4macbook_remap_jis_eisuu2control);
    sysctl_register_oid(&sysctl__keyremap4macbook_remap_jis_eisuu2command);
    sysctl_register_oid(&sysctl__keyremap4macbook_remap_jis_eisuu2option);
    sysctl_register_oid(&sysctl__keyremap4macbook_remap_jis_eisuu2shift);
    sysctl_register_oid(&sysctl__keyremap4macbook_remap_jis_eisuu2tab);
    sysctl_register_oid(&sysctl__keyremap4macbook_remap_jis_kana2control);
    sysctl_register_oid(&sysctl__keyremap4macbook_remap_jis_kana2command);
    sysctl_register_oid(&sysctl__keyremap4macbook_remap_jis_kana2option);
    sysctl_register_oid(&sysctl__keyremap4macbook_remap_jis_kana2shift);
    sysctl_register_oid(&sysctl__keyremap4macbook_remap_jis_kana2return);
    sysctl_register_oid(&sysctl__keyremap4macbook_remap_jis_kanashift2eisuu);
    sysctl_register_oid(&sysctl__keyremap4macbook_remap_jis_unify_kana_eisuu);
    sysctl_register_oid(&sysctl__keyremap4macbook_repeat_initial);
    sysctl_register_oid(&sysctl__keyremap4macbook_repeat_speed);
    sysctl_register_oid(&sysctl__keyremap4macbook_debug);
    sysctl_register_oid(&sysctl__keyremap4macbook_changed);
    sysctl_register_oid(&sysctl__keyremap4macbook_version);
  }

  void
  sysctl_unregister(void)
  {
    sysctl_unregister_oid(&sysctl__keyremap4macbook);
    sysctl_unregister_oid(&sysctl__keyremap4macbook_remap);
    sysctl_unregister_oid(&sysctl__keyremap4macbook_option);
    sysctl_unregister_oid(&sysctl__keyremap4macbook_repeat);
    sysctl_unregister_oid(&sysctl__keyremap4macbook_remap_commandl2control);
    sysctl_unregister_oid(&sysctl__keyremap4macbook_remap_commandl2option);
    sysctl_unregister_oid(&sysctl__keyremap4macbook_remap_shiftl2control);
    sysctl_unregister_oid(&sysctl__keyremap4macbook_remap_controll2command);
    sysctl_unregister_oid(&sysctl__keyremap4macbook_remap_option2command);
    sysctl_unregister_oid(&sysctl__keyremap4macbook_remap_option2shift);
    sysctl_unregister_oid(&sysctl__keyremap4macbook_remap_optionr2command);
    sysctl_unregister_oid(&sysctl__keyremap4macbook_remap_optionr2control);
    sysctl_unregister_oid(&sysctl__keyremap4macbook_remap_optionr2fn);
    sysctl_unregister_oid(&sysctl__keyremap4macbook_remap_optionr2space);
    sysctl_unregister_oid(&sysctl__keyremap4macbook_remap_optionr2semicolon);
    sysctl_unregister_oid(&sysctl__keyremap4macbook_remap_optionr2enter);
    sysctl_unregister_oid(&sysctl__keyremap4macbook_remap_fn2command);
    sysctl_unregister_oid(&sysctl__keyremap4macbook_remap_fn2control);
    sysctl_unregister_oid(&sysctl__keyremap4macbook_remap_fn2option);
    sysctl_unregister_oid(&sysctl__keyremap4macbook_remap_fn2shift);
    sysctl_unregister_oid(&sysctl__keyremap4macbook_remap_spaces_special);
    sysctl_unregister_oid(&sysctl__keyremap4macbook_remap_homeposition_functionkey);
    sysctl_unregister_oid(&sysctl__keyremap4macbook_option_homeposition_functionkey_add_cmd);
    sysctl_unregister_oid(&sysctl__keyremap4macbook_remap_deleteshift2tilde);
    sysctl_unregister_oid(&sysctl__keyremap4macbook_remap_qwerty2colemak);
    sysctl_unregister_oid(&sysctl__keyremap4macbook_remap_return2option);
    sysctl_unregister_oid(&sysctl__keyremap4macbook_remap_return2semicolon);
    sysctl_unregister_oid(&sysctl__keyremap4macbook_remap_shift2escape);
    sysctl_unregister_oid(&sysctl__keyremap4macbook_remap_shift2fn);
    sysctl_unregister_oid(&sysctl__keyremap4macbook_option_space2shift_shift2space);
    sysctl_unregister_oid(&sysctl__keyremap4macbook_remap_space2shift);
    sysctl_unregister_oid(&sysctl__keyremap4macbook_remap_enter2command);
    sysctl_unregister_oid(&sysctl__keyremap4macbook_remap_enter2control);
    sysctl_unregister_oid(&sysctl__keyremap4macbook_remap_enter2option);
    sysctl_unregister_oid(&sysctl__keyremap4macbook_remap_enter2fn);
    sysctl_unregister_oid(&sysctl__keyremap4macbook_remap_enter2return);
    sysctl_unregister_oid(&sysctl__keyremap4macbook_remap_enter2space);
    sysctl_unregister_oid(&sysctl__keyremap4macbook_remap_enter2semicolon);
    sysctl_unregister_oid(&sysctl__keyremap4macbook_remap_backquote2command);
    sysctl_unregister_oid(&sysctl__keyremap4macbook_remap_escape2tilde);
    sysctl_unregister_oid(&sysctl__keyremap4macbook_remap_escape2return);
    sysctl_unregister_oid(&sysctl__keyremap4macbook_remap_escape2rightclick);
    sysctl_unregister_oid(&sysctl__keyremap4macbook_remap_semicolon2return);
    sysctl_unregister_oid(&sysctl__keyremap4macbook_remap_drop_funcshift);
    sysctl_unregister_oid(&sysctl__keyremap4macbook_remap_tab2expose);
    sysctl_unregister_oid(&sysctl__keyremap4macbook_remap_keypad2spaces);
    sysctl_unregister_oid(&sysctl__keyremap4macbook_option_keypad2spaces_modifier_command);
    sysctl_unregister_oid(&sysctl__keyremap4macbook_option_keypad2spaces_modifier_control);
    sysctl_unregister_oid(&sysctl__keyremap4macbook_option_keypad2spaces_modifier_option);
    sysctl_unregister_oid(&sysctl__keyremap4macbook_option_keypad2spaces_modifier_shift);
    sysctl_unregister_oid(&sysctl__keyremap4macbook_remap_hhkmode);
    sysctl_unregister_oid(&sysctl__keyremap4macbook_remap_emacsmode);
    sysctl_unregister_oid(&sysctl__keyremap4macbook_option_emacsmode_controlD);
    sysctl_unregister_oid(&sysctl__keyremap4macbook_option_emacsmode_controlH);
    sysctl_unregister_oid(&sysctl__keyremap4macbook_option_emacsmode_controlI);
    sysctl_unregister_oid(&sysctl__keyremap4macbook_option_emacsmode_controlM);
    sysctl_unregister_oid(&sysctl__keyremap4macbook_option_emacsmode_controlPNBF);
    sysctl_unregister_oid(&sysctl__keyremap4macbook_option_emacsmode_controlV);
    sysctl_unregister_oid(&sysctl__keyremap4macbook_option_emacsmode_controlLeftbracket);
    sysctl_unregister_oid(&sysctl__keyremap4macbook_option_emacsmode_optionBF);
    sysctl_unregister_oid(&sysctl__keyremap4macbook_option_emacsmode_optionD);
    sysctl_unregister_oid(&sysctl__keyremap4macbook_option_emacsmode_ex_controlQ);
    sysctl_unregister_oid(&sysctl__keyremap4macbook_option_emacsmode_ex_controlW);
    sysctl_unregister_oid(&sysctl__keyremap4macbook_option_emacsmode_ex_control12);
    sysctl_unregister_oid(&sysctl__keyremap4macbook_remap_jis_eisuu2control);
    sysctl_unregister_oid(&sysctl__keyremap4macbook_remap_jis_eisuu2command);
    sysctl_unregister_oid(&sysctl__keyremap4macbook_remap_jis_eisuu2option);
    sysctl_unregister_oid(&sysctl__keyremap4macbook_remap_jis_eisuu2shift);
    sysctl_unregister_oid(&sysctl__keyremap4macbook_remap_jis_eisuu2tab);
    sysctl_unregister_oid(&sysctl__keyremap4macbook_remap_jis_kana2control);
    sysctl_unregister_oid(&sysctl__keyremap4macbook_remap_jis_kana2command);
    sysctl_unregister_oid(&sysctl__keyremap4macbook_remap_jis_kana2option);
    sysctl_unregister_oid(&sysctl__keyremap4macbook_remap_jis_kana2shift);
    sysctl_unregister_oid(&sysctl__keyremap4macbook_remap_jis_kana2return);
    sysctl_unregister_oid(&sysctl__keyremap4macbook_remap_jis_kanashift2eisuu);
    sysctl_unregister_oid(&sysctl__keyremap4macbook_remap_jis_unify_kana_eisuu);
    sysctl_unregister_oid(&sysctl__keyremap4macbook_repeat_initial);
    sysctl_unregister_oid(&sysctl__keyremap4macbook_repeat_speed);
    sysctl_unregister_oid(&sysctl__keyremap4macbook_debug);
    sysctl_unregister_oid(&sysctl__keyremap4macbook_changed);
    sysctl_unregister_oid(&sysctl__keyremap4macbook_version);
  }
}
