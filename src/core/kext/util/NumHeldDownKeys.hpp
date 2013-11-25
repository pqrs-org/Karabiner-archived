#ifndef NUMHELDDOWNKEYS_HPP
#define NUMHELDDOWNKEYS_HPP

namespace org_pqrs_KeyRemap4MacBook {
  class NumHeldDownKeys {
  public:
    static void set(int diff);
    static void reset(void);
    static bool iszero(void);

  private:
    static void log(void);

    static int num_;
  };
}

#endif
