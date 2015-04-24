#include <IOKit/IOLib.h>

#include "EventOutputQueue.hpp"
#include "FlagStatus.hpp"
#include "VK_JIS_YEN.hpp"

namespace org_pqrs_Karabiner {
bool VirtualKey::VK_JIS_YEN::handle(const Params_KeyboardEventCallBack& params, AutogenId autogenId) {
  if (params.key != KeyCode::VK_JIS_YEN) return false;

  Params_KeyboardEventCallBack p(params.eventType, params.flags, KeyCode::JIS_YEN,
                                 KeyboardType::JIS_MACBOOK_2008, params.repeat);
  EventOutputQueue::FireKey::fire(p, autogenId);

  return true;
}
}
