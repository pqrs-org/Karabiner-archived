#include <IOKit/IOLib.h>

#include "CommonData.hpp"
#include "DeviceFilter.hpp"
#include "IOLogWrapper.hpp"
#include "bridge.h"

namespace org_pqrs_KeyRemap4MacBook {
  namespace RemapFilter {
    DeviceFilter::DeviceFilter(unsigned int t) : type_(t)
    {}

    DeviceFilter::~DeviceFilter(void)
    {}

    void
    DeviceFilter::add(unsigned int vendorID, unsigned int productID, unsigned int locationID)
    {
      targets_.push_back(DeviceIdentifier(vendorID, productID, locationID));
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
