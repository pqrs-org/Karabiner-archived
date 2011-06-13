#ifndef VIRTUALKEY_HPP
#define VIRTUALKEY_HPP

#include "CallbackWrapper.hpp"
#include "KeyCode.hpp"
#include "RemapClass.hpp"
#include "TimerWrapper.hpp"
#include "Vector.hpp"
#include "VirtualKey/VK_CHANGE_INPUTMODE.hpp"
#include "VirtualKey/VK_CONFIG.hpp"
#include "VirtualKey/VK_LAZY.hpp"
#include "VirtualKey/VK_LOCK.hpp"
#include "VirtualKey/VK_MOUSEKEY.hpp"
#include "VirtualKey/VK_STICKY.hpp"
#include "VirtualKey/VK_JIS_BACKSLASH.hpp"
#include "VirtualKey/VK_JIS_TEMPORARY.hpp"
#include "VirtualKey/VK_JIS_TOGGLE_EISUU_KANA.hpp"
#include "VirtualKey/VK_JIS_YEN.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace VirtualKey {
    void initialize(IOWorkLoop& workloop);
    void terminate(void);
    void reset(void);

    bool isKeyLikeModifier(KeyCode keycode);
  };
}

#endif
