#include "bridge.h"
#include "ConfigFilter.hpp"
#include "Config.hpp"
#include "RemapClass.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace RemapFilter {
    ConfigFilter::ConfigFilter(unsigned int t) : type_(t)
    {
      targets_ = new Vector_FilterValue();
    }

    ConfigFilter::~ConfigFilter(void)
    {
      if (targets_) {
        delete targets_;
      }
    }

    void
    ConfigFilter::add(unsigned int newval)
    {
      if (! targets_) return;

      targets_->push_back(newval);
    }

    bool
    ConfigFilter::isblocked(void)
    {
      if (! targets_) return false;

      switch (type_) {
        case BRIDGE_FILTERTYPE_CONFIG_NOT:
        {
          for (size_t i = 0; i < targets_->size(); ++i) {
            if (RemapClassManager::isEnabled((*targets_)[i])) {
              return true;
            }
          }
          return false;
        }

        case BRIDGE_FILTERTYPE_CONFIG_ONLY:
        {
          for (size_t i = 0; i < targets_->size(); ++i) {
            if (RemapClassManager::isEnabled((*targets_)[i])) {
              return false;
            }
          }
          return true;
        }

        default:
          IOLOG_ERROR("ConfigFilter::isblocked unknown type_(%d)\n", type_);
          break;
      }

      return false;
    }
  }
}
