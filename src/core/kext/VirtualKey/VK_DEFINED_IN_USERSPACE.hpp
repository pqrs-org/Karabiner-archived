#ifndef VIRTUALKEY_VK_DEFINED_IN_USERSPACE_HPP
#define VIRTUALKEY_VK_DEFINED_IN_USERSPACE_HPP

#include "RemapClass.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace VirtualKey {
    class VK_CHANGE_INPUTSOURCE {
    public:
      static void initialize(void);
      static void terminate(void);

      static void add_item(RemapClass* remapclass, unsigned int keycode);
      static void clear_items(void);

      static bool handle(const Params_KeyboardEventCallBack& params);
      static bool handleAfterEnqueued(const Params_KeyboardEventCallBack& params);

    private:
      struct Item {
        Item(void) :
          remapclass(NULL),
          keycode(0)
        {};
        Item(RemapClass* p, unsigned int kc) : remapclass(p), keycode(kc) {}
        RemapClass* remapclass;
        unsigned int keycode;
      };
      DECLARE_VECTOR(Item);
      static Vector_Item* items_;
    };
  }
}

#endif
