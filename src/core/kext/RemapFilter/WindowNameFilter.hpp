#ifndef WINDOWNAMEFILTER_HPP
#define WINDOWNAMEFILTER_HPP

#include "RemapFilterBase.hpp"

namespace org_pqrs_Karabiner {
  namespace RemapFilter {
    class WindowNameFilter : public RemapFilterBase {
    public:
      WindowNameFilter(unsigned int type) : RemapFilterBase(type) {}

      void initialize(const unsigned int* vec, size_t length) {
        for (size_t i = 0; i < length; ++i) {
          targets_.push_back(AddValue(vec[i]));
        }
      }

      bool isblocked(void) {
        if (get_type() == BRIDGE_FILTERTYPE_WINDOWNAME_NOT ||
            get_type() == BRIDGE_FILTERTYPE_WINDOWNAME_ONLY) {

          unsigned int current = CommonData::getcurrent_workspacedata().windowname;

          bool isnot = (get_type() == BRIDGE_FILTERTYPE_WINDOWNAME_NOT);

          for (size_t i = 0; i < targets_.size(); ++i) {
            if (targets_[i] == current) {
              return isnot ? true : false;
            }
          }

          return isnot ? false : true;
        }

        IOLOG_ERROR("WindowNameFilter::isblocked unknown type(%d)\n", get_type());
        return false;
      }

    private:
      Vector_AddValue targets_;
    };
  }
}

#endif
