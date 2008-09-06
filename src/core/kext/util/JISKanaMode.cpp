#include "JISKanaMode.hpp"
#include "RemapUtil.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  void
  JISKanaMode::setMode(unsigned int eventType, unsigned int keyCode, unsigned int flags)
  {
    if (eventType != KeyEvent::DOWN) return;

    if (keyCode == KeyCode::JIS_EISUU) setMode(JISKANAMODE_ASCII);

    if (keyCode == KeyCode::JIS_KANA) {
      if (RemapUtil::isModifierOn(flags, ModifierFlag::SHIFT_L) ||
          RemapUtil::isModifierOn(flags, ModifierFlag::SHIFT_R)) {
        setMode(JISKANAMODE_KATAKANA);
      } else {
        setMode(JISKANAMODE_HIRAGANA);
      }
    }
  }

  void
  JISKanaMode::toggle(void)
  {
    if (mode == JISKANAMODE_ASCII) {
      if (allFlagStatus.isHeldDown_shift()) {
        setMode(JISKANAMODE_KATAKANA);
      } else {
        setMode(JISKANAMODE_HIRAGANA);
      }
    } else {
      setMode(JISKANAMODE_ASCII);
    }
  }
}
