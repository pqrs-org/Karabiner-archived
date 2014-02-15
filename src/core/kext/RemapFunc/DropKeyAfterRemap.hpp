#ifndef DROPKEYAFTERREMAP_HPP
#define DROPKEYAFTERREMAP_HPP

#include "FromEvent.hpp"
#include "RemapFuncClasses.hpp"

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
      FromEvent fromEvent_;
      Flags fromFlags_;
    };
  }
};

#endif
