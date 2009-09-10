#include <sys/types.h>
#include <sys/sysctl.h>

#include "Config.hpp"
#include "version.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  Config config;
  int initialized = 0;

  // ----------------------------------------------------------------------
  // SYSCTL staff
  // http://developer.apple.com/documentation/Darwin/Conceptual/KernelProgramming/boundaries/chapter_14_section_7.html#//apple_ref/doc/uid/TP30000905-CH217-TPXREF116
  SYSCTL_DECL(_keyremap4macbook);
  SYSCTL_NODE(, OID_AUTO, keyremap4macbook, CTLFLAG_RW, 0, "");

  SYSCTL_DECL(_keyremap4macbook_general);
  SYSCTL_NODE(_keyremap4macbook, OID_AUTO, general, CTLFLAG_RW, 0, "");

  SYSCTL_DECL(_keyremap4macbook_remap);
  SYSCTL_NODE(_keyremap4macbook, OID_AUTO, remap, CTLFLAG_RW, 0, "");

  SYSCTL_DECL(_keyremap4macbook_option);
  SYSCTL_NODE(_keyremap4macbook, OID_AUTO, option, CTLFLAG_RW, 0, "");

  SYSCTL_DECL(_keyremap4macbook_repeat);
  SYSCTL_NODE(_keyremap4macbook, OID_AUTO, repeat, CTLFLAG_RW, 0, "");

  SYSCTL_DECL(_keyremap4macbook_pointing);
  SYSCTL_NODE(_keyremap4macbook, OID_AUTO, pointing, CTLFLAG_RW, 0, "");

  SYSCTL_DECL(_keyremap4macbook_parameter);
  SYSCTL_NODE(_keyremap4macbook, OID_AUTO, parameter, CTLFLAG_RW, 0, "");

#include "config/output/include.config_SYSCTL.cpp"

  // ----------------------------------------
  SYSCTL_INT(_keyremap4macbook, OID_AUTO, debug, CTLTYPE_INT|CTLFLAG_RW, &(config.debug), 0, "");
  SYSCTL_INT(_keyremap4macbook, OID_AUTO, debug_pointing, CTLTYPE_INT|CTLFLAG_RW, &(config.debug_pointing), 0, "");
  SYSCTL_INT(_keyremap4macbook, OID_AUTO, debug_devel, CTLTYPE_INT|CTLFLAG_RW, &(config.debug_devel), 0, "");

  SYSCTL_STRING(_keyremap4macbook, OID_AUTO, version, CTLFLAG_RD, config_version, 0, "");
  SYSCTL_INT(_keyremap4macbook, OID_AUTO, initialized, CTLTYPE_INT|CTLFLAG_RW, &(initialized), 0, "");

  // ----------------------------------------------------------------------
  void
  sysctl_register(void)
  {
    sysctl_register_oid(&sysctl__keyremap4macbook);
    sysctl_register_oid(&sysctl__keyremap4macbook_general);
    sysctl_register_oid(&sysctl__keyremap4macbook_remap);
    sysctl_register_oid(&sysctl__keyremap4macbook_option);
    sysctl_register_oid(&sysctl__keyremap4macbook_repeat);
    sysctl_register_oid(&sysctl__keyremap4macbook_pointing);
    sysctl_register_oid(&sysctl__keyremap4macbook_parameter);

    // ----------------------------------------
#include "config/output/include.config_register.cpp"

    // ----------------------------------------
    sysctl_register_oid(&sysctl__keyremap4macbook_debug);
    sysctl_register_oid(&sysctl__keyremap4macbook_debug_pointing);
    sysctl_register_oid(&sysctl__keyremap4macbook_debug_devel);
    sysctl_register_oid(&sysctl__keyremap4macbook_version);
    sysctl_register_oid(&sysctl__keyremap4macbook_initialized);
  }

  void
  sysctl_unregister(void)
  {
    sysctl_unregister_oid(&sysctl__keyremap4macbook);
    sysctl_unregister_oid(&sysctl__keyremap4macbook_general);
    sysctl_unregister_oid(&sysctl__keyremap4macbook_remap);
    sysctl_unregister_oid(&sysctl__keyremap4macbook_option);
    sysctl_unregister_oid(&sysctl__keyremap4macbook_repeat);
    sysctl_unregister_oid(&sysctl__keyremap4macbook_pointing);
    sysctl_unregister_oid(&sysctl__keyremap4macbook_parameter);

    // ----------------------------------------
#include "config/output/include.config_unregister.cpp"

    // ----------------------------------------
    sysctl_unregister_oid(&sysctl__keyremap4macbook_debug);
    sysctl_unregister_oid(&sysctl__keyremap4macbook_debug_pointing);
    sysctl_unregister_oid(&sysctl__keyremap4macbook_debug_devel);
    sysctl_unregister_oid(&sysctl__keyremap4macbook_version);
    sysctl_unregister_oid(&sysctl__keyremap4macbook_initialized);
  }
}
