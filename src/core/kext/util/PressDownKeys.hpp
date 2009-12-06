#ifndef PRESSDOWNKEYS_HPP
#define PRESSDOWNKEYS_HPP

#include "base.hpp"
#include "KeyCode.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  // store RemapUtil::fireKey results
  class PressDownKeys {
  public:
    static void initialize(void);
    static void add(const KeyCode& key, const KeyboardType& keyboardType);
    static void remove(const KeyCode& key, const KeyboardType& keyboardType);
    static void clear(OSObject* target, AbsoluteTime ts, OSObject* sender, void* refcon);

  private:
    enum {
      MAXNUM = 16,
    };
    struct Item {
      bool enable;
      KeyCode key;
      KeyboardType keyboardType;
    };
    static Item item_[MAXNUM];
  };
}

#endif
