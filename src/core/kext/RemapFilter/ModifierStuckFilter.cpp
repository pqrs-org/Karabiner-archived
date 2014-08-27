#include <IOKit/IOLib.h>

#include "FlagStatus.hpp"
#include "IOLogWrapper.hpp"
#include "ModifierStuckFilter.hpp"
#include "bridge.h"

namespace org_pqrs_Karabiner {
  namespace RemapFilter {
    ModifierStuckFilter::ModifierStuckFilter(unsigned int t) : type_(t)
    {
      Vector_ModifierFlag v;
      targets_.push_back(v);
    }

    ModifierStuckFilter::~ModifierStuckFilter(void)
    {}

    void
    ModifierStuckFilter::add(AddDataType datatype, AddValue newval)
    {
      switch (datatype) {
        case BRIDGE_DATATYPE_MODIFIERFLAG:
          if (! targets_.empty()) {
            targets_.back().push_back(ModifierFlag(datatype, newval));
          }
          break;

        case BRIDGE_DATATYPE_MODIFIERFLAGS_END:
        {
          Vector_ModifierFlag v;
          targets_.push_back(v);
          break;
        }

        default:
          IOLOG_ERROR("ModifierStuckFilter::add invalid datatype:%u\n", static_cast<unsigned int>(datatype));
          break;
      }
    }

    bool
    ModifierStuckFilter::isblocked(void)
    {
      switch (type_) {
        case BRIDGE_FILTERTYPE_MODIFIER_STUCK_NOT:
        case BRIDGE_FILTERTYPE_MODIFIER_STUCK_ONLY:
        {
          bool isnot = (type_ == BRIDGE_FILTERTYPE_MODIFIER_STUCK_NOT);

          for (size_t i = 0; i < targets_.size(); ++i) {
            if (targets_[i].empty()) continue;

            if (FlagStatus::globalFlagStatus().isStuck(targets_[i])) {
              return isnot ? true : false;
            }
          }
          return isnot ? false : true;
        }

        default:
          IOLOG_ERROR("ModifierStuckFilter::isblocked unknown type_(%d)\n", type_);
          break;
      }

      return false;
    }
  }
}
