#include <IOKit/IOLib.h>

#include "CommonData.hpp"
#include "IOLogWrapper.hpp"
#include "LastPressedPhysicalKeyFilter.hpp"
#include "bridge.h"

namespace org_pqrs_KeyRemap4MacBook {
  namespace RemapFilter {
    LastPressedPhysicalKeyFilter::LastPressedPhysicalKeyFilter(unsigned int t) : type_(t)
    {}

    LastPressedPhysicalKeyFilter::~LastPressedPhysicalKeyFilter(void)
    {}

    void
    LastPressedPhysicalKeyFilter::add(AddDataType datatype, AddValue value)
    {
      targets_.push_back(FilterValueWithDataType(datatype, value));
    }

    bool
    LastPressedPhysicalKeyFilter::isblocked(void)
    {
      const LastPressedPhysicalKey& current = CommonData::getcurrent_lastpressedphysicalkey();
      if (current.get_datatype() == BRIDGE_DATATYPE_NONE) return false;

      switch (type_) {
        case BRIDGE_FILTERTYPE_LASTPRESSEDPHYSICALKEY_NOT:
        case BRIDGE_FILTERTYPE_LASTPRESSEDPHYSICALKEY_ONLY:
        {
          bool isnot = (type_ == BRIDGE_FILTERTYPE_LASTPRESSEDPHYSICALKEY_NOT);

          for (size_t i = 0; i < targets_.size(); ++i) {
            if (targets_[i].datatype == current.get_datatype() &&
                targets_[i].value == current.get_value()) {
              return isnot ? true : false;
            }
          }
          return isnot ? false : true;
        }

        default:
          IOLOG_ERROR("LastPressedPhysicalKeyFilter::isblocked unknown type_(%d)\n", type_);
          break;
      }

      return false;
    }
  }
}
