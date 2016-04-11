#pragma once

class KeyCode;
class ModifierFlag;
class Params_KeyboardEventCallBack;
class AutogenId;

namespace org_pqrs_Karabiner {
namespace VirtualKey {
class VK_LAZY final {
public:
  static bool handle(const Params_KeyboardEventCallBack& params, AutogenId autogenId, PhysicalEventType physicalEventType);
  static ModifierFlag getModifierFlag(KeyCode keycode);
};
}
}
