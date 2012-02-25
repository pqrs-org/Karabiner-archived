#include "base.hpp"
#include "EventOutputQueue.hpp"
#include "FlagStatus.hpp"
#include "UserClient_kext.hpp"
#include "VK_CHANGE_INPUTMODE.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  bool
  VirtualKey::VK_CHANGE_INPUTMODE::handle(const Params_KeyboardEventCallBack& params)
  {
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
        params.key == KeyCode::VK_CHANGE_INPUTMODE_ESTONIAN ||
        params.key == KeyCode::VK_CHANGE_INPUTMODE_FINNISH ||
        params.key == KeyCode::VK_CHANGE_INPUTMODE_THAI ||
        params.key == KeyCode::VK_CHANGE_INPUTSOURCE_DVORAK ||
        params.key == KeyCode::VK_CHANGE_INPUTSOURCE_DVORAK_DEVANAGARI_PAUL ||
        params.key == KeyCode::VK_CHANGE_INPUTSOURCE_COLEMAK) {

      if (params.ex_iskeydown && params.repeat == false) {
        org_pqrs_driver_KeyRemap4MacBook_UserClient_kext::send_notification_to_userspace(BRIDGE_USERCLIENT_NOTIFICATION_TYPE_CHANGE_INPUT_SOURCE, params.key.get());
      }
      return true;

    } else {
      return false;
    }
  }
}
