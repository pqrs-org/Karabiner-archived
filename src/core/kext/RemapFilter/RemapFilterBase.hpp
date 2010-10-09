#ifndef REMAPFILTERBASE_HPP
#define REMAPFILTERBASE_HPP

#include "Vector.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace RemapFilter {
    struct ConfigPointer {
      ConfigPointer(void) : pointer(NULL) {}
      ConfigPointer(unsigned int* p) : pointer(p) {}

      unsigned int* pointer;
    };
    // XXX: For ConfigFilter. Remove me.
    DECLARE_VECTOR(ConfigPointer);
  }
}

#endif
