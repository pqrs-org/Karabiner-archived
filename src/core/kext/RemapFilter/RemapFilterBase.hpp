#ifndef REMAPFILTERBASE_HPP
#define REMAPFILTERBASE_HPP

#include "bridge.h"
#include "IOLogWrapper.hpp"
#include "KeyCode.hpp"
#include "Vector.hpp"

namespace org_pqrs_Karabiner {
namespace RemapFilter {
class RemapFilterBase {
public:
  RemapFilterBase(unsigned int type) : type_(type) {}
  virtual ~RemapFilterBase(void) {}

  virtual bool isblocked(void) = 0;

  unsigned int get_type(void) const { return type_; }

private:
  unsigned int type_;
};

typedef RemapFilterBase* RemapFilterBasePointer;
DECLARE_VECTOR(RemapFilterBasePointer);
}
}

#endif
