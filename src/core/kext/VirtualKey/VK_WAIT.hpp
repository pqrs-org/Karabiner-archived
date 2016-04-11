#pragma once

namespace org_pqrs_Karabiner {
namespace VirtualKey {
class VK_WAIT final {
public:
  static bool handle(const Params_KeyboardEventCallBack& params, AutogenId autogenId, PhysicalEventType physicalEventType);
};
}
}
