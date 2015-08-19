#ifndef ELAPSEDTIMESINCELASTRELEASEDFILTER_HPP
#define ELAPSEDTIMESINCELASTRELEASEDFILTER_HPP

#include "CommonData.hpp"
#include "RemapFilterBase.hpp"

namespace org_pqrs_Karabiner {
namespace RemapFilter {
class ElapsedTimeSinceLastReleasedFilter final : public RemapFilterBase {
public:
  ElapsedTimeSinceLastReleasedFilter(unsigned int type, const unsigned int* vec, size_t length) : RemapFilterBase(type) {
    targets_.reserve(length / 2);

    for (int i = 0; i < static_cast<int>(length) - 1; i += 2) {
      targets_.push_back(AddValueWithDataType(AddDataType(vec[i]), AddValue(vec[i + 1])));
    }

    if (length % 2 > 0) {
      IOLOG_WARN("Invalid length(%d) in BRIDGE_FILTERTYPE_ELAPSEDTIMESINCELASTRELEASED_*\n", static_cast<int>(length));
    }
  }

  bool
  isblocked(void) override {
    const LastReleasedPhysicalKey& current = CommonData::getcurrent_lastreleasedphysicalkey();
    if (current.get_datatype() == BRIDGE_DATATYPE_NONE) return false;

    if (get_type() == BRIDGE_FILTERTYPE_ELAPSEDTIMESINCELASTRELEASED_GREATERTHAN ||
        get_type() == BRIDGE_FILTERTYPE_ELAPSEDTIMESINCELASTRELEASED_LESSTHAN) {

      uint32_t current_ms = current.get_milliseconds();

      for (size_t i = 0; i < targets_.size(); ++i) {
        uint32_t filter_ms = 0;
        if (targets_[i].dataType == BRIDGE_DATATYPE_MILLISECOND) {
          filter_ms = targets_[i].value;
        } else {
          IOLOG_ERROR("ElapsedTimeSinceLastReleasedFilter::isblocked unknown dataType(%ud)\n",
                      (unsigned int)(targets_[i].dataType));
          continue;
        }

        if (get_type() == BRIDGE_FILTERTYPE_ELAPSEDTIMESINCELASTRELEASED_GREATERTHAN) {
          if (filter_ms >= current_ms) return true;
        }
        if (get_type() == BRIDGE_FILTERTYPE_ELAPSEDTIMESINCELASTRELEASED_LESSTHAN) {
          if (filter_ms <= current_ms) return true;
        }
      }
      return false;
    }

    IOLOG_ERROR("ElapsedTimeSinceLastReleasedFilter::isblocked unknown type(%d)\n", get_type());
    return false;
  }

private:
  Vector_AddValueWithDataType targets_;
};
}
}

#endif
