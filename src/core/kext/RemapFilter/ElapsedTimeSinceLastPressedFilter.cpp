#include <IOKit/IOLib.h>

#include "CommonData.hpp"
#include "ElapsedTimeSinceLastPressedFilter.hpp"
#include "IOLogWrapper.hpp"
#include "bridge.h"

namespace org_pqrs_KeyRemap4MacBook {
  namespace RemapFilter {
    ElapsedTimeSinceLastPressedFilter::ElapsedTimeSinceLastPressedFilter(unsigned int t) : type_(t)
    {}

    ElapsedTimeSinceLastPressedFilter::~ElapsedTimeSinceLastPressedFilter(void)
    {}

    void
    ElapsedTimeSinceLastPressedFilter::add(unsigned int datatype, unsigned int value)
    {
      targets_.push_back(FilterValueWithDataType(datatype, value));
    }

    bool
    ElapsedTimeSinceLastPressedFilter::isblocked(void)
    {
      const LastPressedPhysicalKey& current = CommonData::getcurrent_lastpressedphysicalkey();
      if (current.get_datatype() == BRIDGE_DATATYPE_NONE) return false;

      switch (type_) {
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

            switch (type_) {
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
          IOLOG_ERROR("ElapsedTimeSinceLastPressedFilter::isblocked unknown type_(%d)\n", type_);
          break;
      }

      return false;
    }
  }
}
