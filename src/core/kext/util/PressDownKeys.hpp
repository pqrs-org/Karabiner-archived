#ifndef PRESSDOWNKEYS_HPP
#define PRESSDOWNKEYS_HPP

#include "base.hpp"
#include "KeyCode.hpp"
#include "List.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  // store EventOutput::FireKey results
  class PressDownKeys {
  public:
    static void initialize(void);
    static void terminate(void);
    static void add(KeyCode key, KeyboardType keyboardType);
    static void remove(KeyCode key, KeyboardType keyboardType);
    static void clear(void);

  private:
    class Item : public List::Item {
    public:
      Item(KeyCode k, KeyboardType kt) : key(k), keyboardType(kt) {}
      virtual ~Item(void) {}

      KeyCode key;
      KeyboardType keyboardType;
    };
    static List* list_;
    static IOLock* lock_;
  };
}

#endif
