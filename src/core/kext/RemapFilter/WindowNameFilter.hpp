#ifndef WINDOWNAMEFILTER_HPP
#define WINDOWNAMEFILTER_HPP

#include "RemapFilterBase.hpp"

namespace org_pqrs_Karabiner {
namespace RemapFilter {
class WindowNameFilter final : public RemapFilterBase {
public:
  WindowNameFilter(unsigned int type, const unsigned int* vec, size_t length) : RemapFilterBase(type) {
    targets_.reserve(length);

    for (size_t i = 0; i < length; ++i) {
      targets_.push_back(WorkspaceWindowNameId(vec[i]));
    }
  }

  bool
  isblocked(void) override {
    if (get_type() == BRIDGE_FILTERTYPE_WINDOWNAME_NOT ||
        get_type() == BRIDGE_FILTERTYPE_WINDOWNAME_ONLY) {

      bool isnot = (get_type() == BRIDGE_FILTERTYPE_WINDOWNAME_NOT);

      for (size_t i = 0; i < targets_.size(); ++i) {
        if (CommonData::getcurrent_workspaceWindowNameIds().is_include(targets_[i])) {
          return isnot ? true : false;
        }
      }

      return isnot ? false : true;
    }

    IOLOG_ERROR("WindowNameFilter::isblocked unknown type(%d)\n", get_type());
    return false;
  }

private:
  Vector_WorkspaceWindowNameId targets_;
};
}
}

#endif
