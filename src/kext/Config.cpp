#include <sys/types.h>
#include <sys/sysctl.h>

#include "Config.hpp"
#include "version.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  Config config;

  // ----------------------------------------------------------------------
  // SYSCTL staff
  // http://developer.apple.com/documentation/Darwin/Conceptual/KernelProgramming/boundaries/chapter_14_section_7.html#//apple_ref/doc/uid/TP30000905-CH217-TPXREF116
  SYSCTL_DECL(_keyremap4macbook);
  SYSCTL_NODE(, OID_AUTO, keyremap4macbook, CTLFLAG_RW, 0, "KeyRemap4MacBook");

  SYSCTL_DECL(_keyremap4macbook_general);
  SYSCTL_NODE(_keyremap4macbook, OID_AUTO, general, CTLFLAG_RW, 0, "general");

  SYSCTL_DECL(_keyremap4macbook_remap);
  SYSCTL_NODE(_keyremap4macbook, OID_AUTO, remap, CTLFLAG_RW, 0, "remap");

  SYSCTL_DECL(_keyremap4macbook_option);
  SYSCTL_NODE(_keyremap4macbook, OID_AUTO, option, CTLFLAG_RW, 0, "option");

  SYSCTL_DECL(_keyremap4macbook_repeat);
  SYSCTL_NODE(_keyremap4macbook, OID_AUTO, repeat, CTLFLAG_RW, 0, "repeat");

  // ----------------------------------------
  SYSCTL_INT(_keyremap4macbook_repeat, OID_AUTO, initial_wait, CTLTYPE_INT|CTLFLAG_RW,
             &(config.repeat_initial_wait), 0, "");

  SYSCTL_INT(_keyremap4macbook_repeat, OID_AUTO, wait, CTLTYPE_INT|CTLFLAG_RW,
             &(config.repeat_wait), 0, "");

    // ----------------------------------------
#include "config/output/include.config_SYSCTL.cpp"

  // ----------------------------------------
  SYSCTL_INT(_keyremap4macbook, OID_AUTO, debug, CTLTYPE_INT|CTLFLAG_RW,
             &(config.debug), 0, "");

  SYSCTL_STRING(_keyremap4macbook, OID_AUTO, version, CTLFLAG_RD,
                config_version, 0, "");

  // ----------------------------------------------------------------------
  void
  sysctl_register(void)
  {
    sysctl_register_oid(&sysctl__keyremap4macbook);
    sysctl_register_oid(&sysctl__keyremap4macbook_general);
    sysctl_register_oid(&sysctl__keyremap4macbook_remap);
    sysctl_register_oid(&sysctl__keyremap4macbook_option);
    sysctl_register_oid(&sysctl__keyremap4macbook_repeat);

    // ----------------------------------------
    sysctl_register_oid(&sysctl__keyremap4macbook_repeat_initial_wait);
    sysctl_register_oid(&sysctl__keyremap4macbook_repeat_wait);

    // ----------------------------------------
#include "config/output/include.config_register.cpp"

    // ----------------------------------------
    sysctl_register_oid(&sysctl__keyremap4macbook_debug);
    sysctl_register_oid(&sysctl__keyremap4macbook_version);
  }

  void
  sysctl_unregister(void)
  {
    sysctl_unregister_oid(&sysctl__keyremap4macbook);
    sysctl_unregister_oid(&sysctl__keyremap4macbook_general);
    sysctl_unregister_oid(&sysctl__keyremap4macbook_remap);
    sysctl_unregister_oid(&sysctl__keyremap4macbook_option);
    sysctl_unregister_oid(&sysctl__keyremap4macbook_repeat);

    // ----------------------------------------
    sysctl_unregister_oid(&sysctl__keyremap4macbook_repeat_initial_wait);
    sysctl_unregister_oid(&sysctl__keyremap4macbook_repeat_wait);

    // ----------------------------------------
#include "config/output/include.config_unregister.cpp"

    // ----------------------------------------
    sysctl_unregister_oid(&sysctl__keyremap4macbook_debug);
    sysctl_unregister_oid(&sysctl__keyremap4macbook_version);
  }
}
