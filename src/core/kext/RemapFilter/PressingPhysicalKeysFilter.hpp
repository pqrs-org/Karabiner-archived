#ifndef PRESSINGKEYSFILTER_HPP
#define PRESSINGKEYSFILTER_HPP

#include "CommonData.hpp"
#include "RemapFilterBase.hpp"
#include "ListHookedDevice.hpp"

namespace org_pqrs_Karabiner {
namespace RemapFilter {
class PressingPhysicalKeysFilter final : public RemapFilterBase {
public:
  PressingPhysicalKeysFilter(unsigned int type, const unsigned int* vec, size_t length) : RemapFilterBase(type), count_(0) {
    for (size_t i = 0; i < length; ++i) {
      count_ = vec[i];
    }
  }

  bool
  isblocked(void) override {
    if (get_type() == BRIDGE_FILTERTYPE_PRESSINGPHYSICALKEYS_GREATERTHAN ||
        get_type() == BRIDGE_FILTERTYPE_PRESSINGPHYSICALKEYS_LESSTHAN) {

      auto current = ListHookedDevice::pressingPhysicalKeysCountAll();

      if (get_type() == BRIDGE_FILTERTYPE_PRESSINGPHYSICALKEYS_GREATERTHAN) {
        if (count_ >= current) return true;
      }
      if (get_type() == BRIDGE_FILTERTYPE_PRESSINGPHYSICALKEYS_LESSTHAN) {
        if (count_ <= current) return true;
      }
      return false;
    }

    IOLOG_ERROR("PressingPhysicalKeysFilter::isblocked unknown type(%d)\n", get_type());
    return false;
  }

private:
  size_t count_;
};
}
}

#endif
