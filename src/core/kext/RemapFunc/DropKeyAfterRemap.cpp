#include <IOKit/IOLib.h>

#include "DropKeyAfterRemap.hpp"
#include "IOLogWrapper.hpp"

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
          fromEvent_ = FromEvent(datatype, newval);
          break;
        }

        case BRIDGE_DATATYPE_FLAGS:
        {
          fromFlags_ = Flags(newval);
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
      ParamsUnion paramsUnion(params);
      if (! fromEvent_.changePressingState(paramsUnion, params.flags, fromFlags_)) return false;
      return true;
    }
  }
}
