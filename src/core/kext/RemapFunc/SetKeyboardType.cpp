#include "SetKeyboardType.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace RemapFunc {
    SetKeyboardType::SetKeyboardType(void) : toKeyboardType_(KeyboardType::MACBOOK)
    {}

    SetKeyboardType::~SetKeyboardType(void)
    {}

    void
    SetKeyboardType::add(unsigned int datatype, unsigned int newval)
    {
      switch (datatype) {
        case BRIDGE_DATATYPE_KEYBOARDTYPE:
        {
          toKeyboardType_ = newval;
          break;
        }

        default:
          IOLOG_ERROR("SetKeyboardType::add invalid datatype:%d\n", datatype);
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
