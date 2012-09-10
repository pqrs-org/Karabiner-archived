#include "base.hpp"
#include "EventOutputQueue.hpp"
#include "FlagStatus.hpp"
#include "UserClient_kext.hpp"
#include "VK_CHANGE_INPUTSOURCE.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  VirtualKey::VK_CHANGE_INPUTSOURCE::Vector_Item* VirtualKey::VK_CHANGE_INPUTSOURCE::items_ = NULL;

  void
  VirtualKey::VK_CHANGE_INPUTSOURCE::initialize(void)
  {
    items_ = new Vector_Item();
  }

  void
  VirtualKey::VK_CHANGE_INPUTSOURCE::terminate(void)
  {
    if (items_) {
      delete items_;
    }
  }

  void
  VirtualKey::VK_CHANGE_INPUTSOURCE::add_item(RemapClass* remapclass, unsigned int keycode)
  {
    if (! items_) return;

    items_->push_back(Item(remapclass, keycode));
  }

  void
  VirtualKey::VK_CHANGE_INPUTSOURCE::clear_items(void)
  {
    items_->clear();
  }

  bool
  VirtualKey::VK_CHANGE_INPUTSOURCE::handle(const Params_KeyboardEventCallBack& params)
  {
    // VK_CHANGE_INPUTSOURCE uses UserClient which is effective immediately.
    // Considering KeyCode::VK_WAIT, we should not send notification via UserClient at here.
    //
    // For example:
    // ----------------------------------------
    //   <autogen>
    //     --KeyToKey--
    //     KeyCode::A,
    //
    //     KeyCode::VK_CHANGE_INPUTSOURCE_FRENCH,
    //     <!-- Waiting Input Source switching. -->
    //     KeyCode::VK_WAIT_10MS,
    //     KeyCode::A,
    //
    //     KeyCode::VK_CHANGE_INPUTSOURCE_ENGLISH,
    //     <!-- Waiting Input Source switching. -->
    //     KeyCode::VK_WAIT_10MS,
    //     KeyCode::A,
    //   </autogen>
    // ----------------------------------------
    //
    // VK_WAIT_10MS modifies wait of EventOutputQueue.
    // It is not wait at VirtualKey::handle.
    //
    // Therefore, if we send notification at here,
    // the order of sending keys is follows.
    //
    // 1. KeyCode::VK_CHANGE_INPUTSOURCE_FRENCH
    // 2. KeyCode::VK_CHANGE_INPUTSOURCE_ENGLISH
    // 3. <wait 10ms>
    // 4. KeyCode::A
    // 5. <wait 10ms>
    // 6. KeyCode::A
    //
    // This order is not intended.
    // The intended order is follows.
    //
    // 1. KeyCode::VK_CHANGE_INPUTSOURCE_FRENCH
    // 2. <wait 10ms>
    // 3. KeyCode::A
    // 4. KeyCode::VK_CHANGE_INPUTSOURCE_ENGLISH
    // 5. <wait 10ms>
    // 6. KeyCode::A
    //
    // In order to achieve this behavior,
    // we need to send notification at handleAfterEnqueued.

    return false;
  }

  bool
  VirtualKey::VK_CHANGE_INPUTSOURCE::handleAfterEnqueued(const Params_KeyboardEventCallBack& params)
  {
    if (! items_) return false;

    for (size_t i = 0; i < items_->size(); ++i) {
      RemapClass* remapclass = (*items_)[i].remapclass;
      unsigned int keycode   = (*items_)[i].keycode;

      if (! remapclass) return false;

      if (params.key == keycode) {
        if (params.ex_iskeydown && params.repeat == false) {
          org_pqrs_driver_KeyRemap4MacBook_UserClient_kext::send_notification_to_userspace(BRIDGE_USERCLIENT_NOTIFICATION_TYPE_CHANGE_INPUT_SOURCE, params.key.get());
        }
        return true;
      }
    }

    return false;
  }
}
