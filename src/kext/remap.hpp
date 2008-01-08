#ifndef REMAP_HPP
#define REMAP_HPP

#include "base.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  struct RemapParams {
    unsigned int *eventType;
    unsigned int *flags;
    unsigned int *key;
    unsigned int *charCode;
    unsigned int *charSet;
    unsigned int *origCharCode;
    unsigned int *origCharSet;
    unsigned int *keyboardType;
    bool *repeat;
    AbsoluteTime *ts;
    bool *ex_dropKey;
    unsigned int ex_origKey;
  };
  void remap_core(const RemapParams &params);
}

#endif
