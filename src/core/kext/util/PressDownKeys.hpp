#ifndef REMAPUTIL_PRESSDOWNKEYS_HPP
#define REMAPUTIL_PRESSDOWNKEYS_HPP

#include "base.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  // ----------------------------------------------------------------------
  // store RemapUtil::fireKey results
  class PressDownKeys {
  public:
    void initialize(void);
    void add(unsigned int _key, unsigned int _keyboardType);
    void remove(unsigned int _key, unsigned int _keyboardType);
    void clear(KeyboardEventCallback callback, OSObject *target, AbsoluteTime ts, OSObject *sender, void *refcon);

  private:
    enum {
      MAXNUM = 16,
    };
    struct Item {
      bool enable;
      unsigned int key;
      unsigned int keyboardType;
    };
    Item item[MAXNUM];
  };
}

#endif
