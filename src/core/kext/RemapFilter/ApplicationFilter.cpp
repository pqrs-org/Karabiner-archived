#include "base.hpp"
#include "bridge.h"
#include "bridge.hpp"
#include "CommonData.hpp"
#include "ApplicationFilter.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace RemapFilter {
    ApplicationFilter::ApplicationFilter(unsigned int t) : type_(t)
    {
      targets_ = new Vector_FilterValue();
    }

    ApplicationFilter::~ApplicationFilter(void)
    {
      if (targets_) {
        delete targets_;
      }
    }

    void
    ApplicationFilter::add(unsigned int newval)
    {
      if (! targets_) return;

      targets_->push_back(newval);
    }

    bool
    ApplicationFilter::isblocked(void)
    {
      if (! targets_) return false;

      unsigned int current = CommonData::getcurrent_workspacedata().type;

      switch (type_) {
        case BRIDGE_FILTERTYPE_APPLICATION_NOT:
        {
          for (size_t i = 0; i < targets_->size(); ++i) {
            if ((*targets_)[i] == current) {
              return true;
            }
          }
          return false;
        }

        case BRIDGE_FILTERTYPE_APPLICATION_ONLY:
        {
          for (size_t i = 0; i < targets_->size(); ++i) {
            if ((*targets_)[i] == current) {
              return false;
            }
          }
          return true;
        }

        default:
          IOLOG_ERROR("ApplicationFilter::isblocked unknown type_(%d)\n", type_);
          break;
      }

      return false;
    }
  }
}
