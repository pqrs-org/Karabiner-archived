#ifndef REMAPUTIL_HPP
#define REMAPUTIL_HPP

#include <IOKit/hidsystem/IOHIKeyboard.h>
#include "remap.hpp"
#include "keycode.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace RemapUtil {
    bool isModifierOn(const RemapParams &params, ModifierFlag::ModifierFlag flag);
    KeyCode::KeyCode getModifierKeyCode(ModifierFlag::ModifierFlag flag);
    ModifierFlag::ModifierFlag getKeyCodeModifier(unsigned int keycode);
    void fnToNormal(const RemapParams &params);
    void toFN(const RemapParams &params);

    KeyCode::KeyCode getEnterKeyCode(const RemapParams &params);

    void modifierToModifier(const RemapParams &params, ModifierFlag::ModifierFlag fromFlag, ModifierFlag::ModifierFlag toFlag);
    void modifierToKey(const RemapParams &params, ModifierFlag::ModifierFlag fromFlag, KeyCode::KeyCode toKeyCode);
    void keyToModifier(const RemapParams &params, KeyCode::KeyCode fromKeyCode, ModifierFlag::ModifierFlag toFlag);
    void keyToModifier(const RemapParams &params, KeyCode::KeyCode fromKeyCode, ModifierFlag::ModifierFlag toFlag1, ModifierFlag::ModifierFlag toFlag2);
    void keyToKey(const RemapParams &params, KeyCode::KeyCode fromKeyCode, KeyCode::KeyCode toKeyCode);

    void jis_toggle_eisuu_kana(const RemapParams &params, KeyCode::KeyCode fromKeyCode);
    void jis_toggle_eisuu_kana(const RemapParams &params, ModifierFlag::ModifierFlag fromModifier);
  }

  class FlagStatus {
  public:
    FlagStatus(ModifierFlag::ModifierFlag _flag) : flag(_flag), key(RemapUtil::getModifierKeyCode(_flag)) {}
    void initialize(const RemapParams &params);

    void reset(void) { count = 0; }
    void increase(void) { ++count; }
    void decrease(void) { --count; }
    void temporary_increase(void) { ++temporary_count; }
    void temporary_decrease(void) { --temporary_count; }

    bool isHeldDown(void) const { return (count + temporary_count) > 0; }
    unsigned int getFlag(void) const { return flag; }
    unsigned int makeFlag(void) const { return (isHeldDown()) ? flag : 0; }

  private:
    const ModifierFlag::ModifierFlag flag;
    const KeyCode::KeyCode key;
    int count;
    int temporary_count;
  };

  class AllFlagStatus {
  public:
    FlagStatus capslock;
    FlagStatus shiftL;
    FlagStatus shiftR;
    FlagStatus controlL;
    FlagStatus controlR;
    FlagStatus optionL;
    FlagStatus optionR;
    FlagStatus commandL;
    FlagStatus commandR;
    FlagStatus fn;
    bool cursor;
    bool keypad;

    int numHeldDownKeys;

    AllFlagStatus(void) :
      capslock(ModifierFlag::CAPSLOCK),
      shiftL(ModifierFlag::SHIFT_L),
      shiftR(ModifierFlag::SHIFT_R),
      controlL(ModifierFlag::CONTROL_L),
      controlR(ModifierFlag::CONTROL_R),
      optionL(ModifierFlag::OPTION_L),
      optionR(ModifierFlag::OPTION_R),
      commandL(ModifierFlag::COMMAND_L),
      commandR(ModifierFlag::COMMAND_R),
      fn(ModifierFlag::FN)
    {}
    void initialize(const RemapParams &params);
    unsigned int makeFlags(const RemapParams &params);
    void reset(void);

    FlagStatus *getFlagStatus(ModifierFlag::ModifierFlag flag);
  };
  extern AllFlagStatus allFlagStatus;

  // ----------------------------------------------------------------------
  class FireExtraKey {
  public:
    enum Type {
      TYPE_BEFORE,
      TYPE_AFTER,
    };
    void set(FireExtraKey::Type _type, unsigned int _eventType, unsigned int _flags, unsigned int _key, unsigned int _charCode) {
      enable = true;
      type = _type;
      eventType = _eventType;
      flags = _flags;
      key = _key;
      charCode = _charCode;
    }
    void unset(void) {
      enable = false;
    }
    bool isEnable(void) const { return enable; }
    unsigned int getType(void) const { return type; }
    unsigned int getEventType(void) const { return eventType; }
    unsigned int getFlags(void) const { return flags; }
    unsigned int getKey(void) const { return key; }
    unsigned int getCharCode(void) const { return charCode; }

  private:
    bool enable;
    Type type;
    unsigned int eventType;
    unsigned int flags;
    unsigned int key;
    unsigned int charCode;
  };

  class ListFireExtraKey {
  public:
    enum {
      FIREEXTRAKEY_MAXNUM = 4,
    };
    void reset(void);
    void add(FireExtraKey::Type type, unsigned int eventType, unsigned int flags, unsigned int key, unsigned int charCode);
    void fire(FireExtraKey::Type type, KeyboardEventAction action, OSObject *target, const RemapParams &params);

  private:
    FireExtraKey list[FIREEXTRAKEY_MAXNUM];
  };

  extern ListFireExtraKey listFireExtraKey;

  // ----------------------------------------------------------------------
  // for emacsmode
  class ModifierCanceling {
  public:
    ModifierCanceling(ModifierFlag::ModifierFlag _modifierFlag) : modifierFlag(_modifierFlag) {
      keycode = RemapUtil::getModifierKeyCode(_modifierFlag);
      flagStatus = allFlagStatus.getFlagStatus(_modifierFlag);
      isSendKeyRelease = false;
    }
    void keyRelease(const RemapParams &params);
    void restore(const RemapParams &params);

  private:
    const ModifierFlag::ModifierFlag modifierFlag;
    KeyCode::KeyCode keycode;
    FlagStatus *flagStatus;

    bool isSendKeyRelease;
  };
  extern ModifierCanceling modifierCanceling_control;
  extern ModifierCanceling modifierCanceling_option;

  // ----------------------------------------------------------------------
  // for SandS like behavior remappings (remap_space2shift, remap_enter2optionL_commandSpace, ...)
  class KeyOverlayedModifier {
  public:
    void remap(const RemapParams &params, KeyCode::KeyCode fromKeyCode, ModifierFlag::ModifierFlag toFlag, void (*firefunc)(const RemapParams &params));

  private:
    bool useAsModifier;
  };

  namespace FireFunc {
    void firefunc_commandSpace(const RemapParams &params);
    void firefunc_space(const RemapParams &params);
    void firefunc_jis_kana(const RemapParams &params);
    void firefunc_jis_eisuu(const RemapParams &params);
  }
}

#endif
