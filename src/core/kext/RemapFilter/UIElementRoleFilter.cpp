#include <IOKit/IOLib.h>

#include "UIElementRoleFilter.hpp"
#include "CommonData.hpp"
#include "IOLogWrapper.hpp"
#include "bridge.h"

namespace org_pqrs_Karabiner {
  namespace RemapFilter {
    void
    UIElementRoleFilter::initialize(const unsigned int* vec, size_t length)
    {
      for (size_t i = 0; i < length; ++i) {
        targets_.push_back(AddValue(vec[i]));
      }
    }

    bool
    UIElementRoleFilter::isblocked(void)
    {
      unsigned int current = CommonData::getcurrent_workspacedata().uielementrole;

      switch (type_) {
        case BRIDGE_FILTERTYPE_UIELEMENTROLE_NOT:
        case BRIDGE_FILTERTYPE_UIELEMENTROLE_ONLY:
        {
          bool isnot = (type_ == BRIDGE_FILTERTYPE_UIELEMENTROLE_NOT);

          for (size_t i = 0; i < targets_.size(); ++i) {
            if (targets_[i] == current) {
              return isnot ? true : false;
            }
          }
          return isnot ? false : true;
        }

        default:
          IOLOG_ERROR("UIElementRoleFilter::isblocked unknown type_(%d)\n", type_);
          break;
      }

      return false;
    }
  }
}
