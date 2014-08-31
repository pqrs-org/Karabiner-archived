#ifndef REMAPFUNCBASE_HPP
#define REMAPFUNCBASE_HPP

#include "bridge.h"
#include "FromEvent.hpp"
#include "IOLogWrapper.hpp"
#include "KeyCode.hpp"
#include "RemapFuncClasses.hpp"
#include "ToEvent.hpp"

namespace org_pqrs_Karabiner {
  namespace RemapFunc {
    class RemapFuncBase {
    public:
      RemapFuncBase(unsigned int type) : type_(type) {}
      virtual ~RemapFuncBase(void) {}

      virtual void add(AddDataType datatype, AddValue newval) = 0;

      virtual bool remap(RemapParams& remapParams) { return false; }

      unsigned int get_type(void) const { return type_; }

    private:
      unsigned int type_;
    };
  }
}

#endif
