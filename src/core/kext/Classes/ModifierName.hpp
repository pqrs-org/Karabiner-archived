#ifndef MODIFIERNAME_HPP
#define MODIFIERNAME_HPP

#include "KeyCode.hpp"
#include "strlcpy_utf8.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  class ModifierName {
  public:
    class Item {
    public:
      Item(void) {
        name_[0] = '\0';
      }

      Item(ModifierFlag modifierFlag, const char* name) : modifierFlag_(modifierFlag) {
        pqrs::strlcpy_utf8::strlcpy(name_, name, sizeof(name_));
      }

      Item(ModifierFlag modifierFlag, const uint32_t* name, size_t size) : modifierFlag_(modifierFlag) {
        char* tmp = new char[size + 1];
        {
          for (size_t i = 0; i < size; ++i) {
            tmp[i] = name[i];
          }
          tmp[size] = '\0';

          pqrs::strlcpy_utf8::strlcpy(name_, tmp, sizeof(name_));
        }
        delete[] tmp;
      }

      ModifierFlag getModifierFlag(void) const { return modifierFlag_; }
      const char* getName(void) const { return name_; }

    private:
      enum {
        MAXLEN = 32,
      };

      ModifierFlag modifierFlag_;
      char name_[MAXLEN];
    };
    DECLARE_VECTOR(Item);

    static void initialize(void) {
      clearVirtualModifiers();
    }

    static void clearVirtualModifiers(void);
    static void registerVirtualModifier(ModifierFlag modifierFlag, const uint32_t* name, size_t size);
    static const char* getName(ModifierFlag modifierFlag);

  private:
    static Vector_Item items_;
  };
}

#endif
