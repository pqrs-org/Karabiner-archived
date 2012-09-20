#include "base.hpp"
#include "EventOutputQueue.hpp"
#include "FlagStatus.hpp"
#include "VK_JIS_TOGGLE_EISUU_KANA.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  KeyCode VirtualKey::VK_JIS_TOGGLE_EISUU_KANA::newkeycode_;

  bool
  VirtualKey::VK_JIS_TOGGLE_EISUU_KANA::handle(const Params_KeyboardEventCallBack& params)
  {
    if (params.key != KeyCode::VK_JIS_TOGGLE_EISUU_KANA) return false;

    if (params.ex_iskeydown) {
      if (InputSource::JAPANESE == CommonData::getcurrent_workspacedata().inputsource) {
        newkeycode_ = KeyCode::JIS_EISUU;
      } else {
        newkeycode_ = KeyCode::JIS_KANA;
      }
    }

    Params_KeyboardEventCallBack::auto_ptr ptr(Params_KeyboardEventCallBack::alloc(params.eventType, params.flags, newkeycode_,
                                                                                   params.keyboardType, params.repeat));
    if (ptr) {
      EventOutputQueue::FireKey::fire(*ptr);
    }
    return true;
  }
}
