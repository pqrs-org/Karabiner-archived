#ifndef APPLICATIONFILTER_HPP
#define APPLICATIONFILTER_HPP

#include "CommonData.hpp"
#include "RemapFilterBase.hpp"

namespace org_pqrs_Karabiner {
namespace RemapFilter {
class ApplicationFilter final : public RemapFilterBase {
public:
  ApplicationFilter(unsigned int type, const unsigned int* vec, size_t length) : RemapFilterBase(type) {
    targets_.reserve(length);

    for (size_t i = 0; i < length; ++i) {
      targets_.push_back(WorkspaceAppId(vec[i]));
    }
  }

  bool isblocked(void) override {
    if (get_type() == BRIDGE_FILTERTYPE_APPLICATION_NOT ||
        get_type() == BRIDGE_FILTERTYPE_APPLICATION_ONLY) {

      bool isnot = (get_type() == BRIDGE_FILTERTYPE_APPLICATION_NOT);

      for (size_t i = 0; i < targets_.size(); ++i) {
        if (CommonData::getcurrent_workspaceAppIds().is_include(targets_[i])) {
          return isnot ? true : false;
        }
      }

      return isnot ? false : true;
    }

    IOLOG_ERROR("ApplicationFilter::isblocked unknown type(%d)\n", get_type());
    return false;
  }

private:
  Vector_WorkspaceAppId targets_;
};
}
}

#endif
