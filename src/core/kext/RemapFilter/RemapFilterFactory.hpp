#ifndef REMAPFILTERFACTORY_HPP
#define REMAPFILTERFACTORY_HPP

#include "bridge.h"
#include "ApplicationFilter.hpp"
#include "ConfigFilter.hpp"
#include "DeviceFilter.hpp"
#include "ElapsedTimeSinceLastPressedFilter.hpp"
#include "ElapsedTimeSinceLastReleasedFilter.hpp"
#include "IOLogWrapper.hpp"
#include "InputSourceFilter.hpp"
#include "LastPressedPhysicalKeyFilter.hpp"
#include "LastReleasedPhysicalKeyFilter.hpp"
#include "LastSentEventFilter.hpp"
#include "ModifierFilter.hpp"
#include "PressingPhysicalKeysFilter.hpp"
#include "UIElementRoleFilter.hpp"
#include "WindowNameFilter.hpp"

namespace org_pqrs_Karabiner {
namespace RemapFilter {
class RemapFilterFactory final {
public:
  static RemapFilterBase* create(const unsigned int* vec, size_t length) {
    // ------------------------------------------------------------
    // check parameters.
    //
    if (!vec || length == 0) {
      IOLOG_ERROR("RemapFilterFactory::create invalid parameter %p, %ld.\n", vec, length);
      return nullptr;
    }

    // ------------------------------------------------------------
    unsigned int type = vec[0];
    ++vec;
    --length;

    switch (type) {
    case BRIDGE_FILTERTYPE_APPLICATION_NOT:
    case BRIDGE_FILTERTYPE_APPLICATION_ONLY:
      return new ApplicationFilter(type, vec, length);

    case BRIDGE_FILTERTYPE_CONFIG_NOT:
    case BRIDGE_FILTERTYPE_CONFIG_ONLY:
      return new ConfigFilter(type, vec, length);

    case BRIDGE_FILTERTYPE_DEVICE_NOT:
    case BRIDGE_FILTERTYPE_DEVICE_ONLY:
      return new DeviceFilter(type, vec, length);

    case BRIDGE_FILTERTYPE_ELAPSEDTIMESINCELASTPRESSED_GREATERTHAN:
    case BRIDGE_FILTERTYPE_ELAPSEDTIMESINCELASTPRESSED_LESSTHAN:
      return new ElapsedTimeSinceLastPressedFilter(type, vec, length);

    case BRIDGE_FILTERTYPE_ELAPSEDTIMESINCELASTRELEASED_GREATERTHAN:
    case BRIDGE_FILTERTYPE_ELAPSEDTIMESINCELASTRELEASED_LESSTHAN:
      return new ElapsedTimeSinceLastReleasedFilter(type, vec, length);

    case BRIDGE_FILTERTYPE_INPUTSOURCE_NOT:
    case BRIDGE_FILTERTYPE_INPUTSOURCE_ONLY:
      return new InputSourceFilter(type, vec, length);

    case BRIDGE_FILTERTYPE_LASTPRESSEDPHYSICALKEY_NOT:
    case BRIDGE_FILTERTYPE_LASTPRESSEDPHYSICALKEY_ONLY:
      return new LastPressedPhysicalKeyFilter(type, vec, length);

    case BRIDGE_FILTERTYPE_LASTRELEASEDPHYSICALKEY_NOT:
    case BRIDGE_FILTERTYPE_LASTRELEASEDPHYSICALKEY_ONLY:
      return new LastReleasedPhysicalKeyFilter(type, vec, length);

    case BRIDGE_FILTERTYPE_LASTSENTEVENT_NOT:
    case BRIDGE_FILTERTYPE_LASTSENTEVENT_ONLY:
      return new LastSentEventFilter(type, vec, length);

    case BRIDGE_FILTERTYPE_MODIFIER_NOT:
    case BRIDGE_FILTERTYPE_MODIFIER_ONLY:
    case BRIDGE_FILTERTYPE_MODIFIER_LOCKED_NOT:
    case BRIDGE_FILTERTYPE_MODIFIER_LOCKED_ONLY:
    case BRIDGE_FILTERTYPE_MODIFIER_STUCK_NOT:
    case BRIDGE_FILTERTYPE_MODIFIER_STUCK_ONLY:
      return new ModifierFilter(type, vec, length);

    case BRIDGE_FILTERTYPE_PRESSINGPHYSICALKEYS_GREATERTHAN:
    case BRIDGE_FILTERTYPE_PRESSINGPHYSICALKEYS_LESSTHAN:
      return new PressingPhysicalKeysFilter(type, vec, length);

    case BRIDGE_FILTERTYPE_WINDOWNAME_NOT:
    case BRIDGE_FILTERTYPE_WINDOWNAME_ONLY:
      return new WindowNameFilter(type, vec, length);

    case BRIDGE_FILTERTYPE_UIELEMENTROLE_NOT:
    case BRIDGE_FILTERTYPE_UIELEMENTROLE_ONLY:
      return new UIElementRoleFilter(type, vec, length);
    }

    IOLOG_ERROR("RemapFilterFactory::create unknown type:%d.\n", type);
    return nullptr;
  }
};
}
}

#endif
