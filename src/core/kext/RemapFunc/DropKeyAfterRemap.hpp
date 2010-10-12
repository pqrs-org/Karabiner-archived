#ifndef DROPKEYAFTERREMAP_HPP
#define DROPKEYAFTERREMAP_HPP

#include "RemapFuncBase.hpp"
#include "FromKeyChecker.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace RemapFunc {
    class DropKeyAfterRemap {
    public:
      DropKeyAfterRemap(void);
      ~DropKeyAfterRemap(void);

      bool drop(const Params_KeyboardEventCallBack& params);

      // ----------------------------------------
      // [0] => fromKey_
      void add(unsigned int datatype, unsigned int newval);

    private:
      FromKeyChecker fromkeychecker_;
      PairKeyFlags fromKey_;
    };
  }
};

#endif
