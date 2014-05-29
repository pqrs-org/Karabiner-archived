#include <IOKit/IOLib.h>

#include "UIElementRoleFilter.hpp"
#include "CommonData.hpp"
#include "IOLogWrapper.hpp"
#include "bridge.h"

namespace org_pqrs_KeyRemap4MacBook {
  namespace RemapFilter {
    UIElementRoleFilter::UIElementRoleFilter(unsigned int t) : type_(t)
    {}

    UIElementRoleFilter::~UIElementRoleFilter(void)
    {}

    void
    UIElementRoleFilter::add(AddValue newval)
    {
      targets_.push_back(newval);
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
