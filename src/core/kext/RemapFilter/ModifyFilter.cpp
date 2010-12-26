#include "base.hpp"
#include "bridge.h"
#include "bridge.hpp"
#include "FlagStatus.hpp"
#include "ModifyFilter.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace RemapFilter {
    ModifyFilter::ModifyFilter(unsigned int t) : type_(t)
    {
      targets_ = new Vector_FilterValue();
    }

    ModifyFilter::~ModifyFilter(void)
    {
      if (targets_) {
        delete targets_;
      }
    }

    void
    ModifyFilter::add(unsigned int newval)
    {
      if (! targets_) return;

      targets_->push_back(newval);
    }

    bool
    ModifyFilter::isblocked(void)
    {
      if (! targets_) return false;

      switch (type_) {
        case BRIDGE_FILTERTYPE_MODIFY_NOT:
        {
          for (size_t i = 0; i < targets_->size(); ++i) {
            if (FlagStatus::makeFlags().isOn((Flags)(*targets_)[i])) {
              return true;
            }
          }
          return false;
        }

        case BRIDGE_FILTERTYPE_MODIFY_ONLY:
        {
          for (size_t i = 0; i < targets_->size(); ++i) {
            if (FlagStatus::makeFlags().isOn((Flags)(*targets_)[i])) {
              return false;
            }
          }
          return true;
        }

        default:
          IOLOG_ERROR("ModifyFilter::isblocked unknown type_(%d)\n", type_);
          break;
      }

      return false;
    }
  }
}
