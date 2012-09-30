#include "base.hpp"
#include "bridge.h"
#include "FlagStatus.hpp"
#include "ModifierFilter.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace RemapFilter {
    ModifierFilter::ModifierFilter(unsigned int t) : type_(t)
    {}

    ModifierFilter::~ModifierFilter(void)
    {}

    void
    ModifierFilter::add(unsigned int newval)
    {
      targets_.push_back(newval);
    }

    bool
    ModifierFilter::isblocked(void)
    {
      switch (type_) {
        case BRIDGE_FILTERTYPE_MODIFIER_NOT:
        case BRIDGE_FILTERTYPE_MODIFIER_ONLY:
        {
          bool isnot = (type_ == BRIDGE_FILTERTYPE_MODIFIER_NOT);

          Flags current = FlagStatus::makeFlags();
          for (size_t i = 0; i < targets_.size(); ++i) {
            Flags f(targets_[i]);
            if (current.isOn(f)) {
              return isnot ? true : false;
            }
          }
          return isnot ? false : true;
        }

        default:
          IOLOG_ERROR("ModifierFilter::isblocked unknown type_(%d)\n", type_);
          break;
      }

      return false;
    }
  }
}
