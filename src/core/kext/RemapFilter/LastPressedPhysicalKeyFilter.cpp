#include <IOKit/IOLib.h>

#include "CommonData.hpp"
#include "IOLogWrapper.hpp"
#include "LastPressedPhysicalKeyFilter.hpp"
#include "bridge.h"

namespace org_pqrs_Karabiner {
  namespace RemapFilter {
    void
    LastPressedPhysicalKeyFilter::initialize(const unsigned int* vec, size_t length)
    {
      for (size_t i = 0; i < length - 1; i += 2) {
        targets_.push_back(FilterValueWithDataType(AddDataType(vec[i]), AddValue(vec[i + 1])));
      }

      if (length % 2 > 0) {
        IOLOG_WARN("Invalid length(%d) in BRIDGE_FILTERTYPE_LASTPRESSEDPHYSICALKEY_*\n", static_cast<int>(length));
      }
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
