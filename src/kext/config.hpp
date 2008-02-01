#ifndef CONFIG_HPP
#define CONFIG_HPP

namespace org_pqrs_KeyRemap4MacBook {
  class Config {
  public:
    Config() {
      repeat_initial_wait = 500;
      repeat_wait = 30;
    }

    // ----------------------------------------
    int repeat_initial_wait;
    int repeat_wait;

    // ----------------------------------------
    int remap_backquote2commandL;
    int remap_backquote2escape;

    int remap_commandL2controlL;
    int remap_commandL2optionL;
    int remap_commandL2backquote;

    int remap_controlL2commandL;

    int remap_enter2commandL;
    int remap_enter2controlL;
    int remap_enter2fn;
    int remap_enter2optionL;
    int remap_enter2semicolon;
    int remap_enter2space;

    int remap_escape2backquote;
    int remap_escape2rightclick;

    int remap_fn2commandL;
    int remap_fn2controlL;
    int remap_fn2optionL;
    int remap_fn2shiftL;

    int remap_shiftL2controlL;
    int remap_shiftL2space;

    int remap_shiftR2fn;
    int remap_shiftR2escape;
    int remap_shiftR2space;

    int remap_optionL2commandL;
    int remap_optionL2shiftL;

    int remap_optionR2commandL;
    int remap_optionR2controlL;
    int remap_optionR2fn;
    int remap_optionR2enter;
    int remap_optionR2semicolon;
    int remap_optionR2space;


    int remap_space2shift;

    // ----------------------------------------
    int remap_jis_eisuu2commandL;
    int remap_jis_eisuu2controlL;
    int remap_jis_eisuu2optionL;
    int remap_jis_eisuu2shiftL;
    int remap_jis_eisuu2tab;

    int remap_jis_kana2commandL;
    int remap_jis_kana2controlL;
    int remap_jis_kana2optionL;
    int remap_jis_kana2shiftL;
    int remap_jis_kana2return;


    int remap_jis_kanashift2eisuu;
    int remap_jis_unify_kana_eisuu;



    int remap_spaces_special;
    int remap_deleteshift2tilde;
    int remap_qwerty2colemak;
    int remap_return2option;
    int remap_return2semicolon;

    int remap_semicolon2return;

    int remap_drop_funcshift;
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

    int debug;
  };
  extern Config config;

  void sysctl_register(void);
  void sysctl_unregister(void);
}

#endif
