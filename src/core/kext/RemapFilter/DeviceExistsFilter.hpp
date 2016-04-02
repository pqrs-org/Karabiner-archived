#ifndef DEVICEEXISTSFILTER_HPP
#define DEVICEEXISTSFILTER_HPP

#include "CommonData.hpp"
#include "RemapFilterBase.hpp"

namespace org_pqrs_Karabiner {
namespace RemapFilter {
class DeviceExistsFilter final : public RemapFilterBase {
public:
  DeviceExistsFilter(unsigned int type, const unsigned int* vec, size_t length) : RemapFilterBase(type) {
    targets_.reserve(length / 3);

    for (int i = 0; i < static_cast<int>(length) - 2; i += 3) {
      targets_.push_back(DeviceIdentifier(DeviceVendor(vec[i]),
                                          DeviceProduct(vec[i + 1]),
                                          DeviceLocation(vec[i + 2])));
    }

    if (length % 3 > 0) {
      IOLOG_WARN("Invalid length(%d) in BRIDGE_FILTERTYPE_DEVICEEXISTS_*\n", static_cast<int>(length));
    }
  }

  bool isblocked(void) override {
    if (get_type() == BRIDGE_FILTERTYPE_DEVICEEXISTS_NOT ||
        get_type() == BRIDGE_FILTERTYPE_DEVICEEXISTS_ONLY) {

      bool isnot = (get_type() == BRIDGE_FILTERTYPE_DEVICEEXISTS_NOT);

      for (size_t i = 0; i < targets_.size(); ++i) {
        DeviceIdentifier& v = targets_[i];
        if (ListHookedDevice::existsAll(v)) {
          return isnot ? true : false;
        }
      }

      return isnot ? false : true;
    }

    IOLOG_ERROR("DeviceExistsFilter::isblocked unknown type(%d)\n", get_type());
    return false;
  }

  bool isblocked_keyup(void) override { return isblocked(); }

private:
  Vector_DeviceIdentifier targets_;
};
}
}

#endif
