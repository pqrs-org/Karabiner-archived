#include "base.hpp"
#include "bridge.h"
#include "CommonData.hpp"
#include "DeviceFilter.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace RemapFilter {
    DeviceFilter::DeviceFilter(unsigned int t) : type_(t)
    {
      targets_ = new Vector_DeviceFilterValue();
    }

    DeviceFilter::~DeviceFilter(void)
    {
      if (targets_) {
        delete targets_;
      }
    }

    void
    DeviceFilter::add(unsigned int vendorID, unsigned int productID, unsigned int locationID)
    {
      if (! targets_) return;

      targets_->push_back(DeviceFilterValue(vendorID, productID, locationID));
    }

    bool
    DeviceFilter::isblocked(void)
    {
      if (! targets_) return false;

      switch (type_) {
        case BRIDGE_FILTERTYPE_DEVICE_NOT:
        case BRIDGE_FILTERTYPE_DEVICE_ONLY:
        {
          bool isnot = (type_ == BRIDGE_FILTERTYPE_DEVICE_NOT);

          for (size_t i = 0; i < targets_->size(); ++i) {
            DeviceFilterValue& v = (*targets_)[i];
            if (CommonData::getcurrent_deviceIdentifier().isEqual(v.vendorID, v.productID, v.locationID)) {
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
