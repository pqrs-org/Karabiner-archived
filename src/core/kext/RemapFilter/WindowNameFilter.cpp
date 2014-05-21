#include <IOKit/IOLib.h>

#include "WindowNameFilter.hpp"
#include "CommonData.hpp"
#include "IOLogWrapper.hpp"
#include "bridge.h"

namespace org_pqrs_KeyRemap4MacBook {
  namespace RemapFilter {
    WindowNameFilter::WindowNameFilter(unsigned int t) : type_(t)
    {}

    WindowNameFilter::~WindowNameFilter(void)
    {}

    void
    WindowNameFilter::add(AddValue newval)
    {
      targets_.push_back(newval);
    }

    bool
    WindowNameFilter::isblocked(void)
    {
      unsigned int current = CommonData::getcurrent_workspacedata().windowname;

      switch (type_) {
        case BRIDGE_FILTERTYPE_WINDOWNAME_NOT:
        case BRIDGE_FILTERTYPE_WINDOWNAME_ONLY:
        {
          bool isnot = (type_ == BRIDGE_FILTERTYPE_WINDOWNAME_NOT);

          for (size_t i = 0; i < targets_.size(); ++i) {
            if (targets_[i] == current) {
              return isnot ? true : false;
            }
          }
          return isnot ? false : true;
        }

        default:
          IOLOG_ERROR("WindowNameFilter::isblocked unknown type_(%d)\n", type_);
          break;
      }

      return false;
    }
  }
}
