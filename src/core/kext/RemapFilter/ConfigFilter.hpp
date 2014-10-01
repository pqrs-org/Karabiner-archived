#ifndef CONFIGFILTER_HPP
#define CONFIGFILTER_HPP

#include "Config.hpp"
#include "RemapClass.hpp"
#include "RemapFilterBase.hpp"

namespace org_pqrs_Karabiner {
namespace RemapFilter {
class ConfigFilter final : public RemapFilterBase {
public:
  ConfigFilter(unsigned int type, const unsigned int* vec, size_t length) : RemapFilterBase(type) {
    targets_.reserve(length);

    for (size_t i = 0; i < length; ++i) {
      targets_.push_back(AddValue(vec[i]));
    }
  }

  bool
  isblocked(void) override {
    if (get_type() == BRIDGE_FILTERTYPE_CONFIG_NOT ||
        get_type() == BRIDGE_FILTERTYPE_CONFIG_ONLY) {

      bool isnot = (get_type() == BRIDGE_FILTERTYPE_CONFIG_NOT);

      for (size_t i = 0; i < targets_.size(); ++i) {
        if (RemapClassManager::isEnabled(targets_[i])) {
          return isnot ? true : false;
        }
      }

      return isnot ? false : true;
    }

    IOLOG_ERROR("ConfigFilter::isblocked unknown type(%d)\n", get_type());
    return false;
  }

private:
  Vector_AddValue targets_;
};
}
}

#endif
