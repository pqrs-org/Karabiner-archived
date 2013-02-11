#ifndef VIRTUALKEY_VK_DEFINED_IN_USERSPACE_HPP
#define VIRTUALKEY_VK_DEFINED_IN_USERSPACE_HPP

#include "RemapClass.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace VirtualKey {
    // This class implements these keycodes.
    // - KeyCode::VK_CHANGE_INPUTSOURCE_*
    // - KeyCode::VK_OPEN_URL_*

    class VK_DEFINED_IN_USERSPACE {
    public:
      static void initialize(void);
      static void terminate(void);

      static void add_item(RemapClass* remapclass, unsigned int keycode, uint32_t notification_type);
      static void clear_items(void);

      static bool handle(const Params_KeyboardEventCallBack& params);
      static bool handleAfterEnqueued(const Params_KeyboardEventCallBack& params);

    private:
      struct Item {
        Item(void) :
          remapclass(NULL),
          keycode(0),
          notification_type(0)
        {};
        Item(RemapClass* p, unsigned int kc, uint32_t nt) : remapclass(p), keycode(kc), notification_type(nt) {}
        RemapClass* remapclass;
        unsigned int keycode;
        uint32_t notification_type;
      };
      DECLARE_VECTOR(Item);
      static Vector_Item* items_;
    };
  }
}

#endif
