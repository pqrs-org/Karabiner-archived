#include <IOKit/IOLib.h>

#include "IOLogWrapper.hpp"
#include "SetKeyboardType.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace RemapFunc {
    SetKeyboardType::SetKeyboardType(void) : toKeyboardType_(KeyboardType::MACBOOK)
    {}

    SetKeyboardType::~SetKeyboardType(void)
    {}

    void
    SetKeyboardType::add(AddDataType datatype, AddValue newval)
    {
      switch (datatype) {
        case BRIDGE_DATATYPE_KEYBOARDTYPE:
        {
          toKeyboardType_ = KeyboardType(newval);
          break;
        }

        default:
          IOLOG_ERROR("SetKeyboardType::add invalid datatype:%u\n", static_cast<unsigned int>(datatype));
          break;
      }
    }

    bool
    SetKeyboardType::remap(KeyboardType& keyboardType)
    {
      keyboardType = toKeyboardType_;
      return false;
    }
  }
}
