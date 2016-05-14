#pragma once

#include "IOLogWrapper.hpp"
#include "KeyCode.hpp"
#include "strlcpy_utf8.hpp"

namespace org_pqrs_Karabiner {
class ModifierName final {
public:
  class Item final {
  public:
    Item(void) {
      name_[0] = '\0';
      symbol_[0] = '\0';
    }

    Item(ModifierFlag modifierFlag, const char* name, const char* symbol) : modifierFlag_(modifierFlag) {
      if (name) {
        pqrs::strlcpy_utf8::strlcpy(name_, name, sizeof(name_));
      } else {
        name_[0] = '\0';
      }

      if (symbol) {
        pqrs::strlcpy_utf8::strlcpy(symbol_, symbol, sizeof(symbol_));
      } else {
        symbol_[0] = '\0';
      }
    }

    ModifierFlag getModifierFlag(void) const { return modifierFlag_; }
    const char* getName(void) const { return name_; }
    const char* getSymbol(void) const { return symbol_; }

  private:
    enum {
      MAXLEN = 32,
    };

    ModifierFlag modifierFlag_;
    char name_[MAXLEN];
    char symbol_[MAXLEN];
  };
  DECLARE_VECTOR(Item);

  static void initialize(void) {
    clearVirtualModifiers();
  }

  static void clearVirtualModifiers(void);

  static void registerVirtualModifier(ModifierFlag modifierFlag, const char* name) {
    items_.push_back(Item(modifierFlag, name, name));
  }

  static const char* getName(ModifierFlag modifierFlag);
  static const char* getSymbol(ModifierFlag modifierFlag);

private:
  static Vector_Item items_;
};
}
