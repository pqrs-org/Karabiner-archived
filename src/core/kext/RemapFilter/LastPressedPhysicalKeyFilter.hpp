#ifndef LASTPRESSEDPHYSICALKEYFILTER_HPP
#define LASTPRESSEDPHYSICALKEYFILTER_HPP

#include "CommonData.hpp"
#include "RemapFilterBase.hpp"

namespace org_pqrs_Karabiner {
namespace RemapFilter {
class LastPressedPhysicalKeyFilter final : public RemapFilterBase {
public:
  LastPressedPhysicalKeyFilter(unsigned int type, const unsigned int* vec, size_t length) : RemapFilterBase(type) {
    targets_.reserve(length / 2);

    for (int i = 0; i < static_cast<int>(length) - 1; i += 2) {
      targets_.push_back(AddValueWithDataType(AddDataType(vec[i]), AddValue(vec[i + 1])));
    }

    if (length % 2 > 0) {
      IOLOG_WARN("Invalid length(%d) in BRIDGE_FILTERTYPE_LASTPRESSEDPHYSICALKEY_*\n", static_cast<int>(length));
    }
  }

  bool
  isblocked(void) override {
    if (get_type() == BRIDGE_FILTERTYPE_LASTPRESSEDPHYSICALKEY_NOT ||
        get_type() == BRIDGE_FILTERTYPE_LASTPRESSEDPHYSICALKEY_ONLY) {

      const LastPressedPhysicalKey& current = CommonData::getcurrent_lastpressedphysicalkey();
      if (current.get_datatype() == BRIDGE_DATATYPE_NONE) return false;

      bool isnot = (get_type() == BRIDGE_FILTERTYPE_LASTPRESSEDPHYSICALKEY_NOT);

      for (size_t i = 0; i < targets_.size(); ++i) {
        if (targets_[i].dataType == current.get_datatype() &&
            targets_[i].value == current.get_value()) {
          return isnot ? true : false;
        }
      }

      return isnot ? false : true;
    }

    IOLOG_ERROR("LastPressedPhysicalKeyFilter::isblocked unknown type(%d)\n", get_type());
    return false;
  }

private:
  Vector_AddValueWithDataType targets_;
};
}
}

#endif
