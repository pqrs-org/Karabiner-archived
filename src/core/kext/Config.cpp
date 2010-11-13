#include <sys/types.h>
#include <sys/sysctl.h>

#include "Config.hpp"
#include "version.hpp"
#include "Client.hpp"
#include "FlagStatus.hpp"
#include "RemapClass.hpp"
#include "RemapFunc/PointingRelativeToScroll.hpp"

namespace org_pqrs_KeyRemap4MacBook {
  int Config::debug = 0;
  int Config::debug_pointing = 0;
  int Config::debug_devel = 0;
  int Config::initialized = 0;
  int Config::reload_xml = 0;
  char Config::socket_path[SOCKET_PATH_MAX];

  int Config::essential_config[BRIDGE_ESSENTIAL_CONFIG_INDEX__END__] = {
#include "../bridge/config/output/include.bridge_essential_config_index.cpp"
  };
  int Config::enabled_flags[
#include "config/output/include.config.hpp"
  ];

  namespace {
    void do_reload_xml(void)
    {
      KeyRemap4MacBook_bridge::GetConfigCount::Reply reply;
      int error = KeyRemap4MacBook_client::sendmsg(KeyRemap4MacBook_bridge::REQUEST_GET_CONFIG_COUNT, NULL, 0, &reply, sizeof(reply));
      if (! error) {
        IOLOG_INFO("count = %d\n", reply.count);
      }
    }

    int reload_xml_handler SYSCTL_HANDLER_ARGS
    {
      int error = sysctl_handle_int(oidp, oidp->oid_arg1, oidp->oid_arg2, req);
      if (! error && req->newptr) {
        if (Config::reload_xml) {
          do_reload_xml();
          Config::reload_xml = 0;
        }
      }
      return error;
    }

    int socket_path_handler SYSCTL_HANDLER_ARGS
    {
      int error = sysctl_handle_string(oidp, oidp->oid_arg1, oidp->oid_arg2, req);
      if (! error && req->newptr) {
        KeyRemap4MacBook_client::refreshSockAddr();
      }
      return error;
    }

    int refresh_remapfunc_handler SYSCTL_HANDLER_ARGS
    {
      int error = sysctl_handle_int(oidp, oidp->oid_arg1, oidp->oid_arg2, req);
      if (! error && req->newptr) {
        FlagStatus::lock_clear();
        FlagStatus::sticky_clear();
        RemapClassManager::refresh();
        RemapFunc::PointingRelativeToScroll::cancelScroll();

        // StatusMessageWindowParameter
        {
          static int last_parameter_statuswindow_alpha_font = -1;
          static int last_parameter_statuswindow_alpha_background = -1;
          static int last_parameter_statuswindow_posx_adjustment = 0;
          static int last_parameter_statuswindow_posy_adjustment = 0;

          int alpha_font       = Config::get_essential_config(BRIDGE_ESSENTIAL_CONFIG_INDEX_parameter_statuswindow_alpha_font);
          int alpha_background = Config::get_essential_config(BRIDGE_ESSENTIAL_CONFIG_INDEX_parameter_statuswindow_alpha_background);
          int posx_adjustment  = Config::get_essential_config(BRIDGE_ESSENTIAL_CONFIG_INDEX_parameter_statuswindow_posx_adjustment);
          int posy_adjustment  = Config::get_essential_config(BRIDGE_ESSENTIAL_CONFIG_INDEX_parameter_statuswindow_posy_adjustment);

          if (last_parameter_statuswindow_alpha_font       != alpha_font ||
              last_parameter_statuswindow_alpha_background != alpha_background ||
              last_parameter_statuswindow_posx_adjustment  != posx_adjustment ||
              last_parameter_statuswindow_posy_adjustment  != posy_adjustment) {
            last_parameter_statuswindow_alpha_font       = alpha_font;
            last_parameter_statuswindow_alpha_background = alpha_background;
            last_parameter_statuswindow_posx_adjustment  = posx_adjustment;
            last_parameter_statuswindow_posy_adjustment  = posy_adjustment;

            KeyRemap4MacBook_bridge::StatusMessageWindowParameter::Request request(alpha_font,
                                                                                   alpha_background,
                                                                                   posx_adjustment,
                                                                                   posy_adjustment);
            KeyRemap4MacBook_client::sendmsg(KeyRemap4MacBook_bridge::REQUEST_STATUS_MESSAGE_WINDOW_PARAMETER, &request, sizeof(request), NULL, 0);
          }
        }
      }
      return 0;
    }
  }

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

  SYSCTL_DECL(_keyremap4macbook_notsave);
  SYSCTL_NODE(_keyremap4macbook, OID_AUTO, notsave, CTLFLAG_RW, 0, "");

  SYSCTL_DECL(_keyremap4macbook_repeat);
  SYSCTL_NODE(_keyremap4macbook, OID_AUTO, repeat, CTLFLAG_RW, 0, "");

  SYSCTL_DECL(_keyremap4macbook_pointing);
  SYSCTL_NODE(_keyremap4macbook, OID_AUTO, pointing, CTLFLAG_RW, 0, "");

  SYSCTL_DECL(_keyremap4macbook_parameter);
  SYSCTL_NODE(_keyremap4macbook, OID_AUTO, parameter, CTLFLAG_RW, 0, "");

  SYSCTL_DECL(_keyremap4macbook_passthrough);
  SYSCTL_NODE(_keyremap4macbook, OID_AUTO, passthrough, CTLFLAG_RW, 0, "");

#include "config/output/include.config_SYSCTL.cpp"

  // ----------------------------------------
  SYSCTL_PROC(_keyremap4macbook, OID_AUTO, socket_path, CTLTYPE_STRING | CTLFLAG_RW, Config::socket_path, sizeof(Config::socket_path), socket_path_handler, "A", "");
  SYSCTL_INT(_keyremap4macbook, OID_AUTO, debug, CTLTYPE_INT | CTLFLAG_RW, &(Config::debug), 0, "");
  SYSCTL_INT(_keyremap4macbook, OID_AUTO, debug_pointing, CTLTYPE_INT | CTLFLAG_RW, &(Config::debug_pointing), 0, "");
  SYSCTL_INT(_keyremap4macbook, OID_AUTO, debug_devel, CTLTYPE_INT | CTLFLAG_RW, &(Config::debug_devel), 0, "");

  SYSCTL_STRING(_keyremap4macbook, OID_AUTO, version, CTLFLAG_RD, config_version, 0, "");
  SYSCTL_INT(_keyremap4macbook, OID_AUTO, initialized, CTLTYPE_INT | CTLFLAG_RW, &(Config::initialized), 0, "");
  SYSCTL_PROC(_keyremap4macbook, OID_AUTO, reload_xml, CTLTYPE_INT | CTLFLAG_RW, &(Config::reload_xml), 0, reload_xml_handler, "I", "");

  // ----------------------------------------------------------------------
  void
  Config::initialize(void)
  {
    socket_path[0] = '\0';
  }

  void
  Config::terminate(void)
  {}

  void
  Config::sysctl_register(void)
  {
    sysctl_register_oid(&sysctl__keyremap4macbook);
    sysctl_register_oid(&sysctl__keyremap4macbook_general);
    sysctl_register_oid(&sysctl__keyremap4macbook_remap);
    sysctl_register_oid(&sysctl__keyremap4macbook_option);
    sysctl_register_oid(&sysctl__keyremap4macbook_notsave);
    sysctl_register_oid(&sysctl__keyremap4macbook_repeat);
    sysctl_register_oid(&sysctl__keyremap4macbook_pointing);
    sysctl_register_oid(&sysctl__keyremap4macbook_parameter);
    sysctl_register_oid(&sysctl__keyremap4macbook_passthrough);

    // ----------------------------------------
#include "config/output/include.config_register.cpp"

    // ----------------------------------------
    sysctl_register_oid(&sysctl__keyremap4macbook_socket_path);
    sysctl_register_oid(&sysctl__keyremap4macbook_debug);
    sysctl_register_oid(&sysctl__keyremap4macbook_debug_pointing);
    sysctl_register_oid(&sysctl__keyremap4macbook_debug_devel);
    sysctl_register_oid(&sysctl__keyremap4macbook_version);
    sysctl_register_oid(&sysctl__keyremap4macbook_initialized);
    sysctl_register_oid(&sysctl__keyremap4macbook_reload_xml);
  }

  void
  Config::sysctl_unregister(void)
  {
    sysctl_unregister_oid(&sysctl__keyremap4macbook);
    sysctl_unregister_oid(&sysctl__keyremap4macbook_general);
    sysctl_unregister_oid(&sysctl__keyremap4macbook_remap);
    sysctl_unregister_oid(&sysctl__keyremap4macbook_option);
    sysctl_unregister_oid(&sysctl__keyremap4macbook_notsave);
    sysctl_unregister_oid(&sysctl__keyremap4macbook_repeat);
    sysctl_unregister_oid(&sysctl__keyremap4macbook_pointing);
    sysctl_unregister_oid(&sysctl__keyremap4macbook_parameter);
    sysctl_unregister_oid(&sysctl__keyremap4macbook_passthrough);

    // ----------------------------------------
#include "config/output/include.config_unregister.cpp"

    // ----------------------------------------
    sysctl_unregister_oid(&sysctl__keyremap4macbook_socket_path);
    sysctl_unregister_oid(&sysctl__keyremap4macbook_debug);
    sysctl_unregister_oid(&sysctl__keyremap4macbook_debug_pointing);
    sysctl_unregister_oid(&sysctl__keyremap4macbook_debug_devel);
    sysctl_unregister_oid(&sysctl__keyremap4macbook_version);
    sysctl_unregister_oid(&sysctl__keyremap4macbook_initialized);
    sysctl_unregister_oid(&sysctl__keyremap4macbook_reload_xml);
  }
}
