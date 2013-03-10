#ifndef VIRTUALKEY_VK_PARTIAL_HPP
#define VIRTUALKEY_VK_PARTIAL_HPP

#include "KeyCode.hpp"
#include "List.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace VirtualKey {
    class VK_PARTIAL {
    public:
      static void initialize(void);
      static void terminate(void);
      static bool handle(const Params_KeyboardEventCallBack& params);

    private:
      class Item : public List::Item {
      public:
        Item(KeyCode k) : key(k) {}
        virtual ~Item(void) {}

        KeyCode key;
      };

      static List* changedKeyCodes_;
      static bool active_;
      static EventType eventType_;
    };
  }
}

#endif
