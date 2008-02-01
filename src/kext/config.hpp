#ifndef CONFIG_HPP
#define CONFIG_HPP

namespace org_pqrs_KeyRemap4MacBook {
  class Config {
  public:
    Config() {
      repeat_initial = 500;
      repeat_speed = 30;
    }

    int remap_enter2commandL;
    int remap_enter2controlL;
    int remap_enter2optionL;
    int remap_enter2fn;
    int remap_enter2space;
    int remap_enter2semicolon;

    int remap_optionR2commandL;
    int remap_optionR2controlL;
    int remap_optionR2fn;
    int remap_optionR2space;
    int remap_optionR2semicolon;
    int remap_optionR2enter;

    int remap_fn2commandL;
    int remap_fn2controlL;
    int remap_fn2optionL;
    int remap_fn2shiftL;

    int remap_spaces_special;
    int remap_deleteshift2tilde;
    int remap_qwerty2colemak;
    int remap_return2option;
    int remap_return2semicolon;
    int remap_shift2escape;
    int remap_shift2fn;
    int remap_space2shift;
    int remap_option2command;
    int remap_option2shift;
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
    int option_space2shift_shift2space;
    int option_space2shift_shiftR2space;
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

    int repeat_initial;
    int repeat_speed;

    int debug;
  };
  extern Config config;

  void sysctl_register(void);
  void sysctl_unregister(void);
}

#endif
