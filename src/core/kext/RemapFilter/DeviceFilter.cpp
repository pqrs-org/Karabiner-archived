#include "base.hpp"
#include "bridge.h"
#include "bridge.hpp"
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
    DeviceFilter::add(unsigned int vendorID, unsigned int productID)
    {
      if (! targets_) return;

      targets_->push_back(DeviceFilterValue(vendorID, productID));
    }

    bool
    DeviceFilter::isblocked(void)
    {
      if (! targets_) return false;

      switch (type_) {
        case BRIDGE_FILTERTYPE_DEVICE_NOT:
        {
          for (size_t i = 0; i < targets_->size(); ++i) {
            DeviceFilterValue& v = (*targets_)[i];
            if (CommonData::isEqualVendorProduct(v.vendorID, v.productID)) {
              return true;
            }
          }
          return false;
        }

        case BRIDGE_FILTERTYPE_DEVICE_ONLY:
        {
          for (size_t i = 0; i < targets_->size(); ++i) {
            DeviceFilterValue& v = (*targets_)[i];
            if (CommonData::isEqualVendorProduct(v.vendorID, v.productID)) {
              return false;
            }
          }
          return true;
        }

        default:
          IOLOG_ERROR("DeviceFilter::isblocked unknown type_(%d)\n", type_);
          break;
      }

      return false;
    }
  }
}
