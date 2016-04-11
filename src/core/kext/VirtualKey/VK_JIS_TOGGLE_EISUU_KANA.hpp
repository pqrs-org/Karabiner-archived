#pragma once

namespace org_pqrs_Karabiner {
namespace VirtualKey {
class VK_JIS_TOGGLE_EISUU_KANA final {
public:
  static bool handle(const Params_KeyboardEventCallBack& params, AutogenId autogenId, PhysicalEventType physicalEventType);

private:
  // It is necessary to save toKeyCode for KeyUp.
  static KeyCode newkeycode_;
};
}
}
