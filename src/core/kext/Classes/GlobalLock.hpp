#ifndef GLOBALLOCK_HPP
#define GLOBALLOCK_HPP

#include <IOKit/IOLib.h>

namespace org_pqrs_KeyRemap4MacBook {
  class GlobalLock {
  public:
    static void initialize(void);
    static void terminate(void);

    class ScopedLock {
    public:
      ScopedLock(void);
      ~ScopedLock(void);
      bool operator!(void) const;

    private:
      IOLock* lock_;
    };

    class ScopedUnlock {
    public:
      ScopedUnlock(void);
      ~ScopedUnlock(void);
      bool operator!(void) const;

    private:
      IOLock* lock_;
    };

  private:
    static IOLock* lock_;
  };
}

#endif
