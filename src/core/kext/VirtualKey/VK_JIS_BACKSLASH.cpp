#include <IOKit/IOLib.h>

#include "EventOutputQueue.hpp"
#include "FlagStatus.hpp"
#include "VK_JIS_BACKSLASH.hpp"

namespace org_pqrs_Karabiner {
bool VirtualKey::VK_JIS_BACKSLASH::handle(const Params_KeyboardEventCallBack& params, AutogenId autogenId, PhysicalEventType physicalEventType) {
  if (params.key != KeyCode::VK_JIS_BACKSLASH) return false;

  Params_KeyboardEventCallBack p(params.eventType, params.flags, KeyCode::BACKSLASH,
                                 KeyboardType::MACBOOK, params.repeat);
  EventOutputQueue::FireKey::fire(p, autogenId, physicalEventType);

  return true;
}
}
