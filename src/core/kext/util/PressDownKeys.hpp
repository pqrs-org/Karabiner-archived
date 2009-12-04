#ifndef PRESSDOWNKEYS_HPP
#define PRESSDOWNKEYS_HPP

#include "base.hpp"
#include "keycode.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  // store RemapUtil::fireKey results
  class PressDownKeys {
  public:
    static void initialize(void);
    static void add(unsigned int key, const KeyboardType& keyboardType);
    static void remove(unsigned int key, const KeyboardType& keyboardType);
    static void clear(OSObject* target, AbsoluteTime ts, OSObject* sender, void* refcon);

  private:
    enum {
      MAXNUM = 16,
    };
    struct Item {
      bool enable;
      unsigned int key;
      KeyboardType keyboardType;
    };
    static Item item_[MAXNUM];
  };
}

#endif
