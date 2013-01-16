#include <IOKit/hidsystem/ev_keymap.h>
#include "base.hpp"
#include "UserClient_kext.hpp"
#include "VK_IOHIDPOSTEVENT.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  bool
  VirtualKey::VK_IOHIDPOSTEVENT::handle(const Params_KeyboardEventCallBack& params)
  {
    // We need to send events at handleAfterEnqueued.
    // See VK_CHANGE_INPUTSOURCE::handle.

    return false;
  }

  bool
  VirtualKey::VK_IOHIDPOSTEVENT::handleAfterEnqueued(const Params_KeyboardEventCallBack& params)
  {
    bool target = false;
    uint32_t keytype = 0;

#define SET_KEYTYPE(KEYTYPE)                                  \
  if (params.key == KeyCode::VK_IOHIDPOSTEVENT_ ## KEYTYPE) { \
    target = true;                                            \
    keytype = KEYTYPE;                                        \
  }

    SET_KEYTYPE(NX_KEYTYPE_SOUND_UP);
    SET_KEYTYPE(NX_KEYTYPE_SOUND_DOWN);
    SET_KEYTYPE(NX_KEYTYPE_BRIGHTNESS_UP);
    SET_KEYTYPE(NX_KEYTYPE_BRIGHTNESS_DOWN);
    SET_KEYTYPE(NX_KEYTYPE_MUTE);
    SET_KEYTYPE(NX_KEYTYPE_PLAY);
    SET_KEYTYPE(NX_KEYTYPE_NEXT);
    SET_KEYTYPE(NX_KEYTYPE_PREVIOUS);
    SET_KEYTYPE(NX_KEYTYPE_ILLUMINATION_UP);
    SET_KEYTYPE(NX_KEYTYPE_ILLUMINATION_DOWN);
    SET_KEYTYPE(NX_KEYTYPE_ILLUMINATION_TOGGLE);

    if (! target) {
      return false;
    }

    if (params.ex_iskeydown && params.repeat == false) {
      org_pqrs_driver_KeyRemap4MacBook_UserClient_kext::send_notification_to_userspace(BRIDGE_USERCLIENT_NOTIFICATION_TYPE_IOHIDPOSTEVENT, keytype);
    }
    return true;
  }

  void
  VirtualKey::VK_IOHIDPOSTEVENT::post(ConsumerKeyCode keycode)
  {
    uint32_t keytype = 0;
    /**/ if (keycode == ConsumerKeyCode::BRIGHTNESS_DOWN)    { keytype = NX_KEYTYPE_BRIGHTNESS_DOWN;     }
    else if (keycode == ConsumerKeyCode::BRIGHTNESS_UP)      { keytype = NX_KEYTYPE_BRIGHTNESS_UP;       }
    else if (keycode == ConsumerKeyCode::KEYBOARDLIGHT_OFF)  { keytype = NX_KEYTYPE_ILLUMINATION_TOGGLE; }
    else if (keycode == ConsumerKeyCode::KEYBOARDLIGHT_LOW)  { keytype = NX_KEYTYPE_ILLUMINATION_DOWN;   }
    else if (keycode == ConsumerKeyCode::KEYBOARDLIGHT_HIGH) { keytype = NX_KEYTYPE_ILLUMINATION_UP;     }
    else if (keycode == ConsumerKeyCode::MUSIC_PREV)         { keytype = NX_KEYTYPE_PREVIOUS;            }
    else if (keycode == ConsumerKeyCode::MUSIC_PLAY)         { keytype = NX_KEYTYPE_PLAY;                }
    else if (keycode == ConsumerKeyCode::MUSIC_NEXT)         { keytype = NX_KEYTYPE_NEXT;                }
    else if (keycode == ConsumerKeyCode::VOLUME_MUTE)        { keytype = NX_KEYTYPE_MUTE;                }
    else if (keycode == ConsumerKeyCode::VOLUME_DOWN)        { keytype = NX_KEYTYPE_SOUND_DOWN;          }
    else if (keycode == ConsumerKeyCode::VOLUME_UP)          { keytype = NX_KEYTYPE_SOUND_UP;            }
    else {
      return;
    }

    org_pqrs_driver_KeyRemap4MacBook_UserClient_kext::send_notification_to_userspace(BRIDGE_USERCLIENT_NOTIFICATION_TYPE_IOHIDPOSTEVENT, keytype);
  }
}
