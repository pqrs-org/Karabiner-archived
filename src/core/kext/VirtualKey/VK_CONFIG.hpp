#ifndef VIRTUALKEY_VK_CONFIG_HPP
#define VIRTUALKEY_VK_CONFIG_HPP

#include "RemapClass.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace VirtualKey {
    class VK_CONFIG {
    public:
      static void initialize(void);
      static void terminate(void);

      static void add_item(RemapClass* remapclass,
                           unsigned int keycode_toggle,
                           unsigned int keycode_force_on,
                           unsigned int keycode_force_off,
                           unsigned int keycode_sync_keydownup);
      static void clear_items(void);

      static bool handle(const Params_KeyboardEventCallBack& params);

      static bool is_VK_CONFIG_SYNC_KEYDOWNUP(KeyCode keycode);

    private:
      struct Item {
        Item(void) :
          remapclass(NULL),
          keycode_toggle(0),
          keycode_force_on(0),
          keycode_force_off(0),
          keycode_sync_keydownup(0)
        {};
        Item(RemapClass* p, unsigned int kc_toggle, unsigned int kc_force_on, unsigned int kc_force_off, unsigned int kc_sync) :
          remapclass(p), keycode_toggle(kc_toggle), keycode_force_on(kc_force_on), keycode_force_off(kc_force_off), keycode_sync_keydownup(kc_sync) {}
        RemapClass* remapclass;
        unsigned int keycode_toggle;
        unsigned int keycode_force_on;
        unsigned int keycode_force_off;
        unsigned int keycode_sync_keydownup;
      };
      DECLARE_VECTOR(Item);
      static Vector_Item* items_;
    };
  }
}

#endif
