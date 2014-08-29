#include <IOKit/IOLib.h>

#include "Config.hpp"
#include "ConfigFilter.hpp"
#include "IOLogWrapper.hpp"
#include "RemapClass.hpp"
#include "bridge.h"

namespace org_pqrs_Karabiner {
  namespace RemapFilter {
    void
    ConfigFilter::initialize(const unsigned int* vec, size_t length)
    {
      for (size_t i = 0; i < length; ++i) {
        targets_.push_back(AddValue(vec[i]));
      }
    }

    bool
    ConfigFilter::isblocked(void)
    {
      switch (get_type()) {
        case BRIDGE_FILTERTYPE_CONFIG_NOT:
        case BRIDGE_FILTERTYPE_CONFIG_ONLY:
        {
          bool isnot = (get_type() == BRIDGE_FILTERTYPE_CONFIG_NOT);

          for (size_t i = 0; i < targets_.size(); ++i) {
            if (RemapClassManager::isEnabled(targets_[i])) {
              return isnot ? true : false;
            }
          }
          return isnot ? false : true;
        }

        default:
          IOLOG_ERROR("ConfigFilter::isblocked unknown type_(%d)\n", get_type());
          break;
      }

      return false;
    }
  }
}
