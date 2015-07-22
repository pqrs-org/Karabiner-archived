#ifndef INPUTSOURCEFILTER_HPP
#define INPUTSOURCEFILTER_HPP

#include "CommonData.hpp"
#include "RemapFilterBase.hpp"

namespace org_pqrs_Karabiner {
namespace RemapFilter {
class InputSourceFilter final : public RemapFilterBase {
public:
  InputSourceFilter(unsigned int type, const unsigned int* vec, size_t length) : RemapFilterBase(type) {
    targets_.reserve(length);

    for (size_t i = 0; i < length; ++i) {
      targets_.push_back(WorkspaceInputSourceId(vec[i]));
    }
  }

  bool
  isblocked(void) override {
    if (get_type() == BRIDGE_FILTERTYPE_INPUTSOURCE_NOT ||
        get_type() == BRIDGE_FILTERTYPE_INPUTSOURCE_ONLY) {

      bool isnot = (get_type() == BRIDGE_FILTERTYPE_INPUTSOURCE_NOT);

      for (size_t i = 0; i < targets_.size(); ++i) {
        if (CommonData::getcurrent_workspaceInputSourceIds().is_include(targets_[i])) {
          return isnot ? true : false;
        }
      }

      return isnot ? false : true;
    }

    IOLOG_ERROR("InputSourceFilter::isblocked unknown type(%d)\n", get_type());
    return false;
  }

private:
  Vector_WorkspaceInputSourceId targets_;
};
}
}

#endif
