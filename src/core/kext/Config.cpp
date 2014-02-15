#include <IOKit/IOLib.h>

#include "ButtonStatus.hpp"
#include "CommonData.hpp"
#include "Config.hpp"
#include "FlagStatus.hpp"
#include "IOLogWrapper.hpp"
#include "KeyboardRepeat.hpp"
#include "ListHookedConsumer.hpp"
#include "ListHookedKeyboard.hpp"
#include "ListHookedPointing.hpp"
#include "RemapClass.hpp"
#include "RemapFunc/PointingRelativeToScroll.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  namespace {
    int sysctl_debug          = 0;
    int sysctl_debug_pointing = 0;
    int sysctl_debug_devel    = 0;
    int sysctl_initialized    = 0;
  }

  int Config::essential_config_[BRIDGE_ESSENTIAL_CONFIG_INDEX__END__] = {
#include "../../bridge/output/include.bridge_essential_configuration_default_values.c"
  };

  const int Config::essential_config_default_[BRIDGE_ESSENTIAL_CONFIG_INDEX__END__] = {
#include "../../bridge/output/include.bridge_essential_configuration_default_values.c"
  };

  // ----------------------------------------------------------------------
  // SYSCTL staff
  // http://developer.apple.com/documentation/Darwin/Conceptual/KernelProgramming/boundaries/chapter_14_section_7.html#//apple_ref/doc/uid/TP30000905-CH217-TPXREF116
  SYSCTL_DECL(_keyremap4macbook);
  SYSCTL_NODE(, OID_AUTO, keyremap4macbook, CTLFLAG_RW, 0, "");

  // ----------------------------------------
  SYSCTL_INT(_keyremap4macbook,    OID_AUTO, initialized,    CTLTYPE_INT,              &(sysctl_initialized),    0, "");
  SYSCTL_INT(_keyremap4macbook,    OID_AUTO, debug,          CTLTYPE_INT | CTLFLAG_RW, &(sysctl_debug),          0, "");
  SYSCTL_INT(_keyremap4macbook,    OID_AUTO, debug_pointing, CTLTYPE_INT | CTLFLAG_RW, &(sysctl_debug_pointing), 0, "");
  SYSCTL_INT(_keyremap4macbook,    OID_AUTO, debug_devel,    CTLTYPE_INT | CTLFLAG_RW, &(sysctl_debug_devel),    0, "");

  // ----------------------------------------------------------------------
  void
  Config::sysctl_register(void)
  {
    sysctl_register_oid(&sysctl__keyremap4macbook);

    sysctl_register_oid(&sysctl__keyremap4macbook_initialized);
    sysctl_register_oid(&sysctl__keyremap4macbook_debug);
    sysctl_register_oid(&sysctl__keyremap4macbook_debug_pointing);
    sysctl_register_oid(&sysctl__keyremap4macbook_debug_devel);
  }

  void
  Config::sysctl_unregister(void)
  {
    sysctl_unregister_oid(&sysctl__keyremap4macbook);

    sysctl_unregister_oid(&sysctl__keyremap4macbook_initialized);
    sysctl_unregister_oid(&sysctl__keyremap4macbook_debug);
    sysctl_unregister_oid(&sysctl__keyremap4macbook_debug_pointing);
    sysctl_unregister_oid(&sysctl__keyremap4macbook_debug_devel);
  }

  void
  Config::set_initialized(bool newvalue)
  {
    sysctl_initialized = newvalue;

    // ----------------------------------------
    // reset values
    org_pqrs_KeyRemap4MacBook::FlagStatus::lock_clear();
    org_pqrs_KeyRemap4MacBook::FlagStatus::sticky_clear();
    org_pqrs_KeyRemap4MacBook::ButtonStatus::lock_clear();

    // cancel timers
    org_pqrs_KeyRemap4MacBook::KeyboardRepeat::cancel();
    org_pqrs_KeyRemap4MacBook::RemapFunc::PointingRelativeToScroll::cancelScroll();

    // refresh devices
    ListHookedDevice::refreshAll();

    // ----------------------------------------
    // reset sysctl_debug* and essential_config_.
    if (newvalue == false) {
      sysctl_debug          = 0;
      sysctl_debug_devel    = 0;
      sysctl_debug_pointing = 0;

      load_essential_config_default();
    }
  }

  bool Config::get_initialized(void)    { return sysctl_initialized;    }
  bool Config::get_debug(void)          { return sysctl_debug;          }
  bool Config::get_debug_devel(void)    { return sysctl_debug_devel;    }
  bool Config::get_debug_pointing(void) { return sysctl_debug_pointing; }

  void
  Config::load_essential_config_default(void)
  {
    for (int i = 0; i < BRIDGE_ESSENTIAL_CONFIG_INDEX__END__; ++i) {
      essential_config_[i] = essential_config_default_[i];
    }
  }

  void
  Config::set_essential_config(const int32_t* newvalues, size_t num)
  {
    if (num != BRIDGE_ESSENTIAL_CONFIG_INDEX__END__) {
      IOLOG_ERROR("Config::set_essential_config wrong 'num' parameter. (%d)\n", static_cast<int>(num));
      load_essential_config_default();

    } else {
      for (int i = 0; i < BRIDGE_ESSENTIAL_CONFIG_INDEX__END__; ++i) {
        essential_config_[i] = newvalues[i];
      }
    }
  }

  bool
  Config::set_essential_config_one(uint32_t index, int32_t value)
  {
    if (index >= BRIDGE_ESSENTIAL_CONFIG_INDEX__END__) {
      IOLOG_ERROR("Config::set_essential_config_one wrong 'index' parameter. (%d)\n", static_cast<int>(index));
      return false;
    }

    essential_config_[index] = value;
    return true;
  }
}
