#include "diagnostic_macros.hpp"

BEGIN_IOKIT_INCLUDE;
#include <IOKit/IOLib.h>
END_IOKIT_INCLUDE;

#include "IOLogWrapper.hpp"
#include "SetKeyboardType.hpp"

namespace org_pqrs_Karabiner {
namespace RemapFunc {
void SetKeyboardType::add(AddDataType datatype, AddValue newval) {
  switch (datatype) {
  case BRIDGE_DATATYPE_KEYBOARDTYPE: {
    toKeyboardType_ = KeyboardType(newval);
    break;
  }

  default:
    IOLOG_ERROR("SetKeyboardType::add invalid datatype:%u\n", static_cast<unsigned int>(datatype));
    break;
  }
}

bool SetKeyboardType::remapSetKeyboardType(KeyboardType& keyboardType) {
  keyboardType = toKeyboardType_;
  return true;
}
}
}
