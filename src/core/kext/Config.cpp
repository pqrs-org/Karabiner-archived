#include "Config.hpp"
#include "version.hpp"
#include "FlagStatus.hpp"
#include "RemapClass.hpp"
#include "RemapFunc/PointingRelativeToScroll.hpp"
#include "util/CommonData.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  int Config::debug = 0;
  int Config::debug_pointing = 0;
  int Config::debug_devel = 0;
  int Config::initialized = 0;

  int Config::essential_config_[BRIDGE_ESSENTIAL_CONFIG_INDEX__END__] = {
#include "../bridge/config/output/include.bridge_essential_config_index.cpp"
  };

  const int Config::essential_config_default_[BRIDGE_ESSENTIAL_CONFIG_INDEX__END__] = {
#include "../bridge/config/output/include.bridge_essential_config_index.cpp"
  };

  // ----------------------------------------------------------------------
  // SYSCTL staff
  // http://developer.apple.com/documentation/Darwin/Conceptual/KernelProgramming/boundaries/chapter_14_section_7.html#//apple_ref/doc/uid/TP30000905-CH217-TPXREF116
  SYSCTL_DECL(_keyremap4macbook);
  SYSCTL_NODE(, OID_AUTO, keyremap4macbook, CTLFLAG_RW, 0, "");

  // ----------------------------------------
  SYSCTL_INT(_keyremap4macbook,    OID_AUTO, debug,          CTLTYPE_INT | CTLFLAG_RW, &(Config::debug),          0, "");
  SYSCTL_INT(_keyremap4macbook,    OID_AUTO, debug_pointing, CTLTYPE_INT | CTLFLAG_RW, &(Config::debug_pointing), 0, "");
  SYSCTL_INT(_keyremap4macbook,    OID_AUTO, debug_devel,    CTLTYPE_INT | CTLFLAG_RW, &(Config::debug_devel),    0, "");
  SYSCTL_STRING(_keyremap4macbook, OID_AUTO, version,        CTLFLAG_RD,               config_version,            0, "");

  // ----------------------------------------------------------------------
  void
  Config::initialize(void)
  {}

  void
  Config::terminate(void)
  {}

  void
  Config::sysctl_register(void)
  {
    sysctl_register_oid(&sysctl__keyremap4macbook);

    sysctl_register_oid(&sysctl__keyremap4macbook_debug);
    sysctl_register_oid(&sysctl__keyremap4macbook_debug_pointing);
    sysctl_register_oid(&sysctl__keyremap4macbook_debug_devel);
    sysctl_register_oid(&sysctl__keyremap4macbook_version);
  }

  void
  Config::sysctl_unregister(void)
  {
    sysctl_unregister_oid(&sysctl__keyremap4macbook);

    sysctl_unregister_oid(&sysctl__keyremap4macbook_debug);
    sysctl_unregister_oid(&sysctl__keyremap4macbook_debug_pointing);
    sysctl_unregister_oid(&sysctl__keyremap4macbook_debug_devel);
    sysctl_unregister_oid(&sysctl__keyremap4macbook_version);
  }

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
}
