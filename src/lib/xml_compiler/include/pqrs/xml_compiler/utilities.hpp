#ifndef PQRS_XML_COMPILER_UTILITIES_HPP
#define PQRS_XML_COMPILER_UTILITIES_HPP

#include "bridge.h"

namespace pqrs {
  class xml_compiler_utilities {
  public:
    static uint32_t get_datatype(const std::string& s) {
      if (boost::starts_with(s, "KeyCode::"))          { return BRIDGE_DATATYPE_KEYCODE;          }
      if (boost::starts_with(s, "ModifierFlag::"))     { return BRIDGE_DATATYPE_FLAGS;            }
      if (boost::starts_with(s, "ConsumerKeyCode::"))  { return BRIDGE_DATATYPE_CONSUMERKEYCODE;  }
      if (boost::starts_with(s, "PointingButton::"))   { return BRIDGE_DATATYPE_POINTINGBUTTON;   }
      if (boost::starts_with(s, "ScrollWheel::"))      { return BRIDGE_DATATYPE_SCROLLWHEEL;      }
      if (boost::starts_with(s, "PointingRelative::")) { return BRIDGE_DATATYPE_POINTINGRELATIVE; }
      if (boost::starts_with(s, "KeyboardType::"))     { return BRIDGE_DATATYPE_KEYBOARDTYPE;     }
      if (boost::starts_with(s, "DeviceVendor::"))     { return BRIDGE_DATATYPE_DEVICEVENDOR;     }
      if (boost::starts_with(s, "DeviceProduct::"))    { return BRIDGE_DATATYPE_DEVICEPRODUCT;    }
      if (boost::starts_with(s, "DeviceLocation::"))   { return BRIDGE_DATATYPE_DEVICELOCATION;   }
      if (boost::starts_with(s, "Option::"))           { return BRIDGE_DATATYPE_OPTION;           }
      if (boost::starts_with(s, "Millisecond::"))      { return BRIDGE_DATATYPE_MILLISECOND;      }
      if (boost::starts_with(s, "DelayUntilRepeat::")) { return BRIDGE_DATATYPE_DELAYUNTILREPEAT; }
      if (boost::starts_with(s, "KeyRepeat::"))        { return BRIDGE_DATATYPE_KEYREPEAT;        }
      return BRIDGE_DATATYPE_NONE;
    }
  };
}

#endif
