#include "base.hpp"
#include "bridge.h"
#include "bridge.hpp"
#include "CommonData.hpp"
#include "InputModeFilter.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace RemapFilter {
    InputModeFilter::InputModeFilter(unsigned int t) : type_(t)
    {
      targets_ = new Vector_FilterValue();
    }

    InputModeFilter::~InputModeFilter(void)
    {
      if (targets_) {
        delete targets_;
      }
    }

    void
    InputModeFilter::add(unsigned int newval)
    {
      if (! targets_) return;

      targets_->push_back(newval);
    }

    bool
    InputModeFilter::isblocked(void)
    {
      if (! targets_) return false;

      unsigned int current = 0;
      switch (type_) {
        case BRIDGE_FILTERTYPE_INPUTMODE_NOT:
        case BRIDGE_FILTERTYPE_INPUTMODE_ONLY:
          current = CommonData::getcurrent_workspacedata().inputmode;
          break;

        case BRIDGE_FILTERTYPE_INPUTMODEDETAIL_NOT:
        case BRIDGE_FILTERTYPE_INPUTMODEDETAIL_ONLY:
          current = CommonData::getcurrent_workspacedata().inputmodedetail;
          break;
      }

      switch (type_) {
        case BRIDGE_FILTERTYPE_INPUTMODE_NOT:
        case BRIDGE_FILTERTYPE_INPUTMODEDETAIL_NOT:
        {
          for (size_t i = 0; i < targets_->size(); ++i) {
            if ((*targets_)[i] == current) {
              return true;
            }
          }
          return false;
        }

        case BRIDGE_FILTERTYPE_INPUTMODE_ONLY:
        case BRIDGE_FILTERTYPE_INPUTMODEDETAIL_ONLY:
        {
          for (size_t i = 0; i < targets_->size(); ++i) {
            if ((*targets_)[i] == current) {
              return false;
            }
          }
          return true;
        }

        default:
          IOLOG_ERROR("InputModeFilter::isblocked unknown type_(%d)\n", type_);
          break;
      }

      return false;
    }
  }
}
