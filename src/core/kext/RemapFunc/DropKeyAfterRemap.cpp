#include "DropKeyAfterRemap.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace RemapFunc {
    DropKeyAfterRemap::DropKeyAfterRemap(void)
    {}

    DropKeyAfterRemap::~DropKeyAfterRemap(void)
    {}

    void
    DropKeyAfterRemap::add(unsigned int datatype, unsigned int newval)
    {
      switch (datatype) {
        case BRIDGE_DATATYPE_KEYCODE:
        {
          fromKey_.key = newval;
          break;
        }

        case BRIDGE_DATATYPE_FLAGS:
        {
          fromKey_.flags = newval;
          break;
        }

        default:
          IOLOG_ERROR("DropKeyAfterRemap::add invalid datatype:%d\n", datatype);
          break;
      }
    }

    bool
    DropKeyAfterRemap::drop(const Params_KeyboardEventCallBack& params)
    {
      if (fromkeychecker_.isFromKey(params.ex_iskeydown, params.key, params.flags, fromKey_.key, fromKey_.flags)) return true;
      return false;
    }
  }
}
