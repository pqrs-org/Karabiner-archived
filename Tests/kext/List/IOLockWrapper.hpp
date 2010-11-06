#ifndef IOLOCKWRAPPER_HPP
#define IOLOCKWRAPPER_HPP

#include <stdio.h>

typedef int IOLock;

class IOLockWrapper {
public:
  static IOLock* alloc(void) { return new IOLock; }
  static void free(IOLock*& lock) { delete lock; }

  class ScopedLock {
  public:
    ScopedLock(IOLock* p) {}
    bool operator!(void) const { return false; }
  };
};

#endif
