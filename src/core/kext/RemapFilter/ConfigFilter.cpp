#include "bridge.h"
#include "ConfigFilter.hpp"
#include "Config.hpp"
#include "RemapClass.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace RemapFilter {
    ConfigFilter::ConfigFilter(unsigned int t) : type_(t)
    {}

    ConfigFilter::~ConfigFilter(void)
    {}

    void
    ConfigFilter::add(unsigned int newval)
    {
      targets_.push_back(newval);
    }

    bool
    ConfigFilter::isblocked(void)
    {
      switch (type_) {
        case BRIDGE_FILTERTYPE_CONFIG_NOT:
        case BRIDGE_FILTERTYPE_CONFIG_ONLY:
        {
          bool isnot = (type_ == BRIDGE_FILTERTYPE_CONFIG_NOT);

          for (size_t i = 0; i < targets_.size(); ++i) {
            if (RemapClassManager::isEnabled(targets_[i])) {
              return isnot ? true : false;
            }
          }
          return isnot ? false : true;
        }

        default:
          IOLOG_ERROR("ConfigFilter::isblocked unknown type_(%d)\n", type_);
          break;
      }

      return false;
    }
  }
}
