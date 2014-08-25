#include <IOKit/IOLib.h>

#include "FlagStatus.hpp"
#include "IOLogWrapper.hpp"
#include "ModifierLockedFilter.hpp"
#include "bridge.h"

namespace org_pqrs_Karabiner {
  namespace RemapFilter {
    ModifierLockedFilter::ModifierLockedFilter(unsigned int t) : type_(t)
    {
      Vector_ModifierFlag v;
      targets_.push_back(v);
    }

    ModifierLockedFilter::~ModifierLockedFilter(void)
    {}

    void
    ModifierLockedFilter::add(AddDataType datatype, AddValue newval)
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
          IOLOG_ERROR("ModifierLockedFilter::add invalid datatype:%u\n", static_cast<unsigned int>(datatype));
          break;
      }
    }

    bool
    ModifierLockedFilter::isblocked(void)
    {
      switch (type_) {
        case BRIDGE_FILTERTYPE_MODIFIER_NOT:
        case BRIDGE_FILTERTYPE_MODIFIER_ONLY:
        {
          bool isnot = (type_ == BRIDGE_FILTERTYPE_MODIFIER_NOT);

          for (size_t i = 0; i < targets_.size(); ++i) {
            if (targets_[i].empty()) continue;

            if (FlagStatus::globalFlagStatus().isLocked(targets_[i])) {
              return isnot ? true : false;
            }
          }
          return isnot ? false : true;
        }

        default:
          IOLOG_ERROR("ModifierLockedFilter::isblocked unknown type_(%d)\n", type_);
          break;
      }

      return false;
    }
  }
}
