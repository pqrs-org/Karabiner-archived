#ifndef MODIFIERFILTER_HPP
#define MODIFIERFILTER_HPP

#include "FlagStatus.hpp"
#include "RemapFilterBase.hpp"

namespace org_pqrs_Karabiner {
namespace RemapFilter {
class ModifierFilter final : public RemapFilterBase {
public:
  ModifierFilter(unsigned int type, const unsigned int* vec, size_t length) : RemapFilterBase(type) {
    targets_.reserve(length / 2);

    {
      Vector_ModifierFlag v;
      targets_.push_back(v);
    }

    for (int i = 0; i < static_cast<int>(length) - 1; i += 2) {
      AddDataType datatype(vec[i]);
      AddValue newval(vec[i + 1]);

      switch (datatype) {
      case BRIDGE_DATATYPE_MODIFIERFLAG:
        if (!targets_.empty()) {
          targets_.back().push_back(ModifierFlag(datatype, newval));
        }
        break;

      case BRIDGE_DATATYPE_MODIFIERFLAGS_END: {
        Vector_ModifierFlag v;
        targets_.push_back(v);
        break;
      }

      default:
        IOLOG_ERROR("ModifierFilter::add invalid datatype:%u\n", static_cast<unsigned int>(datatype));
        break;
      }
    }

    if (length % 2 > 0) {
      IOLOG_WARN("Invalid length(%d) in BRIDGE_FILTERTYPE_MODIFIER_*\n", static_cast<int>(length));
    }
  }

  bool
  isblocked(void) override {
    if (get_type() == BRIDGE_FILTERTYPE_MODIFIER_NOT ||
        get_type() == BRIDGE_FILTERTYPE_MODIFIER_ONLY ||
        get_type() == BRIDGE_FILTERTYPE_MODIFIER_LOCKED_NOT ||
        get_type() == BRIDGE_FILTERTYPE_MODIFIER_LOCKED_ONLY ||
        get_type() == BRIDGE_FILTERTYPE_MODIFIER_STUCK_NOT ||
        get_type() == BRIDGE_FILTERTYPE_MODIFIER_STUCK_ONLY) {

      bool isnot = (get_type() == BRIDGE_FILTERTYPE_MODIFIER_NOT ||
                    get_type() == BRIDGE_FILTERTYPE_MODIFIER_LOCKED_NOT ||
                    get_type() == BRIDGE_FILTERTYPE_MODIFIER_STUCK_NOT);

      for (size_t i = 0; i < targets_.size(); ++i) {
        if (targets_[i].empty()) continue;

        if (get_type() == BRIDGE_FILTERTYPE_MODIFIER_NOT ||
            get_type() == BRIDGE_FILTERTYPE_MODIFIER_ONLY) {
          if (FlagStatus::globalFlagStatus().isOn(targets_[i])) {
            return isnot ? true : false;
          }
        }

        if (get_type() == BRIDGE_FILTERTYPE_MODIFIER_LOCKED_NOT ||
            get_type() == BRIDGE_FILTERTYPE_MODIFIER_LOCKED_ONLY) {
          if (FlagStatus::globalFlagStatus().isLocked(targets_[i])) {
            return isnot ? true : false;
          }
        }

        if (get_type() == BRIDGE_FILTERTYPE_MODIFIER_STUCK_NOT ||
            get_type() == BRIDGE_FILTERTYPE_MODIFIER_STUCK_ONLY) {
          if (FlagStatus::globalFlagStatus().isStuck(targets_[i])) {
            return isnot ? true : false;
          }
        }
      }

      return isnot ? false : true;
    }

    IOLOG_ERROR("ModifierFilter::isblocked unknown type(%d)\n", get_type());
    return false;
  }

private:
  Vector_Vector_ModifierFlag targets_;
};
}
}

#endif
