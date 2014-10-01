#ifndef VIRTUALKEY_VK_LAZY_HPP
#define VIRTUALKEY_VK_LAZY_HPP

class KeyCode;
class ModifierFlag;
class Params_KeyboardEventCallBack;

namespace org_pqrs_Karabiner {
namespace VirtualKey {
class VK_LAZY final {
public:
  static bool handle(const Params_KeyboardEventCallBack& params);
  static ModifierFlag getModifierFlag(KeyCode keycode);
};
}
}

#endif
