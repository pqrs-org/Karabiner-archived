#ifndef VK_KEYTOKEY_DELAYED_ACTION_DROP_EVENT_HPP
#define VK_KEYTOKEY_DELAYED_ACTION_DROP_EVENT_HPP

namespace org_pqrs_Karabiner {
namespace VirtualKey {
class VK_KEYTOKEY_DELAYED_ACTION_DROP_EVENT final {
public:
  static void reset(void);

  static bool handle(const Params_KeyboardEventCallBack& params, AutogenId autogenId, PhysicalEventType physicalEventType);

  static bool needToDrop(void) { return needToDrop_; }
  static void setNeedToDrop(bool v) { needToDrop_ = v; }

private:
  static bool needToDrop_;
};
}
}

#endif
