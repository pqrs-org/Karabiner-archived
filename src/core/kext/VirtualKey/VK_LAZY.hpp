#ifndef VIRTUALKEY_VK_LAZY_HPP
#define VIRTUALKEY_VK_LAZY_HPP

class KeyCode;
class ModifierFlag;
class Params_KeyboardEventCallBack;

namespace org_pqrs_KeyRemap4MacBook {
  namespace VirtualKey {
    class VK_LAZY {
    public:
      static bool handle(const Params_KeyboardEventCallBack& params);
      static ModifierFlag getModifierFlag(KeyCode keycode);
    };
  }
}

#endif
