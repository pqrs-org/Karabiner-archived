#ifndef DROPKEYAFTERREMAP_HPP
#define DROPKEYAFTERREMAP_HPP

#include "RemapFuncBase.hpp"
#include "FromKeyChecker.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace RemapFunc {
    class DropKeyAfterRemap {
    public:
      DropKeyAfterRemap(void) {}
      void initialize(void);
      void terminate(void);
      bool drop(const Params_KeyboardEventCallBack& params);

      // ----------------------------------------
      // [0] => fromKey_
      void add(KeyCode newval);
      void add(Flags newval);

    private:
      size_t index_;
      FromKeyChecker fromkeychecker_;
      PairKeyFlags fromKey_;
    };
  }
};

#endif
