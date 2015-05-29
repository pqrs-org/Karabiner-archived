#ifndef VIRTUALKEY_VK_LOCK_HPP
#define VIRTUALKEY_VK_LOCK_HPP

namespace org_pqrs_Karabiner {
namespace VirtualKey {
class VK_LOCK final {
public:
  static bool handle(const Params_KeyboardEventCallBack& params, AutogenId autogenId, PhysicalEventType physicalEventType);
};
}
}

#endif
