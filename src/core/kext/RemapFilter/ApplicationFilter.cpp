#include <IOKit/IOLib.h>

#include "ApplicationFilter.hpp"
#include "CommonData.hpp"
#include "IOLogWrapper.hpp"
#include "bridge.h"

namespace org_pqrs_KeyRemap4MacBook {
  namespace RemapFilter {
    ApplicationFilter::ApplicationFilter(unsigned int t) : type_(t)
    {}

    ApplicationFilter::~ApplicationFilter(void)
    {}

    void
    ApplicationFilter::add(unsigned int newval)
    {
      targets_.push_back(newval);
    }

    bool
    ApplicationFilter::isblocked(void)
    {
      unsigned int current = CommonData::getcurrent_workspacedata().applicationtype;

      switch (type_) {
        case BRIDGE_FILTERTYPE_APPLICATION_NOT:
        case BRIDGE_FILTERTYPE_APPLICATION_ONLY:
        {
          bool isnot = (type_ == BRIDGE_FILTERTYPE_APPLICATION_NOT);

          for (size_t i = 0; i < targets_.size(); ++i) {
            if (targets_[i] == current) {
              return isnot ? true : false;
            }
          }
          return isnot ? false : true;
        }

        default:
          IOLOG_ERROR("ApplicationFilter::isblocked unknown type_(%d)\n", type_);
          break;
      }

      return false;
    }
  }
}
