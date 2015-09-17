#include "diagnostic_macros.hpp"

BEGIN_IOKIT_INCLUDE;
#include <IOKit/IOLib.h>
END_IOKIT_INCLUDE;

#include "EventOutputQueue.hpp"
#include "FlagStatus.hpp"
#include "VK_JIS_TOGGLE_EISUU_KANA.hpp"

namespace org_pqrs_Karabiner {
KeyCode VirtualKey::VK_JIS_TOGGLE_EISUU_KANA::newkeycode_;

bool VirtualKey::VK_JIS_TOGGLE_EISUU_KANA::handle(const Params_KeyboardEventCallBack& params, AutogenId autogenId, PhysicalEventType physicalEventType) {
  if (params.key != KeyCode::VK_JIS_TOGGLE_EISUU_KANA) return false;

  if (params.ex_iskeydown) {
    if (CommonData::getcurrent_workspaceInputSourceIds().is_include(WorkspaceInputSourceId(InputSource::JAPANESE.get()))) {
      newkeycode_ = KeyCode::JIS_EISUU;
    } else {
      newkeycode_ = KeyCode::JIS_KANA;
    }
  }

  Params_KeyboardEventCallBack p(params.eventType, params.flags, newkeycode_,
                                 params.keyboardType, params.repeat);
  EventOutputQueue::FireKey::fire(p, autogenId, physicalEventType);

  return true;
}
}
