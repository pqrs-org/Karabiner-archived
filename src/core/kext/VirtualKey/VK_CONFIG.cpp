#include "base.hpp"
#include "EventOutputQueue.hpp"
#include "FlagStatus.hpp"
#include "UserClient_kext.hpp"
#include "VK_CONFIG.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  VirtualKey::VK_CONFIG::Vector_Item* VirtualKey::VK_CONFIG::items_ = NULL;

  void
  VirtualKey::VK_CONFIG::initialize(void)
  {
    items_ = new Vector_Item();
  }

  void
  VirtualKey::VK_CONFIG::terminate(void)
  {
    if (items_) {
      delete items_;
    }
  }

  void
  VirtualKey::VK_CONFIG::add_item(RemapClass* remapclass,
                                  unsigned int keycode_toggle,
                                  unsigned int keycode_force_on,
                                  unsigned int keycode_force_off,
                                  unsigned int keycode_sync_keydownup)
  {
    if (! items_) return;

    items_->push_back(Item(remapclass, keycode_toggle, keycode_force_on, keycode_force_off, keycode_sync_keydownup));
  }

  void
  VirtualKey::VK_CONFIG::clear_items(void)
  {
    items_->clear();
  }

  bool
  VirtualKey::VK_CONFIG::handle(const Params_KeyboardEventCallBack& params)
  {
    if (! items_) return false;

    RemapClass* remapclass = NULL;

    for (size_t i = 0; i < items_->size(); ++i) {
      remapclass                          = (*items_)[i].remapclass;
      unsigned int keycode_toggle         = (*items_)[i].keycode_toggle;
      unsigned int keycode_force_on       = (*items_)[i].keycode_force_on;
      unsigned int keycode_force_off      = (*items_)[i].keycode_force_off;
      unsigned int keycode_sync_keydownup = (*items_)[i].keycode_sync_keydownup;

      if (! remapclass) return false;

      if (params.ex_iskeydown && params.repeat == false) {
        /*  */ if (params.key == keycode_toggle) {
          remapclass->toggleEnabled();
          goto refresh;

        } else if (params.key == keycode_force_on) {
          remapclass->setEnabled(true);
          goto refresh;

        } else if (params.key == keycode_force_off) {
          remapclass->setEnabled(false);
          goto refresh;

        } else if (params.key == keycode_sync_keydownup) {
          remapclass->setEnabled(true);
          goto refresh;
        }

      } else if (params.eventType == EventType::UP) {
        if (params.key == keycode_toggle ||
            params.key == keycode_force_on ||
            params.key == keycode_force_off) {
          goto finish;
        }

        if (params.key == keycode_sync_keydownup) {
          remapclass->setEnabled(false);
          goto refresh;
        }
      }
    }

    return false;

  refresh:
    RemapClassManager::refresh();

    // Tell remapclass status is changed to userspace.
    if (remapclass) {
      org_pqrs_driver_KeyRemap4MacBook_UserClient_kext::send_notification_to_userspace(BRIDGE_USERCLIENT_NOTIFICATION_TYPE_CONFIG_ENABLED_UPDATED, remapclass->get_configindex());
    }

  finish:
    EventOutputQueue::FireModifiers::fire();
    return true;
  }

  bool
  VirtualKey::VK_CONFIG::is_VK_CONFIG_SYNC_KEYDOWNUP(KeyCode keycode)
  {
    if (! items_) return false;

    for (size_t i = 0; i < items_->size(); ++i) {
      unsigned int keycode_sync_keydownup = (*items_)[i].keycode_sync_keydownup;
      if (keycode == keycode_sync_keydownup) return true;
    }

    return false;
  }
}
