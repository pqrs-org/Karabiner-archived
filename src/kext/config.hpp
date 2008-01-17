#ifndef CONFIG_HPP
#define CONFIG_HPP

namespace org_pqrs_KeyRemap4MacBook {
  class Config {
  public:
    int remap_spaces_special;
    int remap_homeposition_functionkey;
    int remap_deleteshift2tilde;
    int remap_qwerty2colemak;
    int remap_return2option;
    int remap_return2semicolon;
    int remap_shift2escape;
    int remap_shift2fn;
    int remap_space2shift;
    int remap_enter2command;
    int remap_enter2control;
    int remap_enter2option;
    int remap_enter2fn;
    int remap_enter2return;
    int remap_enter2space;
    int remap_enter2semicolon;
    int remap_option2command;
    int remap_option2shift;
    int remap_optionr2command;
    int remap_optionr2control;
    int remap_optionr2fn;
    int remap_optionr2space;
    int remap_optionr2semicolon;
    int remap_optionr2enter;
    int remap_fn2command;
    int remap_fn2control;
    int remap_fn2option;
    int remap_fn2shift;
    int remap_escape2tilde;
    int remap_escape2return;
    int remap_escape2rightclick;
    int remap_backquote2command;
    int remap_semicolon2return;
    int remap_commandl2control;
    int remap_commandl2option;
    int remap_shiftl2control;
    int remap_controll2command;
    int remap_drop_funcshift;
    int option_homeposition_functionkey_add_cmd;
    int option_space2shift_shift2space;
    int remap_tab2expose;
    int remap_keypad2spaces;
    int option_keypad2spaces_modifier_command;
    int option_keypad2spaces_modifier_control;
    int option_keypad2spaces_modifier_option;
    int option_keypad2spaces_modifier_shift;
    int remap_hhkmode;
    int remap_emacsmode;
    int option_emacsmode_controlD;
    int option_emacsmode_controlH;
    int option_emacsmode_controlI;
    int option_emacsmode_controlM;
    int option_emacsmode_controlPNBF;
    int option_emacsmode_controlV;
    int option_emacsmode_controlLeftbracket;
    int option_emacsmode_optionBF;
    int option_emacsmode_optionD;
    int option_emacsmode_ex_controlW;
    int option_emacsmode_ex_controlQ;
    int option_emacsmode_ex_control12;

    // for JIS keyboard
    int remap_jis_eisuu2control;
    int remap_jis_eisuu2command;
    int remap_jis_eisuu2option;
    int remap_jis_eisuu2shift;
    int remap_jis_eisuu2tab;
    int remap_jis_kana2control;
    int remap_jis_kana2command;
    int remap_jis_kana2option;
    int remap_jis_kana2shift;
    int remap_jis_kana2return;
    int remap_jis_kanashift2eisuu;
    int remap_jis_unify_kana_eisuu;

    int debug;
  };
  extern Config config;

  void sysctl_register(void);
  void sysctl_unregister(void);
}

#endif
