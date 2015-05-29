#ifndef VIRTUALKEY_VK_STICKY_HPP
#define VIRTUALKEY_VK_STICKY_HPP

namespace org_pqrs_Karabiner {
namespace VirtualKey {
class VK_STICKY final {
public:
  static bool handle(const Params_KeyboardEventCallBack& params, AutogenId autogenId, PhysicalEventType physicalEventType);
};
}
}

#endif
