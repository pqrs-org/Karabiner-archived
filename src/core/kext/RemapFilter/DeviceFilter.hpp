#ifndef DEVICEFILTER_HPP
#define DEVICEFILTER_HPP

#include "CommonData.hpp"
#include "RemapFilterBase.hpp"

namespace org_pqrs_Karabiner {
namespace RemapFilter {
class DeviceFilter final : public RemapFilterBase {
public:
  DeviceFilter(unsigned int type, const unsigned int* vec, size_t length) : RemapFilterBase(type) {
    targets_.reserve(length / 3);

    for (int i = 0; i < static_cast<int>(length) - 2; i += 3) {
      targets_.push_back(DeviceIdentifier(DeviceVendor(vec[i]),
                                          DeviceProduct(vec[i + 1]),
                                          DeviceLocation(vec[i + 2])));
    }

    if (length % 3 > 0) {
      IOLOG_WARN("Invalid length(%d) in BRIDGE_FILTERTYPE_DEVICE_*\n", static_cast<int>(length));
    }
  }

  bool
  isblocked(void) override {
    if (get_type() == BRIDGE_FILTERTYPE_DEVICE_NOT ||
        get_type() == BRIDGE_FILTERTYPE_DEVICE_ONLY) {

      bool isnot = (get_type() == BRIDGE_FILTERTYPE_DEVICE_NOT);

      for (size_t i = 0; i < targets_.size(); ++i) {
        DeviceIdentifier& v = targets_[i];
        if (CommonData::getcurrent_deviceIdentifier().isEqual(v)) {
          return isnot ? true : false;
        }
      }

      return isnot ? false : true;
    }

    IOLOG_ERROR("DeviceFilter::isblocked unknown type(%d)\n", get_type());
    return false;
  }

  bool isblocked_keyup(void) override { return isblocked(); }

private:
  Vector_DeviceIdentifier targets_;
};
}
}

#endif
