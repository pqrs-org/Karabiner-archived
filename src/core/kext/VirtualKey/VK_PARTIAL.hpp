#ifndef VIRTUALKEY_VK_PARTIAL_HPP
#define VIRTUALKEY_VK_PARTIAL_HPP

#include "KeyCode.hpp"
#include "List.hpp"

namespace org_pqrs_Karabiner {
namespace VirtualKey {
class VK_PARTIAL final {
public:
  static void initialize(void);
  static void terminate(void);
  static bool handle(const Params_KeyboardEventCallBack& params, AutogenId autogenId, PhysicalEventType physicalEventType);

private:
  class Item final : public List::Item {
  public:
    Item(KeyCode k) : key(k) {}
    virtual ~Item(void) {}

    KeyCode key;
  };

  static List changedKeyCodes_;
  static bool active_;
  static EventType eventType_;
};
}
}

#endif
