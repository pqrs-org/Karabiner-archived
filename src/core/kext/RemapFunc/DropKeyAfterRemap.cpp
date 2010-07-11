#include "DropKeyAfterRemap.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace RemapFunc {
    void
    DropKeyAfterRemap::initialize(void)
    {}

    void
    DropKeyAfterRemap::terminate(void)
    {}

    void
    DropKeyAfterRemap::add(KeyCode newval)
    {
      fromKey_.key = newval;
    }

    void
    DropKeyAfterRemap::add(Flags newval)
    {
      fromKey_.flags = newval;
    }

    bool
    DropKeyAfterRemap::drop(const Params_KeyboardEventCallBack& params)
    {
      if (fromkeychecker_.isFromKey(params.ex_iskeydown, params.key, params.flags, fromKey_.key, fromKey_.flags)) return true;
      return false;
    }
  }
}
