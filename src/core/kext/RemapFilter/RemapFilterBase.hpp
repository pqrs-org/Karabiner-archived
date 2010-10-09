#ifndef REMAPFILTERBASE_HPP
#define REMAPFILTERBASE_HPP

#include "Vector.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace RemapFilter {
    struct ConfigPointer {
      ConfigPointer(void) : pointer(NULL) {}
      ConfigPointer(int* p) : pointer(p) {}

      int* pointer;
    };
    // XXX: For ConfigFilter. Remove me.
    DECLARE_VECTOR(ConfigPointer);
  }
}

#endif
