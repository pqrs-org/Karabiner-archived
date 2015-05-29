#ifndef VIRTUALKEY_VK_CONSUMERKEY_HPP
#define VIRTUALKEY_VK_CONSUMERKEY_HPP

namespace org_pqrs_Karabiner {
namespace VirtualKey {
class VK_CONSUMERKEY final {
public:
  static bool handle(const Params_KeyboardEventCallBack& params, AutogenId autogenId, PhysicalEventType physicalEventType);
};
}
}

#endif
