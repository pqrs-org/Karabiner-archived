#include <IOKit/IOLib.h>

#include "CommonData.hpp"
#include "DeviceFilter.hpp"
#include "IOLogWrapper.hpp"
#include "bridge.h"

namespace org_pqrs_Karabiner {
  namespace RemapFilter {
    void
    DeviceFilter::initialize(const unsigned int* vec, size_t length)
    {
      for (size_t i = 0; i < length - 2; i += 3) {
        targets_.push_back(DeviceIdentifier(DeviceVendor(vec[i]),
                                            DeviceProduct(vec[i + 1]),
                                            DeviceLocation(vec[i + 2])));
      }

      if (length % 3 > 0) {
        IOLOG_WARN("Invalid length(%d) in BRIDGE_FILTERTYPE_DEVICE_*\n", static_cast<int>(length));
      }
    }

    bool
    DeviceFilter::isblocked(void)
    {
      switch (type_) {
        case BRIDGE_FILTERTYPE_DEVICE_NOT:
        case BRIDGE_FILTERTYPE_DEVICE_ONLY:
        {
          bool isnot = (type_ == BRIDGE_FILTERTYPE_DEVICE_NOT);

          for (size_t i = 0; i < targets_.size(); ++i) {
            DeviceIdentifier& v = targets_[i];
            if (CommonData::getcurrent_deviceIdentifier().isEqual(v)) {
              return isnot ? true : false;
            }
          }
          return isnot ? false : true;
        }

        default:
          IOLOG_ERROR("DeviceFilter::isblocked unknown type_(%d)\n", type_);
          break;
      }

      return false;
    }
  }
}
