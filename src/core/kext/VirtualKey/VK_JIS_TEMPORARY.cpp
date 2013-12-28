#include <IOKit/IOLib.h>

#include "EventOutputQueue.hpp"
#include "FlagStatus.hpp"
#include "VK_JIS_TEMPORARY.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  InputSourceDetail VirtualKey::VK_JIS_TEMPORARY::savedinputsourcedetail_(0);
  InputSourceDetail VirtualKey::VK_JIS_TEMPORARY::currentinputsourcedetail_(0);
  VirtualKey::VK_JIS_TEMPORARY::FireKeyInfo VirtualKey::VK_JIS_TEMPORARY::fireKeyInfo_;
  TimerWrapper VirtualKey::VK_JIS_TEMPORARY::fire_timer_;

  void
  VirtualKey::VK_JIS_TEMPORARY::initialize(IOWorkLoop& workloop)
  {
    fire_timer_.initialize(&workloop, NULL, VirtualKey::VK_JIS_TEMPORARY::fire_timer_callback);
    fireKeyInfo_.active = false;
  }

  void
  VirtualKey::VK_JIS_TEMPORARY::terminate(void)
  {
    fire_timer_.terminate();
  }

  bool
  VirtualKey::VK_JIS_TEMPORARY::handle_core(const Params_KeyboardEventCallBack& params,
                                            KeyCode key,
                                            InputSourceDetail inputsourcedetail)
  {
    if (params.key != key) return false;

    if (params.ex_iskeydown) {
      if (savedinputsourcedetail_ == InputSourceDetail::NONE) {
        savedinputsourcedetail_ = CommonData::getcurrent_workspacedata().inputsourcedetail;
        currentinputsourcedetail_ = CommonData::getcurrent_workspacedata().inputsourcedetail;
      }
      firekeytoinputdetail(params, inputsourcedetail);
    }

    return true;
  }

  bool
  VirtualKey::VK_JIS_TEMPORARY::handle_RESTORE(const Params_KeyboardEventCallBack& params)
  {
    if (params.key != KeyCode::VK_JIS_TEMPORARY_RESTORE) return false;

    if (params.ex_iskeydown) {
      if (savedinputsourcedetail_ != InputSourceDetail::NONE) {
        firekeytoinputdetail(params, savedinputsourcedetail_);
        savedinputsourcedetail_ = InputSourceDetail::NONE;
        currentinputsourcedetail_ = InputSourceDetail::NONE;
      }
    }

    return true;
  }

  bool
  VirtualKey::VK_JIS_TEMPORARY::handle(const Params_KeyboardEventCallBack& params)
  {
    // ------------------------------------------------------------
    if (handle_core(params,
                    KeyCode::VK_JIS_TEMPORARY_ROMAN,
                    InputSourceDetail::ENGLISH)) return true;

    if (handle_core(params,
                    KeyCode::VK_JIS_TEMPORARY_HIRAGANA,
                    InputSourceDetail::JAPANESE_HIRAGANA)) return true;

    if (handle_core(params,
                    KeyCode::VK_JIS_TEMPORARY_KATAKANA,
                    InputSourceDetail::JAPANESE_KATAKANA)) return true;

    // OK, Ainu is not Japanese.
    // But the input source of Ainu is Kotoeri, we need to handle it here.
    if (handle_core(params,
                    KeyCode::VK_JIS_TEMPORARY_AINU,
                    InputSourceDetail::AINU)) return true;

    // ------------------------------------------------------------
    if (handle_RESTORE(params)) return true;

    // ------------------------------------------------------------
    // flash keyevent
    if (fireKeyInfo_.active) {
      fire_timer_.cancelTimeout();
      fire();
    }

    return false;
  }

  void
  VirtualKey::VK_JIS_TEMPORARY::firekeytoinputdetail(const Params_KeyboardEventCallBack& params,
                                                     InputSourceDetail inputsourcedetail)
  {
    inputsourcedetail = normalize(inputsourcedetail);
    currentinputsourcedetail_ = normalize(currentinputsourcedetail_);

    // ------------------------------------------------------------
    if (inputsourcedetail == currentinputsourcedetail_) return;
    if (inputsourcedetail == InputSourceDetail::NONE) return;

    currentinputsourcedetail_ = inputsourcedetail;

    // ------------------------------------------------------------
    if (inputsourcedetail == InputSourceDetail::ENGLISH) {
      fireKeyInfo_.flags = Flags(0);
      fireKeyInfo_.key = KeyCode::JIS_EISUU;

    } else if (inputsourcedetail == InputSourceDetail::JAPANESE_HIRAGANA) {
      fireKeyInfo_.flags = Flags(0);
      fireKeyInfo_.key = KeyCode::JIS_KANA;

    } else if (inputsourcedetail == InputSourceDetail::JAPANESE_KATAKANA) {
      fireKeyInfo_.flags = Flags(ModifierFlag::SHIFT_L);
      fireKeyInfo_.key = KeyCode::JIS_KANA;

    } else if (inputsourcedetail == InputSourceDetail::AINU) {
      fireKeyInfo_.flags = Flags(ModifierFlag::OPTION_L);
      fireKeyInfo_.key = KeyCode::JIS_KANA;

    } else {
      return;
    }

    fireKeyInfo_.keyboardType = params.keyboardType;
    fireKeyInfo_.active = true;

    fire_timer_.setTimeoutMS(KEYEVENT_DELAY_MS);
  }

  InputSourceDetail
  VirtualKey::VK_JIS_TEMPORARY::normalize(InputSourceDetail imd)
  {
    if (imd == InputSourceDetail::JAPANESE) {
      return InputSourceDetail::JAPANESE_HIRAGANA;
    }
    return imd;
  }

  void
  VirtualKey::VK_JIS_TEMPORARY::fire_timer_callback(OSObject* notuse_owner, IOTimerEventSource* notuse_sender)
  {
    fire();
  }

  void
  VirtualKey::VK_JIS_TEMPORARY::fire(void)
  {
    if (! fireKeyInfo_.active) return;
    fireKeyInfo_.active = false;
    EventOutputQueue::FireKey::fire_downup(fireKeyInfo_.flags, fireKeyInfo_.key, fireKeyInfo_.keyboardType);
  }
}
