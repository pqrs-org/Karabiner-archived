#ifndef DEVICEFILTER_HPP
#define DEVICEFILTER_HPP

#include "bridge.h"
#include "CommonData.hpp"
#include "DeviceFilter.hpp"
#include "IOLogWrapper.hpp"
#include "RemapFilterBase.hpp"

namespace org_pqrs_Karabiner {
  namespace RemapFilter {
    class DeviceFilter : public RemapFilterBase {
    public:
      DeviceFilter(unsigned int type) : RemapFilterBase(type) {};

      void initialize(const unsigned int* vec, size_t length) {
        for (size_t i = 0; i < length - 2; i += 3) {
          targets_.push_back(DeviceIdentifier(DeviceVendor(vec[i]),
                                              DeviceProduct(vec[i + 1]),
                                              DeviceLocation(vec[i + 2])));
        }

        if (length % 3 > 0) {
          IOLOG_WARN("Invalid length(%d) in BRIDGE_FILTERTYPE_DEVICE_*\n", static_cast<int>(length));
        }
      }

      bool isblocked(void) {
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

    private:
      Vector_DeviceIdentifier targets_;
    };
  }
}

#endif
