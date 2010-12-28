#include "base.hpp"
#include "Client.hpp"
#include "CommonData.hpp"
#include "Config.hpp"
#include "EventOutputQueue.hpp"
#include "FlagStatus.hpp"
#include "RemapClass.hpp"
#include "VirtualKey.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  void
  VirtualKey::initialize(IOWorkLoop& workloop)
  {
    Handle_VK_MOUSEKEY::initialize(workloop);
    Handle_VK_JIS_TEMPORARY::initialize(workloop);

    Handle_VK_CONFIG::initialize();
  }

  void
  VirtualKey::terminate(void)
  {
    Handle_VK_MOUSEKEY::terminate();
    Handle_VK_JIS_TEMPORARY::terminate();

    Handle_VK_CONFIG::terminate();
  }

  void
  VirtualKey::reset(void)
  {
    Handle_VK_MOUSEKEY::reset();
  }

  // ----------------------------------------------------------------------
  bool
  Handle_VK_LOCK::handle(const Params_KeyboardEventCallBack& params)
  {
    Flags flags(0);
    bool force_on = false;
    bool force_off = false;

    if (params.key == KeyCode::VK_LOCK_FN) {
      flags.add(ModifierFlag::FN);
    } else if (params.key == KeyCode::VK_LOCK_FN_FORCE_OFF) {
      flags.add(ModifierFlag::FN);
      force_off = true;

    } else if (params.key == KeyCode::VK_LOCK_COMMAND_L) {
      flags.add(ModifierFlag::COMMAND_L);
    } else if (params.key == KeyCode::VK_LOCK_COMMAND_L_FORCE_ON) {
      flags.add(ModifierFlag::COMMAND_L);
      force_on = true;
    } else if (params.key == KeyCode::VK_LOCK_COMMAND_L_FORCE_OFF) {
      flags.add(ModifierFlag::COMMAND_L);
      force_off = true;

    } else if (params.key == KeyCode::VK_LOCK_COMMAND_R) {
      flags.add(ModifierFlag::COMMAND_R);

    } else if (params.key == KeyCode::VK_LOCK_SHIFT_L) {
      flags.add(ModifierFlag::SHIFT_L);
    } else if (params.key == KeyCode::VK_LOCK_SHIFT_L_FORCE_ON) {
      flags.add(ModifierFlag::SHIFT_L);
      force_on = true;
    } else if (params.key == KeyCode::VK_LOCK_SHIFT_L_FORCE_OFF) {
      flags.add(ModifierFlag::SHIFT_L);
      force_off = true;

    } else if (params.key == KeyCode::VK_LOCK_ALL_FORCE_OFF) {
      FlagStatus::lock_clear();
      return true;
    } else {
      return false;
    }

    if (params.ex_iskeydown && params.repeat == false) {
      if (force_off) {
        FlagStatus::lock_decrease(flags);
      } else if (force_on) {
        FlagStatus::lock_increase(flags);
      } else {
        FlagStatus::lock_toggle(flags);
      }
      EventOutputQueue::FireModifiers::fire();
    }

    return true;
  }

  // ----------------------------------------------------------------------
  bool
  Handle_VK_STICKY::handle(const Params_KeyboardEventCallBack& params)
  {
    ModifierFlag flag = ModifierFlag::NONE;

    /*  */ if (params.key == KeyCode::VK_STICKY_COMMAND_L) {
      flag = ModifierFlag::COMMAND_L;
    } else if (params.key == KeyCode::VK_STICKY_COMMAND_R) {
      flag = ModifierFlag::COMMAND_R;
    } else if (params.key == KeyCode::VK_STICKY_CONTROL_L) {
      flag = ModifierFlag::CONTROL_L;
    } else if (params.key == KeyCode::VK_STICKY_CONTROL_R) {
      flag = ModifierFlag::CONTROL_R;
    } else if (params.key == KeyCode::VK_STICKY_FN) {
      flag = ModifierFlag::FN;
    } else if (params.key == KeyCode::VK_STICKY_OPTION_L) {
      flag = ModifierFlag::OPTION_L;
    } else if (params.key == KeyCode::VK_STICKY_OPTION_R) {
      flag = ModifierFlag::OPTION_R;
    } else if (params.key == KeyCode::VK_STICKY_SHIFT_L) {
      flag = ModifierFlag::SHIFT_L;
    } else if (params.key == KeyCode::VK_STICKY_SHIFT_R) {
      flag = ModifierFlag::SHIFT_R;
    } else if (params.key == KeyCode::VK_STICKY_EXTRA1) {
      flag = ModifierFlag::EXTRA1;
    } else if (params.key == KeyCode::VK_STICKY_EXTRA2) {
      flag = ModifierFlag::EXTRA2;
    } else if (params.key == KeyCode::VK_STICKY_EXTRA3) {
      flag = ModifierFlag::EXTRA3;
    } else if (params.key == KeyCode::VK_STICKY_EXTRA4) {
      flag = ModifierFlag::EXTRA4;
    } else if (params.key == KeyCode::VK_STICKY_EXTRA5) {
      flag = ModifierFlag::EXTRA5;
    } else {
      return false;
    }

    if (params.ex_iskeydown && params.repeat == false) {
      FlagStatus::sticky_toggle(flag);
      EventOutputQueue::FireModifiers::fire();
    }

    return true;
  }

  // ----------------------------------------------------------------------
  bool
  Handle_VK_LAZY::handle(const Params_KeyboardEventCallBack& params)
  {
    ModifierFlag f = ModifierFlag::NONE;

    /**/ if (params.key == KeyCode::VK_LAZY_COMMAND_L) { f = ModifierFlag::COMMAND_L; }
    else if (params.key == KeyCode::VK_LAZY_COMMAND_R) { f = ModifierFlag::COMMAND_R; }
    else if (params.key == KeyCode::VK_LAZY_CONTROL_L) { f = ModifierFlag::CONTROL_L; }
    else if (params.key == KeyCode::VK_LAZY_CONTROL_R) { f = ModifierFlag::CONTROL_R; }
    else if (params.key == KeyCode::VK_LAZY_FN)        { f = ModifierFlag::FN;        }
    else if (params.key == KeyCode::VK_LAZY_OPTION_L)  { f = ModifierFlag::OPTION_L;  }
    else if (params.key == KeyCode::VK_LAZY_OPTION_R)  { f = ModifierFlag::OPTION_R;  }
    else if (params.key == KeyCode::VK_LAZY_SHIFT_L)   { f = ModifierFlag::SHIFT_L;   }
    else if (params.key == KeyCode::VK_LAZY_SHIFT_R)   { f = ModifierFlag::SHIFT_R;   }

    if (f == ModifierFlag::NONE) return false;

    // ----------------------------------------
    if (params.repeat) return true;

    if (params.eventType == EventType::DOWN) {
      FlagStatus::increase(f);
    } else if (params.eventType == EventType::UP) {
      FlagStatus::decrease(f);
    } else {
      IOLOG_ERROR("Handle_VK_LAZY invalid EventType.\n");
    }

    return true;
  }

  // ----------------------------------------------------------------------
  bool
  Handle_VK_CHANGE_INPUTMODE::handle(const Params_KeyboardEventCallBack& params)
  {
    KeyRemap4MacBook_bridge::ChangeInputMode::Request request;
    if (params.key == KeyCode::VK_CHANGE_INPUTMODE_ENGLISH ||
        params.key == KeyCode::VK_CHANGE_INPUTMODE_FRENCH ||
        params.key == KeyCode::VK_CHANGE_INPUTMODE_GERMAN ||
        params.key == KeyCode::VK_CHANGE_INPUTMODE_JAPANESE ||
        params.key == KeyCode::VK_CHANGE_INPUTMODE_SWEDISH ||
        params.key == KeyCode::VK_CHANGE_INPUTMODE_CANADIAN ||
        params.key == KeyCode::VK_CHANGE_INPUTMODE_RUSSIAN ||
        params.key == KeyCode::VK_CHANGE_INPUTMODE_RUSSIAN_TYPOGRAPHIC ||
        params.key == KeyCode::VK_CHANGE_INPUTMODE_ENGLISH_TYPOGRAPHIC ||
        params.key == KeyCode::VK_CHANGE_INPUTMODE_TRADITIONAL_CHINESE_YAHOO_KEYKEY ||
        params.key == KeyCode::VK_CHANGE_INPUTMODE_ESTONIAN) {
      request.vk_keycode = params.key.get();
    } else {
      return false;
    }

    if (params.ex_iskeydown && params.repeat == false) {
      KeyRemap4MacBook_client::sendmsg(KeyRemap4MacBook_bridge::REQUEST_CHANGE_INPUTMODE, &request, sizeof(request), NULL, 0);
    }
    return true;
  }

  // ----------------------------------------------------------------------
  Handle_VK_CONFIG::Vector_Item* Handle_VK_CONFIG::items_ = NULL;
  IOLock* Handle_VK_CONFIG::lock_ = NULL;

  void
  Handle_VK_CONFIG::initialize(void)
  {
    items_ = new Vector_Item();
    lock_ = IOLockWrapper::alloc();
  }

  void
  Handle_VK_CONFIG::terminate(void)
  {
    if (items_) {
      delete items_;
    }
    if (lock_) {
      IOLockWrapper::free(lock_);
    }
  }

  void
  Handle_VK_CONFIG::add_item(RemapClass* remapclass,
                             unsigned int keycode_toggle,
                             unsigned int keycode_force_on,
                             unsigned int keycode_force_off,
                             unsigned int keycode_sync_keydownup)
  {
    IOLockWrapper::ScopedLock lk(lock_);

    if (! items_) return;

    items_->push_back(Item(remapclass, keycode_toggle, keycode_force_on, keycode_force_off, keycode_sync_keydownup));
  }

  void
  Handle_VK_CONFIG::clear_items(void)
  {
    IOLockWrapper::ScopedLock lk(lock_);

    items_->clear();
  }

  bool
  Handle_VK_CONFIG::handle(const Params_KeyboardEventCallBack& params)
  {
    IOLockWrapper::ScopedLock lk(lock_);

    if (! items_) return false;

    for (size_t i = 0; i < items_->size(); ++i) {
      RemapClass* remapclass              = (*items_)[i].remapclass;
      unsigned int keycode_toggle         = (*items_)[i].keycode_toggle;
      unsigned int keycode_force_on       = (*items_)[i].keycode_force_on;
      unsigned int keycode_force_off      = (*items_)[i].keycode_force_off;
      unsigned int keycode_sync_keydownup = (*items_)[i].keycode_sync_keydownup;

      if (! remapclass) return false;

      if (params.ex_iskeydown && params.repeat == false) {
        /*  */ if (params.key == keycode_toggle) {
          remapclass->toggleEnabled();
          goto finish;

        } else if (params.key == keycode_force_on) {
          remapclass->setEnabled(true);
          goto finish;

        } else if (params.key == keycode_force_off) {
          remapclass->setEnabled(false);
          goto finish;

        } else if (params.key == keycode_sync_keydownup) {
          remapclass->setEnabled(true);
          goto finish;
        }

      } else if (params.eventType == EventType::UP) {
        if (params.key == keycode_toggle ||
            params.key == keycode_force_on ||
            params.key == keycode_force_off) {
          return true;
        }

        if (params.key == keycode_sync_keydownup) {
          remapclass->setEnabled(false);
          goto finish;
        }
      }
    }

    return false;

  finish:
    RemapClassManager::refresh();
    return true;
  }

  // ----------------------------------------------------------------------
  void
  Handle_VK_MOUSEKEY::initialize(IOWorkLoop& workloop)
  {
    dx_ = 0;
    dy_ = 0;
    scale_ = 1;
    scrollmode_ = false;
    highspeed_ = false;

    timer_.initialize(&workloop, NULL, Handle_VK_MOUSEKEY::fire);
  }

  void
  Handle_VK_MOUSEKEY::terminate(void)
  {
    timer_.terminate();
  }

  void
  Handle_VK_MOUSEKEY::reset(void)
  {
    dx_ = 0;
    dy_ = 0;
    scale_ = 1;
    scrollmode_ = false;
    highspeed_ = false;

    timer_.cancelTimeout();
  }

  bool
  Handle_VK_MOUSEKEY::handle(const Params_KeyboardEventCallBack& params)
  {
    /*  */ if (params.key == KeyCode::VK_MOUSEKEY_UP) {
      if (params.repeat == false) {
        if (params.ex_iskeydown) { --dy_; scrollmode_ = false; } else { ++dy_; }
      }
    } else if (params.key == KeyCode::VK_MOUSEKEY_DOWN) {
      if (params.repeat == false) {
        if (params.ex_iskeydown) { ++dy_; scrollmode_ = false; } else { --dy_; }
      }
    } else if (params.key == KeyCode::VK_MOUSEKEY_LEFT) {
      if (params.repeat == false) {
        if (params.ex_iskeydown) { --dx_; scrollmode_ = false; } else { ++dx_; }
      }
    } else if (params.key == KeyCode::VK_MOUSEKEY_RIGHT) {
      if (params.repeat == false) {
        if (params.ex_iskeydown) { ++dx_; scrollmode_ = false; } else { --dx_; }
      }

    } else if (params.key == KeyCode::VK_MOUSEKEY_SCROLL_UP) {
      if (params.repeat == false) {
        if (params.ex_iskeydown) { --dy_; scrollmode_ = true; } else { ++dy_; }
      }
    } else if (params.key == KeyCode::VK_MOUSEKEY_SCROLL_DOWN) {
      if (params.repeat == false) {
        if (params.ex_iskeydown) { ++dy_; scrollmode_ = true; } else { --dy_; }
      }
    } else if (params.key == KeyCode::VK_MOUSEKEY_SCROLL_LEFT) {
      if (params.repeat == false) {
        if (params.ex_iskeydown) { --dx_; scrollmode_ = true; } else { ++dx_; }
      }
    } else if (params.key == KeyCode::VK_MOUSEKEY_SCROLL_RIGHT) {
      if (params.repeat == false) {
        if (params.ex_iskeydown) { ++dx_; scrollmode_ = true; } else { --dx_; }
      }

    } else if (params.key == KeyCode::VK_MOUSEKEY_HIGHSPEED) {
      if (params.repeat == false) {
        highspeed_ = params.ex_iskeydown;
      }

    } else {
      return false;
    }

    if (dx_ != 0 || dy_ != 0) {
      timer_.setTimeoutMS(TIMER_INTERVAL, false);
    } else {
      scale_ = 1;

      // keep scrollmode_ & highspeed_.
      //
      // When VK_MOUSEKEY_SCROLL_UP and VK_MOUSEKEY_SCROLL_DOWN are pressed at the same time,
      // this code will be executed.
      //
      // In the above case, we need to keep scrollmode_, highspeed_ value.

      timer_.cancelTimeout();
    }

    return true;
  }

  void
  Handle_VK_MOUSEKEY::fire(OSObject* notuse_owner, IOTimerEventSource* notuse_sender)
  {
    IOLockWrapper::ScopedLock lk(timer_.getlock());

    if (! scrollmode_) {
      int s = scale_;
      if (highspeed_) s = HIGHSPEED_RELATIVE_SCALE;

      EventOutputQueue::FireRelativePointer::fire(ButtonStatus::makeButtons(), dx_ * s, dy_ * s);

    } else {
      int s = scale_;
      if (highspeed_) s = HIGHSPEED_SCROLL_SCALE;

      int delta1 = -dy_ * s * EventOutputQueue::FireScrollWheel::DELTA_SCALE;
      int delta2 = -dx_ * s * EventOutputQueue::FireScrollWheel::DELTA_SCALE;
      EventOutputQueue::FireScrollWheel::fire(delta1, delta2);
    }

    if (scale_ < SCALE_MAX) {
      ++scale_;
    }

    timer_.setTimeoutMS(TIMER_INTERVAL);
  }

  int Handle_VK_MOUSEKEY::dx_;
  int Handle_VK_MOUSEKEY::dy_;
  int Handle_VK_MOUSEKEY::scale_;
  bool Handle_VK_MOUSEKEY::scrollmode_;
  bool Handle_VK_MOUSEKEY::highspeed_;
  TimerWrapper Handle_VK_MOUSEKEY::timer_;

  // ----------------------------------------------------------------------
  bool
  Handle_VK_JIS_TOGGLE_EISUU_KANA::handle(const Params_KeyboardEventCallBack& params)
  {
    if (params.key != KeyCode::VK_JIS_TOGGLE_EISUU_KANA) return false;

    if (params.ex_iskeydown) {
      if (InputMode::JAPANESE == CommonData::getcurrent_workspacedata().inputmode) {
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

  KeyCode Handle_VK_JIS_TOGGLE_EISUU_KANA::newkeycode_;

  // ----------------------------------------------------------------------
  bool
  handle_VK_JIS_BACKSLASH(const Params_KeyboardEventCallBack& params)
  {
    if (params.key != KeyCode::VK_JIS_BACKSLASH) return false;

    Params_KeyboardEventCallBack::auto_ptr ptr(Params_KeyboardEventCallBack::alloc(params.eventType, params.flags, KeyCode::BACKSLASH,
                                                                                   KeyboardType::MACBOOK, params.repeat));
    if (ptr) {
      EventOutputQueue::FireKey::fire(*ptr);
    }
    return true;
  }

  bool
  handle_VK_JIS_YEN(const Params_KeyboardEventCallBack& params)
  {
    if (params.key != KeyCode::VK_JIS_YEN) return false;

    Params_KeyboardEventCallBack::auto_ptr ptr(Params_KeyboardEventCallBack::alloc(params.eventType, params.flags, KeyCode::JIS_YEN,
                                                                                   KeyboardType::JIS_MACBOOK_2008, params.repeat));
    if (ptr) {
      EventOutputQueue::FireKey::fire(*ptr);
    }
    return true;
  }

  // ----------------------------------------------------------------------
  void
  Handle_VK_JIS_TEMPORARY::initialize(IOWorkLoop& workloop)
  {
    timer_.initialize(&workloop, NULL, Handle_VK_JIS_TEMPORARY::fire);
    fireKeyInfo_.active = false;
  }

  void
  Handle_VK_JIS_TEMPORARY::terminate(void)
  {
    timer_.terminate();
  }

  bool
  Handle_VK_JIS_TEMPORARY::handle_core(const Params_KeyboardEventCallBack& params,
                                       KeyCode key,
                                       InputModeDetail inputmodedetail)
  {
    if (params.key != key) return false;

    if (params.ex_iskeydown) {
      if (savedinputmodedetail_ == InputModeDetail::NONE) {
        savedinputmodedetail_ = CommonData::getcurrent_workspacedata().inputmodedetail;
        currentinputmodedetail_ = CommonData::getcurrent_workspacedata().inputmodedetail;
      }
      firekeytoinputdetail(params, inputmodedetail);
    }

    return true;
  }

  bool
  Handle_VK_JIS_TEMPORARY::handle_RESTORE(const Params_KeyboardEventCallBack& params)
  {
    if (params.key != KeyCode::VK_JIS_TEMPORARY_RESTORE) return false;

    if (params.ex_iskeydown) {
      if (savedinputmodedetail_ != InputModeDetail::NONE) {
        firekeytoinputdetail(params, savedinputmodedetail_);
        savedinputmodedetail_ = InputModeDetail::NONE;
        currentinputmodedetail_ = InputModeDetail::NONE;
      }
    }

    return true;
  }

  bool
  Handle_VK_JIS_TEMPORARY::handle(const Params_KeyboardEventCallBack& params)
  {
    // ------------------------------------------------------------
    if (handle_core(params,
                    KeyCode::VK_JIS_TEMPORARY_ROMAN,
                    InputModeDetail::ROMAN)) return true;

    if (handle_core(params,
                    KeyCode::VK_JIS_TEMPORARY_HIRAGANA,
                    InputModeDetail::JAPANESE_HIRAGANA)) return true;

    if (handle_core(params,
                    KeyCode::VK_JIS_TEMPORARY_KATAKANA,
                    InputModeDetail::JAPANESE_KATAKANA)) return true;

    // OK, Ainu is not Japanese.
    // But the input source of Ainu is Kotoeri, we need to handle it here.
    if (handle_core(params,
                    KeyCode::VK_JIS_TEMPORARY_AINU,
                    InputModeDetail::AINU)) return true;

    // ------------------------------------------------------------
    if (handle_RESTORE(params)) return true;

    // ------------------------------------------------------------
    // flash keyevent
    if (fireKeyInfo_.active) {
      IOLockWrapper::ScopedLock lk(timer_.getlock());
      timer_.cancelTimeout();
      fire_nolock();
    }

    return false;
  }

  void
  Handle_VK_JIS_TEMPORARY::firekeytoinputdetail(const Params_KeyboardEventCallBack& params,
                                                InputModeDetail inputmodedetail)
  {
    IOLockWrapper::ScopedLock lk(timer_.getlock());

    inputmodedetail = normalize(inputmodedetail);
    currentinputmodedetail_ = normalize(currentinputmodedetail_);

    // ------------------------------------------------------------
    if (inputmodedetail == currentinputmodedetail_) return;
    if (inputmodedetail == InputModeDetail::NONE) return;

    currentinputmodedetail_ = inputmodedetail;

    // ------------------------------------------------------------
    if (inputmodedetail == InputModeDetail::ROMAN) {
      fireKeyInfo_.flags = Flags(0);
      fireKeyInfo_.key = KeyCode::JIS_EISUU;

    } else if (inputmodedetail == InputModeDetail::JAPANESE_HIRAGANA) {
      fireKeyInfo_.flags = Flags(0);
      fireKeyInfo_.key = KeyCode::JIS_KANA;

    } else if (inputmodedetail == InputModeDetail::JAPANESE_KATAKANA) {
      fireKeyInfo_.flags = ModifierFlag::SHIFT_L;
      fireKeyInfo_.key = KeyCode::JIS_KANA;

    } else if (inputmodedetail == InputModeDetail::AINU) {
      fireKeyInfo_.flags = ModifierFlag::OPTION_L;
      fireKeyInfo_.key = KeyCode::JIS_KANA;

    } else {
      return;
    }

    fireKeyInfo_.keyboardType = params.keyboardType;
    fireKeyInfo_.active = true;

    timer_.setTimeoutMS(KEYEVENT_DELAY_MS);
  }

  InputModeDetail
  Handle_VK_JIS_TEMPORARY::normalize(InputModeDetail imd)
  {
    if (imd == InputModeDetail::JAPANESE) {
      return InputModeDetail::JAPANESE_HIRAGANA;
    }
    return imd;
  }

  void
  Handle_VK_JIS_TEMPORARY::fire(OSObject* notuse_owner, IOTimerEventSource* notuse_sender)
  {
    IOLockWrapper::ScopedLock lk(timer_.getlock());
    fire_nolock();
  }

  void
  Handle_VK_JIS_TEMPORARY::fire_nolock(void)
  {
    if (! fireKeyInfo_.active) return;
    fireKeyInfo_.active = false;
    EventOutputQueue::FireKey::fire_downup(fireKeyInfo_.flags, fireKeyInfo_.key, fireKeyInfo_.keyboardType);
  }

  InputModeDetail Handle_VK_JIS_TEMPORARY::savedinputmodedetail_(0);
  InputModeDetail Handle_VK_JIS_TEMPORARY::currentinputmodedetail_(0);
  Handle_VK_JIS_TEMPORARY::FireKeyInfo Handle_VK_JIS_TEMPORARY::fireKeyInfo_;
  TimerWrapper Handle_VK_JIS_TEMPORARY::timer_;
}
