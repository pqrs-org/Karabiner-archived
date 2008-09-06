#ifndef JISKANAMODE_HPP
#define JISKANAMODE_HPP

namespace org_pqrs_KeyRemap4MacBook {
  class JISKanaMode {
  public:
    enum Mode {
      JISKANAMODE_ASCII,
      JISKANAMODE_HIRAGANA,
      JISKANAMODE_KATAKANA,
    };

    Mode getMode(void) const { return mode; }
    void setMode(Mode _mode) { mode = _mode; }
    void setMode(unsigned int eventType, unsigned int keyCode, unsigned int flags);
    void toggle(void);
    bool iskana(void) const { return mode != JISKANAMODE_ASCII; }

  private:
    Mode mode;
  };
}

#endif
