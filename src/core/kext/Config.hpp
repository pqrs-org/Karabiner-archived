#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <sys/types.h>
#include <sys/sysctl.h>
#include "bridge.h"

namespace org_pqrs_KeyRemap4MacBook {
  class Config {
  public:
    static void initialize(void);
    static void terminate(void);

    static void sysctl_register(void);
    static void sysctl_unregister(void);

    static int do_reset_handler SYSCTL_HANDLER_ARGS;
    static int do_reload_xml_handler SYSCTL_HANDLER_ARGS;
    static int do_reload_only_config_handler SYSCTL_HANDLER_ARGS;
    static int socket_path_handler SYSCTL_HANDLER_ARGS;

    static int get_essential_config(unsigned int index) {
      if (index >= BRIDGE_ESSENTIAL_CONFIG_INDEX__END__) {
        return 0;
      }
      return essential_config_[index];
    }

    static unsigned int get_repeat_initial_wait(void) {
      int v = get_essential_config(BRIDGE_ESSENTIAL_CONFIG_INDEX_repeat_initial_wait);
      return getvalue(v, 200);
    }
    static unsigned int get_repeat_wait(void) {
      int v = get_essential_config(BRIDGE_ESSENTIAL_CONFIG_INDEX_repeat_wait);
      return getvalue(v, 5);
    }
    static unsigned int get_repeat_consumer_initial_wait(void) {
      int v = get_essential_config(BRIDGE_ESSENTIAL_CONFIG_INDEX_repeat_consumer_initial_wait);
      return getvalue(v, 10);
    }
    static unsigned int get_repeat_consumer_wait(void) {
      int v = get_essential_config(BRIDGE_ESSENTIAL_CONFIG_INDEX_repeat_consumer_wait);
      return getvalue(v, 10);
    }
    static unsigned int get_keyoverlaidmodifier_initial_wait(void) {
      int v = get_essential_config(BRIDGE_ESSENTIAL_CONFIG_INDEX_repeat_keyoverlaidmodifier_initial_wait);
      return getvalue(v, 200);
    }
    static unsigned int get_simultaneouskeypresses_delay(void) {
      int v = get_essential_config(BRIDGE_ESSENTIAL_CONFIG_INDEX_parameter_simultaneouskeypresses_delay);
      return getvalue(v, 5, 1000);
    }
    static unsigned int get_simultaneouskeypresses_pointingbutton_delay(void) {
      int v = get_essential_config(BRIDGE_ESSENTIAL_CONFIG_INDEX_parameter_simultaneouskeypresses_pointingbutton_delay);
      return getvalue(v, 5, 1000);
    }
    static unsigned int get_holdingkeytokey_wait(void) {
      int v = get_essential_config(BRIDGE_ESSENTIAL_CONFIG_INDEX_parameter_holdingkeytokey_wait);
      return getvalue(v, 10);
    }

    // ----------------------------------------
    static int debug;
    static int debug_pointing;
    static int debug_devel;
    static int initialized;
    static int do_reset;
    static int do_reload_xml;
    static int do_reload_only_config;

    enum { SOCKET_PATH_MAX = 256 };
    static char socket_path[SOCKET_PATH_MAX];

  private:
    static unsigned int getvalue(int value, int minval) {
      if (value < minval) return minval;
      return value;
    }
    static unsigned int getvalue(int value, int minval, int maxval) {
      if (value < minval) return minval;
      if (value > maxval) return maxval;
      return value;
    }

    static void load_essential_config_default(void);
    static void load_essential_config(void);

    static int essential_config_[BRIDGE_ESSENTIAL_CONFIG_INDEX__END__];
    static const int essential_config_default_[BRIDGE_ESSENTIAL_CONFIG_INDEX__END__];
  };
}

#endif
