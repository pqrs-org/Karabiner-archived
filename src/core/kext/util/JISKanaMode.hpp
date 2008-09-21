#ifndef JISKANAMODE_HPP
#define JISKANAMODE_HPP

#include "CallBackWrapper.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace JISKanaMode {
    enum Mode {
      JISKANAMODE_ASCII,
      JISKANAMODE_HIRAGANA,
      JISKANAMODE_KATAKANA,
    };

    Mode getMode(void);
    void setMode(Mode newmode);
    void setMode(const Params_KeyboardEventCallBack &params);
    void toggle(void);
    bool iskana(void);
  }
}

#endif
