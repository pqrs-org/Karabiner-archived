#ifndef INPUTSOURCEFILTER_HPP
#define INPUTSOURCEFILTER_HPP

#include "CommonData.hpp"
#include "RemapFilterBase.hpp"

namespace org_pqrs_Karabiner {
namespace RemapFilter {
class InputSourceFilter : public RemapFilterBase {
public:
  InputSourceFilter(unsigned int type, const unsigned int* vec, size_t length) : RemapFilterBase(type) {
    targets_.reserve(length);

    for (size_t i = 0; i < length; ++i) {
      targets_.push_back(AddValue(vec[i]));
    }
  }

  bool
  isblocked(void) {
    if (get_type() == BRIDGE_FILTERTYPE_INPUTSOURCE_NOT ||
        get_type() == BRIDGE_FILTERTYPE_INPUTSOURCEDETAIL_NOT ||
        get_type() == BRIDGE_FILTERTYPE_INPUTSOURCE_ONLY ||
        get_type() == BRIDGE_FILTERTYPE_INPUTSOURCEDETAIL_ONLY) {

      unsigned int current = 0;
      if (get_type() == BRIDGE_FILTERTYPE_INPUTSOURCE_NOT ||
          get_type() == BRIDGE_FILTERTYPE_INPUTSOURCE_ONLY) {
        current = CommonData::getcurrent_workspacedata().inputsource;
      }
      if (get_type() == BRIDGE_FILTERTYPE_INPUTSOURCEDETAIL_NOT ||
          get_type() == BRIDGE_FILTERTYPE_INPUTSOURCEDETAIL_ONLY) {
        current = CommonData::getcurrent_workspacedata().inputsourcedetail;
      }

      bool isnot = (get_type() == BRIDGE_FILTERTYPE_INPUTSOURCE_NOT ||
                    get_type() == BRIDGE_FILTERTYPE_INPUTSOURCEDETAIL_NOT);

      for (size_t i = 0; i < targets_.size(); ++i) {
        if (targets_[i] == current) {
          return isnot ? true : false;
        }
      }

      return isnot ? false : true;
    }

    IOLOG_ERROR("InputSourceFilter::isblocked unknown type(%d)\n", get_type());
    return false;
  }

private:
  Vector_AddValue targets_;
};
}
}

#endif
