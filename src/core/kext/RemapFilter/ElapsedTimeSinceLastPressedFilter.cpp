#include "base.hpp"
#include "bridge.h"
#include "CommonData.hpp"
#include "ElapsedTimeSinceLastPressedFilter.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace RemapFilter {
    ElapsedTimeSinceLastPressedFilter::ElapsedTimeSinceLastPressedFilter(unsigned int t) : type_(t)
    {}

    ElapsedTimeSinceLastPressedFilter::~ElapsedTimeSinceLastPressedFilter(void)
    {}

    void
    ElapsedTimeSinceLastPressedFilter::add(unsigned int newval)
    {
      targets_.push_back(newval);
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
          for (size_t i = 0; i < targets_.size(); ++i) {
            switch (type_) {
              case BRIDGE_FILTERTYPE_ELAPSEDTIMESINCELASTPRESSED_GREATERTHAN:
                if (targets_[i] > milliseconds) return true;
                break;
              case BRIDGE_FILTERTYPE_ELAPSEDTIMESINCELASTPRESSED_LESSTHAN:
                if (targets_[i] < milliseconds) return true;
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
