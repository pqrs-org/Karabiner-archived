#include <IOKit/IOLib.h>

#include "CommonData.hpp"
#include "ElapsedTimeSinceLastPressedFilter.hpp"
#include "IOLogWrapper.hpp"
#include "bridge.h"

namespace org_pqrs_Karabiner {
  namespace RemapFilter {
    void
    ElapsedTimeSinceLastPressedFilter::initialize(const unsigned int* vec, size_t length)
    {
      for (size_t i = 0; i < length - 1; i += 2) {
        targets_.push_back(FilterValueWithDataType(AddDataType(vec[i]), AddValue(vec[i + 1])));
      }

      if (length % 2 > 0) {
        IOLOG_WARN("Invalid length(%d) in BRIDGE_FILTERTYPE_ELAPSEDTIMESINCELASTPRESSED_*\n", static_cast<int>(length));
      }
    }

    bool
    ElapsedTimeSinceLastPressedFilter::isblocked(void)
    {
      const LastPressedPhysicalKey& current = CommonData::getcurrent_lastpressedphysicalkey();
      if (current.get_datatype() == BRIDGE_DATATYPE_NONE) return false;

      switch (get_type()) {
        case BRIDGE_FILTERTYPE_ELAPSEDTIMESINCELASTPRESSED_GREATERTHAN:
        case BRIDGE_FILTERTYPE_ELAPSEDTIMESINCELASTPRESSED_LESSTHAN:
        {
          uint32_t current_ms = current.get_milliseconds();

          for (size_t i = 0; i < targets_.size(); ++i) {
            uint32_t filter_ms = 0;
            if (targets_[i].datatype == BRIDGE_DATATYPE_MILLISECOND) {
              filter_ms = targets_[i].value;
            } else {
              IOLOG_ERROR("ElapsedTimeSinceLastPressedFilter::isblocked unknown datatype(%d)\n", targets_[i].datatype);
              continue;
            }

            switch (get_type()) {
              case BRIDGE_FILTERTYPE_ELAPSEDTIMESINCELASTPRESSED_GREATERTHAN:
                if (filter_ms > current_ms) return true;
                break;
              case BRIDGE_FILTERTYPE_ELAPSEDTIMESINCELASTPRESSED_LESSTHAN:
                if (filter_ms < current_ms) return true;
                break;
            }
          }
          return false;
        }

        default:
          IOLOG_ERROR("ElapsedTimeSinceLastPressedFilter::isblocked unknown type_(%d)\n", get_type());
          break;
      }

      return false;
    }
  }
}
