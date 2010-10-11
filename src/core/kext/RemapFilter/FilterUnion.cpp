#include "FilterUnion.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace RemapFilter {
    FilterUnion::FilterUnion(const unsigned int* vec, size_t length)
    {
      initialize(vec, length);
    }

    FilterUnion::~FilterUnion(void)
    {
      terminate();
    }

    void
    FilterUnion::initialize(const unsigned int* vec, size_t length)
    {
      type_ = BRIDGE_FILTERTYPE_NONE;

      // ------------------------------------------------------------
      // check parameters.
      //
      if (! vec || length <= 0) {
        IOLOG_ERROR("FilterUnion::FilterUnion invalid parameter.\n");
        goto error;
      }

      // ------------------------------------------------------------
      // initialize values.
      //
      type_ = vec[0];

      switch (type_) {
        case BRIDGE_FILTERTYPE_APPLICATION_NOT:
        case BRIDGE_FILTERTYPE_APPLICATION_ONLY:
          p_.applicationFilter = new ApplicationFilter(type_);
          if (p_.applicationFilter) {
            for (size_t i = 1; i < length; ++i) {
              (p_.applicationFilter)->add(vec[i]);
            }
          }
          break;

        case BRIDGE_FILTERTYPE_INPUTMODE_NOT:
        case BRIDGE_FILTERTYPE_INPUTMODE_ONLY:
        case BRIDGE_FILTERTYPE_INPUTMODEDETAIL_NOT:
        case BRIDGE_FILTERTYPE_INPUTMODEDETAIL_ONLY:
          p_.inputModeFilter = new InputModeFilter(type_);
          if (p_.inputModeFilter) {
            for (size_t i = 1; i < length; ++i) {
              (p_.inputModeFilter)->add(vec[i]);
            }
          }
          break;
      }

    error:
      terminate();
    }

    void
    FilterUnion::terminate(void)
    {
      switch (type_) {
        case BRIDGE_FILTERTYPE_APPLICATION_NOT:
        case BRIDGE_FILTERTYPE_APPLICATION_ONLY:
          if (p_.applicationFilter) {
            delete p_.applicationFilter;
          }
          break;

        case BRIDGE_FILTERTYPE_INPUTMODE_NOT:
        case BRIDGE_FILTERTYPE_INPUTMODE_ONLY:
        case BRIDGE_FILTERTYPE_INPUTMODEDETAIL_NOT:
        case BRIDGE_FILTERTYPE_INPUTMODEDETAIL_ONLY:
          if (p_.inputModeFilter) {
            delete p_.inputModeFilter;
          }
          break;
      }
    }

    bool
    FilterUnion::isblocked(void)
    {
      return false;
    }
  }
}
