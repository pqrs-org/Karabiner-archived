#ifndef REMAPUTIL_HPP
#define REMAPUTIL_HPP

#include <IOKit/hidsystem/IOHIKeyboard.h>
#include "remap.hpp"
#include "keycode.hpp"
#include "util/FlagStatus.hpp"
#include "util/PressDownKeys.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace RemapUtil {
    enum {
      // see IOHIPointing.cpp in darwin.
      POINTING_FIXED_SCALE = 65536, // (== << 16)
      POINTING_POINT_SCALE = 10, // (== SCROLL_WHEEL_TO_PIXEL_SCALE >> 16)
    };

    inline bool isKey(const RemapParams &params, unsigned int keyCode) {
      return params.ex_origKey == keyCode && *(params.key) == keyCode;
    }
    inline bool isModifierOn(unsigned int flags, ModifierFlag::ModifierFlag f) {
      return ((flags & f) == f);
    }
    inline bool isModifierOn(const RemapParams &params, ModifierFlag::ModifierFlag flag) {
      return isModifierOn(*(params.flags), flag);
    }

    KeyCode::KeyCode getModifierKeyCode(ModifierFlag::ModifierFlag flag);
    ModifierFlag::ModifierFlag getKeyCodeModifier(unsigned int keycode);

    void normalizeKeyBeforeRemap(unsigned int *key, unsigned int *flags, unsigned int keyboardType);

    inline bool isKeyDown(const RemapParams &params, unsigned int keyCode) {
      if (*(params.eventType) == KeyEvent::DOWN) {
        return isKey(params, keyCode);
      } else if (*(params.eventType) == KeyEvent::MODIFY) {
        return isModifierOn(params, getKeyCodeModifier(keyCode));
      } else {
        return false;
      }
    }

    bool isInternalKeyboard(unsigned int keyboardType);

    bool keyToKey(const RemapParams &params, KeyCode::KeyCode fromKeyCode, unsigned int fromFlags, KeyCode::KeyCode toKeyCode, unsigned int toFlags = ModifierFlag::NONE);
    inline bool keyToKey(const RemapParams &params, KeyCode::KeyCode fromKeyCode, KeyCode::KeyCode toKeyCode, unsigned int toFlags = ModifierFlag::NONE) {
      return keyToKey(params, fromKeyCode, 0, toKeyCode, toFlags);
    }

    bool keyToKey_dependingShift(const RemapParams &params, KeyCode::KeyCode fromKeyCode,
                                 KeyCode::KeyCode toKeyCode_noflag1, CharCode::CharCode toCharCode_noflag1,
                                 KeyCode::KeyCode toKeyCode_noflag2, CharCode::CharCode toCharCode_noflag2,
                                 KeyCode::KeyCode toKeyCode_shiftL1, CharCode::CharCode toCharCode_shiftL1,
                                 KeyCode::KeyCode toKeyCode_shiftL2, CharCode::CharCode toCharCode_shiftL2,
                                 KeyCode::KeyCode toKeyCode_shiftR1, CharCode::CharCode toCharCode_shiftR1,
                                 KeyCode::KeyCode toKeyCode_shiftR2, CharCode::CharCode toCharCode_shiftR2);

    void keyToPointingButton(const RemapParams &params, KeyCode::KeyCode fromKeyCode, PointingButton::PointingButton toButton);

    bool keyToConsumer(const RemapParams &params,
                       KeyCode::KeyCode fromKeyCode, unsigned int fromFlags,
                       ConsumerKeyCode::ConsumerKeyCode toKeyCode, unsigned int toFlags = ModifierFlag::NONE);
    inline bool keyToConsumer(const RemapParams &params,
                              KeyCode::KeyCode fromKeyCode,
                              ConsumerKeyCode::ConsumerKeyCode toKeyCode, unsigned int toFlags = ModifierFlag::NONE) {
      return keyToConsumer(params, fromKeyCode, 0, toKeyCode, toFlags);
    }

    bool consumerToKey(const RemapConsumerParams &params,
                       ConsumerKeyCode::ConsumerKeyCode fromKeyCode, unsigned int fromFlags,
                       KeyCode::KeyCode toKeyCode, unsigned int toFlags = ModifierFlag::NONE);
    inline bool consumerToKey(const RemapConsumerParams &params,
                              ConsumerKeyCode::ConsumerKeyCode fromKeyCode,
                              KeyCode::KeyCode toKeyCode, unsigned int toFlags = ModifierFlag::NONE) {
      return consumerToKey(params, fromKeyCode, 0, toKeyCode, toFlags);
    }

    bool consumerToConsumer(const RemapConsumerParams &params,
                            ConsumerKeyCode::ConsumerKeyCode fromKeyCode, unsigned int fromFlags,
                            ConsumerKeyCode::ConsumerKeyCode toKeyCode, unsigned int toFlags = ModifierFlag::NONE);
    inline bool consumerToConsumer(const RemapConsumerParams &params,
                                   ConsumerKeyCode::ConsumerKeyCode fromKeyCode,
                                   ConsumerKeyCode::ConsumerKeyCode toKeyCode, unsigned int toFlags = ModifierFlag::NONE) {
      return consumerToConsumer(params, fromKeyCode, 0, toKeyCode, toFlags);
    }

    // ----------------------------------------
    void execCallBack_keyboardEventCallBack(KeyboardEventCallback callback, Params_KeyboardEventCallBack params);

    void fireModifiers(unsigned int toFlags,
                       KeyboardEventCallback callback, OSObject *target,
                       unsigned int keyboardType, AbsoluteTime ts, OSObject *sender, void *refcon);
    void fireKey(KeyboardEventCallback callback,
                 OSObject *target, unsigned int eventType, unsigned int flags, unsigned int key, unsigned int charCode,
                 unsigned int charSet, unsigned int origCharCode, unsigned int origCharSet, unsigned int keyboardType,
                 bool repeat, AbsoluteTime ts, OSObject *sender, void *refcon);
    void fireConsumer(KeyboardSpecialEventCallback callback,
                      OSObject *target, unsigned int eventType, unsigned int flags, unsigned int key,
                      unsigned int flavor, UInt64 guid,
                      bool repeat, AbsoluteTime ts, OSObject *sender, void *refcon);

    // ----------------------------------------
    void jis_toggle_eisuu_kana(const RemapParams &params, KeyCode::KeyCode fromKeyCode);

    // ----------------------------------------
    void pointingRelativeToScroll(const RemapPointingParams_relative &params);
  }

  extern AllFlagStatus allFlagStatus;

  // ----------------------------------------------------------------------
  class FireExtraKey {
  public:
    void set(unsigned int _eventType, unsigned int _flags, unsigned int _key, unsigned int _charCode) {
      eventType = _eventType;
      flags = _flags;
      key = _key;
      charCode = _charCode;
    }
    unsigned int getEventType(void) const { return eventType; }
    unsigned int getFlags(void) const { return flags; }
    unsigned int getKey(void) const { return key; }
    unsigned int getCharCode(void) const { return charCode; }

  private:
    unsigned int eventType;
    unsigned int flags;
    unsigned int key;
    unsigned int charCode;
  };

  class ListFireExtraKey {
  public:
    enum {
      FIREEXTRAKEY_MAXNUM = 32,
    };
    void reset(void) { size = 0; }
    bool isEmpty(void) { return size == 0; }
    void add(unsigned int eventType, unsigned int flags, unsigned int key, unsigned int charCode) {
      if (size >= FIREEXTRAKEY_MAXNUM) return;
      list[size].set(eventType, flags, key, charCode);
      ++size;
    }
    void fire(KeyboardEventCallback callback,
              OSObject *target,
              unsigned int charSet, unsigned int origCharCode, unsigned int origCharSet, unsigned int keyboardType,
              AbsoluteTime ts, OSObject *sender, void *refcon);

    // utility
    void addKey(unsigned int flags, KeyCode::KeyCode keyCode, CharCode::CharCode charCode) {
      if (RemapUtil::getKeyCodeModifier(keyCode) != ModifierFlag::NONE) {
        add(KeyEvent::MODIFY, flags, keyCode, charCode);
      } else {
        add(KeyEvent::DOWN, flags, keyCode, charCode);
        add(KeyEvent::UP, flags, keyCode, charCode);
      }
    }

  private:
    FireExtraKey list[FIREEXTRAKEY_MAXNUM];
    int size;
  };

  extern ListFireExtraKey listFireExtraKey;

  // ----------------------------------------------------------------------
  class FireConsumerKey {
  public:
    void set(unsigned int _eventType, unsigned int _flags, unsigned int _key) {
      eventType = _eventType;
      flags = _flags;
      key = _key;
    }
    unsigned int getEventType(void) const { return eventType; }
    unsigned int getFlags(void) const { return flags; }
    unsigned int getKey(void) const { return key; }

  private:
    unsigned int eventType;
    unsigned int flags;
    unsigned int key;
  };

  class ListFireConsumerKey {
  public:
    enum {
      FIRECONSUMERKEY_MAXNUM = 4,
    };
    void reset(void) { size = 0; }
    bool isEmpty(void) { return size == 0; }
    void add(unsigned int eventType, unsigned int flags, unsigned int key) {
      if (size >= FIRECONSUMERKEY_MAXNUM) return;
      list[size].set(eventType, flags, key);
      ++size;
    }
    void fire(KeyboardSpecialEventCallback callback,
              OSObject *target, AbsoluteTime ts, OSObject *sender, void *refcon);

  private:
    FireConsumerKey list[FIRECONSUMERKEY_MAXNUM];
    int size;
  };

  extern ListFireConsumerKey listFireConsumerKey;

  // ----------------------------------------------------------------------
  class IntervalChecker {
  public:
    void begin(void);
    bool checkThreshold(uint32_t millisec);

  private:
    uint32_t secs;
    uint32_t microsecs;
  };

  // ----------------------------------------------------------------------
  namespace FireFunc {
    typedef void (*FireFunc)(const RemapParams &params);
    void firefunc_nop(const RemapParams &params);
    void firefunc_backslash(const RemapParams &params);
    void firefunc_commandO(const RemapParams &params);
    void firefunc_commandSpace(const RemapParams &params);
    void firefunc_enter(const RemapParams &params);
    void firefunc_escape(const RemapParams &params);
    void firefunc_exposeAll(const RemapParams &params);
    void firefunc_return(const RemapParams &params);
    void firefunc_space(const RemapParams &params);
    void firefunc_tab(const RemapParams &params);
    void firefunc_emacsmode_controlK(const RemapParams &params, bool first);
    void firefunc_emacsmode_controlK_2nd(const RemapParams &params);
    void firefunc_emacsmode_ex_controlU(const RemapParams &params);
    void firefunc_jis_kana(const RemapParams &params);
    void firefunc_jis_kana_x2(const RemapParams &params);
    void firefunc_jis_eisuu(const RemapParams &params);
    void firefunc_jis_eisuu_x2(const RemapParams &params);
    void firefunc_jis_toggle_eisuu_kana(const RemapParams &params);
  }

  namespace ExtraRepeatFunc {
    void call_firefunc(FireFunc::FireFunc firefunc, KeyboardEventCallback callback, OSObject *target, unsigned int flags, unsigned int keyboardType, AbsoluteTime ts, OSObject *sender, void *refcon);

    void extraRepeatFunc_enter(KeyboardEventCallback callback, OSObject *target, unsigned int flags, unsigned int keyboardType, AbsoluteTime ts, OSObject *sender, void *refcon);
    void extraRepeatFunc_space(KeyboardEventCallback callback, OSObject *target, unsigned int flags, unsigned int keyboardType, AbsoluteTime ts, OSObject *sender, void *refcon);
    void extraRepeatFunc_emacsmode_controlK(KeyboardEventCallback callback, OSObject *target, unsigned int flags, unsigned int keyboardType, AbsoluteTime ts, OSObject *sender, void *refcon);

    void register_keyCombination(KeyCode::KeyCode keyCode1, CharCode::CharCode charCode1, KeyCode::KeyCode keyCode2, CharCode::CharCode charCode2);
    void extraRepeatFunc_keyCombination(KeyboardEventCallback callback, OSObject *target, unsigned int flags, unsigned int keyboardType, AbsoluteTime ts, OSObject *sender, void *refcon);
  }

  // ----------------------------------------
  // for SandS like behavior remappings (remap_space2shift, remap_enter2optionL_commandSpace, ...)
  class KeyOverlaidModifier {
  public:
    void remap(const RemapParams &params, KeyCode::KeyCode fromKeyCode, ModifierFlag::ModifierFlag toFlag, FireFunc::FireFunc firefunc, ExtraRepeatFunc::ExtraRepeatFunc extraRepeatFunc = NULL);

  private:
    bool useAsModifier;
    bool isClick;
    IntervalChecker ic;
  };

  // ----------------------------------------
  // Command_R+Command_L to Escape, ...
  class KeyOverlaidModifierCombination {
  public:
    void remap(const RemapParams &params, ModifierFlag::ModifierFlag fromFlag1, ModifierFlag::ModifierFlag fromFlag2, FireFunc::FireFunc firefunc);

  private:
    bool isModifier1HeldDown;
    bool isCallFireFunc;
    bool isClick;
  };

  // ----------------------------------------
  // A modifier has DoublePressed key action.
  class DoublePressModifier {
  public:
    void remap(const RemapParams &params, KeyCode::KeyCode fromKeyCode, ModifierFlag::ModifierFlag toFlag, FireFunc::FireFunc firefunc);

  private:
    int pressCount;
  };

  // ----------------------------------------
  // Modifier Holding + Key -> Key
  class ModifierHoldingKeyToKey {
  public:
    void remap(const RemapParams &params, ModifierFlag::ModifierFlag fromFlag, KeyCode::KeyCode fromKeyCode, KeyCode::KeyCode toKeyCode);

  private:
    IntervalChecker ic;
    bool doremap;
    bool first;
  };

  // ----------------------------------------------------------------------
  class ClickWatcher {
  public:
    enum {
      CLICKWATCHER_MAXNUM = 8,
    };
    void reset(void);
    void click(void);

    void set(bool *b);
    void unset(bool *b);

  private:
    bool *watchlist[CLICKWATCHER_MAXNUM];
  };
  extern ClickWatcher clickWatcher;

  // --------------------
  class PointingButtonStatus {
  public:
    bool helddown_left;
    bool helddown_right;
    bool helddown_middle;

    bool *getButtonStatus(PointingButton::PointingButton button);
  };
  extern PointingButtonStatus pointingButtonStatus;

  // --------------------
  class FirePointingClick {
  public:
    void set(PointingButton::PointingButton _button) {
      button = _button;
    }
    PointingButton::PointingButton getButton(void) const { return button; }

  private:
    PointingButton::PointingButton button;
  };

  class ListFirePointingClick {
  public:
    enum {
      FIREPOINTINGCLICK_MAXNUM = 4,
    };
    void reset(void) { size = 0; }
    bool isEmpty(void) { return size == 0; }
    void add(PointingButton::PointingButton button);
    void fire(RelativePointerEventCallback callback, OSObject *target, OSObject *sender, AbsoluteTime ts);

  private:
    FirePointingClick list[FIREPOINTINGCLICK_MAXNUM];
    int size;
  };

  extern ListFirePointingClick listFirePointingClick;

  // --------------------
  class FirePointingScroll {
  public:
    void set(short int _deltaAxis1, short int _deltaAxis2, short int _deltaAxis3, IOFixed _fixedDelta1, IOFixed _fixedDelta2, IOFixed _fixedDelta3, SInt32 _pointDelta1, SInt32 _pointDelta2, SInt32 _pointDelta3) {
      enable = true;
      deltaAxis1 = _deltaAxis1;
      deltaAxis2 = _deltaAxis2;
      deltaAxis3 = _deltaAxis3;
      fixedDelta1 = _fixedDelta1;
      fixedDelta2 = _fixedDelta2;
      fixedDelta3 = _fixedDelta3;
      pointDelta1 = _pointDelta1;
      pointDelta2 = _pointDelta2;
      pointDelta3 = _pointDelta3;
    }
    void fire(ScrollWheelEventCallback callback, OSObject *target, IOHIPointing *pointing, AbsoluteTime ts);
    bool isEnable(void) const { return enable; }

  private:
    bool enable;
    short int deltaAxis1;
    short int deltaAxis2;
    short int deltaAxis3;
    IOFixed fixedDelta1;
    IOFixed fixedDelta2;
    IOFixed fixedDelta3;
    SInt32 pointDelta1;
    SInt32 pointDelta2;
    SInt32 pointDelta3;
  };

  extern FirePointingScroll firePointingScroll;

  // ----------------------------------------
  class ButtonRelativeToScroll {
  public:
    void remap(const RemapPointingParams_relative &params, PointingButton::PointingButton button);

  private:
    bool isButtonHeldDown;
  };

  // --------------------
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
  extern JISKanaMode jisKanaMode;

  extern PressDownKeys pressDownKeys;
}

#endif
