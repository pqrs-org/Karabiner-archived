#include <IOKit/IOLib.h>

#include "FilterUnion.hpp"
#include "IOLogWrapper.hpp"

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
      if (! vec || length == 0) {
        IOLOG_ERROR("FilterUnion::initialize invalid parameter.\n");
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

        case BRIDGE_FILTERTYPE_CONFIG_NOT:
        case BRIDGE_FILTERTYPE_CONFIG_ONLY:
          p_.configFilter = new ConfigFilter(type_);
          if (p_.configFilter) {
            for (size_t i = 1; i < length; ++i) {
              (p_.configFilter)->add(vec[i]);
            }
          }
          break;

        case BRIDGE_FILTERTYPE_DEVICE_NOT:
        case BRIDGE_FILTERTYPE_DEVICE_ONLY:
          p_.deviceFilter = new DeviceFilter(type_);
          if (p_.deviceFilter) {
            for (size_t i = 1; i < length - 2; i += 3) {
              (p_.deviceFilter)->add(vec[i], vec[i + 1], vec[i + 2]);
            }
            if ((length - 1) % 3 > 0) {
              IOLOG_WARN("Invalid length(%d) in BRIDGE_FILTERTYPE_DEVICE_*\n", static_cast<int>(length));
            }
          }
          break;

        case BRIDGE_FILTERTYPE_ELAPSEDTIMESINCELASTPRESSED_GREATERTHAN:
        case BRIDGE_FILTERTYPE_ELAPSEDTIMESINCELASTPRESSED_LESSTHAN:
          p_.elapsedTimeSinceLastPressedFilter = new ElapsedTimeSinceLastPressedFilter(type_);
          if (p_.elapsedTimeSinceLastPressedFilter) {
            for (size_t i = 1; i < length - 1; i += 2) {
              (p_.elapsedTimeSinceLastPressedFilter)->add(vec[i], vec[i + 1]);
            }
          }
          break;

        case BRIDGE_FILTERTYPE_INPUTSOURCE_NOT:
        case BRIDGE_FILTERTYPE_INPUTSOURCE_ONLY:
        case BRIDGE_FILTERTYPE_INPUTSOURCEDETAIL_NOT:
        case BRIDGE_FILTERTYPE_INPUTSOURCEDETAIL_ONLY:
          p_.inputSourceFilter = new InputSourceFilter(type_);
          if (p_.inputSourceFilter) {
            for (size_t i = 1; i < length; ++i) {
              (p_.inputSourceFilter)->add(vec[i]);
            }
          }
          break;

        case BRIDGE_FILTERTYPE_LASTPRESSEDPHYSICALKEY_NOT:
        case BRIDGE_FILTERTYPE_LASTPRESSEDPHYSICALKEY_ONLY:
          p_.lastPressedPhysicalKeyFilter = new LastPressedPhysicalKeyFilter(type_);
          if (p_.lastPressedPhysicalKeyFilter) {
            for (size_t i = 1; i < length - 1; i += 2) {
              (p_.lastPressedPhysicalKeyFilter)->add(vec[i], vec[i + 1]);
            }
          }
          break;

        case BRIDGE_FILTERTYPE_MODIFIER_NOT:
        case BRIDGE_FILTERTYPE_MODIFIER_ONLY:
          p_.modifierFilter = new ModifierFilter(type_);
          if (p_.modifierFilter) {
            for (size_t i = 1; i < length; ++i) {
              (p_.modifierFilter)->add(vec[i]);
            }
          }
          break;

        default:
          IOLOG_ERROR("FilterUnion::initialize unknown type_:%d.\n", type_);
          goto error;
      }

      return;

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

        case BRIDGE_FILTERTYPE_CONFIG_NOT:
        case BRIDGE_FILTERTYPE_CONFIG_ONLY:
          if (p_.configFilter) {
            delete p_.configFilter;
          }
          break;

        case BRIDGE_FILTERTYPE_DEVICE_NOT:
        case BRIDGE_FILTERTYPE_DEVICE_ONLY:
          if (p_.deviceFilter) {
            delete p_.deviceFilter;
          }
          break;

        case BRIDGE_FILTERTYPE_ELAPSEDTIMESINCELASTPRESSED_GREATERTHAN:
        case BRIDGE_FILTERTYPE_ELAPSEDTIMESINCELASTPRESSED_LESSTHAN:
          if (p_.elapsedTimeSinceLastPressedFilter) {
            delete p_.elapsedTimeSinceLastPressedFilter;
          }
          break;

        case BRIDGE_FILTERTYPE_INPUTSOURCE_NOT:
        case BRIDGE_FILTERTYPE_INPUTSOURCE_ONLY:
        case BRIDGE_FILTERTYPE_INPUTSOURCEDETAIL_NOT:
        case BRIDGE_FILTERTYPE_INPUTSOURCEDETAIL_ONLY:
          if (p_.inputSourceFilter) {
            delete p_.inputSourceFilter;
          }
          break;

        case BRIDGE_FILTERTYPE_LASTPRESSEDPHYSICALKEY_NOT:
        case BRIDGE_FILTERTYPE_LASTPRESSEDPHYSICALKEY_ONLY:
          if (p_.lastPressedPhysicalKeyFilter) {
            delete p_.lastPressedPhysicalKeyFilter;
          }
          break;

        case BRIDGE_FILTERTYPE_MODIFIER_NOT:
        case BRIDGE_FILTERTYPE_MODIFIER_ONLY:
          if (p_.modifierFilter) {
            delete p_.modifierFilter;
          }
          break;
      }

      type_ = BRIDGE_FILTERTYPE_NONE;
    }

    bool
    FilterUnion::isblocked(void)
    {
      switch (type_) {
        case BRIDGE_FILTERTYPE_APPLICATION_NOT:
        case BRIDGE_FILTERTYPE_APPLICATION_ONLY:
          if (p_.applicationFilter) {
            return (p_.applicationFilter)->isblocked();
          }
          break;

        case BRIDGE_FILTERTYPE_CONFIG_NOT:
        case BRIDGE_FILTERTYPE_CONFIG_ONLY:
          if (p_.configFilter) {
            return (p_.configFilter)->isblocked();
          }
          break;

        case BRIDGE_FILTERTYPE_DEVICE_NOT:
        case BRIDGE_FILTERTYPE_DEVICE_ONLY:
          if (p_.deviceFilter) {
            return (p_.deviceFilter)->isblocked();
          }
          break;

        case BRIDGE_FILTERTYPE_ELAPSEDTIMESINCELASTPRESSED_GREATERTHAN:
        case BRIDGE_FILTERTYPE_ELAPSEDTIMESINCELASTPRESSED_LESSTHAN:
          if (p_.elapsedTimeSinceLastPressedFilter) {
            return (p_.elapsedTimeSinceLastPressedFilter)->isblocked();
          }
          break;

        case BRIDGE_FILTERTYPE_INPUTSOURCE_NOT:
        case BRIDGE_FILTERTYPE_INPUTSOURCE_ONLY:
        case BRIDGE_FILTERTYPE_INPUTSOURCEDETAIL_NOT:
        case BRIDGE_FILTERTYPE_INPUTSOURCEDETAIL_ONLY:
          if (p_.inputSourceFilter) {
            return (p_.inputSourceFilter)->isblocked();
          }
          break;

        case BRIDGE_FILTERTYPE_LASTPRESSEDPHYSICALKEY_NOT:
        case BRIDGE_FILTERTYPE_LASTPRESSEDPHYSICALKEY_ONLY:
          if (p_.lastPressedPhysicalKeyFilter) {
            return (p_.lastPressedPhysicalKeyFilter)->isblocked();
          }
          break;

        case BRIDGE_FILTERTYPE_MODIFIER_NOT:
        case BRIDGE_FILTERTYPE_MODIFIER_ONLY:
          if (p_.modifierFilter) {
            return (p_.modifierFilter)->isblocked();
          }
          break;
      }

      return false;
    }
  }
}
