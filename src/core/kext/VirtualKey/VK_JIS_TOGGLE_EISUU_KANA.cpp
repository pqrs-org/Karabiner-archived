#include <IOKit/IOLib.h>

#include "EventOutputQueue.hpp"
#include "FlagStatus.hpp"
#include "VK_JIS_TOGGLE_EISUU_KANA.hpp"

namespace org_pqrs_Karabiner {
KeyCode VirtualKey::VK_JIS_TOGGLE_EISUU_KANA::newkeycode_;

bool VirtualKey::VK_JIS_TOGGLE_EISUU_KANA::handle(const Params_KeyboardEventCallBack& params, AutogenId autogenId) {
  if (params.key != KeyCode::VK_JIS_TOGGLE_EISUU_KANA) return false;

  if (params.ex_iskeydown) {
    if (InputSource::JAPANESE == InputSource(CommonData::getcurrent_workspacedata().inputsource)) {
      newkeycode_ = KeyCode::JIS_EISUU;
    } else {
      newkeycode_ = KeyCode::JIS_KANA;
    }
  }

  Params_KeyboardEventCallBack p(params.eventType, params.flags, newkeycode_,
                                 params.keyboardType, params.repeat);
  EventOutputQueue::FireKey::fire(p, autogenId);

  return true;
}
}
