#ifndef VIRTUALKEY_VK_JIS_TOGGLE_EISUU_KANA_HPP
#define VIRTUALKEY_VK_JIS_TOGGLE_EISUU_KANA_HPP

namespace org_pqrs_KeyRemap4MacBook {
  namespace VirtualKey {
    class VK_JIS_TOGGLE_EISUU_KANA {
    public:
      static bool handle(const Params_KeyboardEventCallBack& params);

    private:
      // It is necessary to save toKeyCode for KeyUp.
      static KeyCode newkeycode_;
    };
  }
}

#endif
