#include <IOKit/IOLib.h>

#include "CommonData.hpp"
#include "IOLogWrapper.hpp"
#include "InputSourceFilter.hpp"
#include "bridge.h"

namespace org_pqrs_KeyRemap4MacBook {
  namespace RemapFilter {
    InputSourceFilter::InputSourceFilter(unsigned int t) : type_(t)
    {}

    InputSourceFilter::~InputSourceFilter(void)
    {}

    void
    InputSourceFilter::add(AddValue newval)
    {
      targets_.push_back(newval);
    }

    bool
    InputSourceFilter::isblocked(void)
    {
      unsigned int current = 0;
      switch (type_) {
        case BRIDGE_FILTERTYPE_INPUTSOURCE_NOT:
        case BRIDGE_FILTERTYPE_INPUTSOURCE_ONLY:
          current = CommonData::getcurrent_workspacedata().inputsource;
          break;

        case BRIDGE_FILTERTYPE_INPUTSOURCEDETAIL_NOT:
        case BRIDGE_FILTERTYPE_INPUTSOURCEDETAIL_ONLY:
          current = CommonData::getcurrent_workspacedata().inputsourcedetail;
          break;
      }

      switch (type_) {
        case BRIDGE_FILTERTYPE_INPUTSOURCE_NOT:
        case BRIDGE_FILTERTYPE_INPUTSOURCEDETAIL_NOT:
        case BRIDGE_FILTERTYPE_INPUTSOURCE_ONLY:
        case BRIDGE_FILTERTYPE_INPUTSOURCEDETAIL_ONLY:
        {
          bool isnot = (type_ == BRIDGE_FILTERTYPE_INPUTSOURCE_NOT || type_ == BRIDGE_FILTERTYPE_INPUTSOURCEDETAIL_NOT);

          for (size_t i = 0; i < targets_.size(); ++i) {
            if (targets_[i] == current) {
              return isnot ? true : false;
            }
          }
          return isnot ? false : true;
        }

        default:
          IOLOG_ERROR("InputSourceFilter::isblocked unknown type_(%d)\n", type_);
          break;
      }

      return false;
    }
  }
}
