#ifndef VIRTUALKEY_VK_IOHIKEYBOARD_TOGGLE_NUMLOCK_HPP
#define VIRTUALKEY_VK_IOHIKEYBOARD_TOGGLE_NUMLOCK_HPP

namespace org_pqrs_Karabiner {
namespace VirtualKey {
class VK_IOHIKEYBOARD_TOGGLE_NUMLOCK final {
public:
  static bool handle(const Params_KeyboardEventCallBack& params, AutogenId autogenId, PhysicalEventType physicalEventType);
};
}
}

#endif
