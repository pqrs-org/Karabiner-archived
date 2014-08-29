#include <IOKit/IOLib.h>

#include "ApplicationFilter.hpp"
#include "CommonData.hpp"
#include "IOLogWrapper.hpp"
#include "bridge.h"

namespace org_pqrs_Karabiner {
  namespace RemapFilter {
    void
    ApplicationFilter::initialize(const unsigned int* vec, size_t length)
    {
      for (size_t i = 0; i < length; ++i) {
        targets_.push_back(AddValue(vec[i]));
      }
    }

    bool
    ApplicationFilter::isblocked(void)
    {
      unsigned int current = CommonData::getcurrent_workspacedata().applicationtype;

      switch (get_type()) {
        case BRIDGE_FILTERTYPE_APPLICATION_NOT:
        case BRIDGE_FILTERTYPE_APPLICATION_ONLY:
        {
          bool isnot = (get_type() == BRIDGE_FILTERTYPE_APPLICATION_NOT);

          for (size_t i = 0; i < targets_.size(); ++i) {
            if (targets_[i] == current) {
              return isnot ? true : false;
            }
          }
          return isnot ? false : true;
        }

        default:
          IOLOG_ERROR("ApplicationFilter::isblocked unknown type_(%d)\n", get_type());
          break;
      }

      return false;
    }
  }
}
