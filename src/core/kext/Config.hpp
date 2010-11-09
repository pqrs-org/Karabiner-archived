#ifndef CONFIG_HPP
#define CONFIG_HPP

#include "bridge.h"

namespace org_pqrs_KeyRemap4MacBook {
  class Config {
  public:
    Config() : debug(0), debug_pointing(0), debug_devel(0), initialized(0) {}

#include "config/output/include.config.hpp"

    int get_essential_config(unsigned int index) const {
      if (index >= BRIDGE_ESSENTIAL_CONFIG_INDEX__END__) {
        return 0;
      }
      return essential_config[index];
    }

    unsigned int get_repeat_initial_wait(void) const {
      int v = get_essential_config(BRIDGE_ESSENTIAL_CONFIG_INDEX_repeat_initial_wait);
      return getvalue(v, 200);
    }
    unsigned int get_repeat_wait(void) const {
      int v = get_essential_config(BRIDGE_ESSENTIAL_CONFIG_INDEX_repeat_wait);
      return getvalue(v, 5);
    }
    unsigned int get_repeat_consumer_initial_wait(void) const {
      int v = get_essential_config(BRIDGE_ESSENTIAL_CONFIG_INDEX_repeat_consumer_initial_wait);
      return getvalue(v, 10);
    }
    unsigned int get_repeat_consumer_wait(void) const {
      int v = get_essential_config(BRIDGE_ESSENTIAL_CONFIG_INDEX_repeat_consumer_wait);
      return getvalue(v, 10);
    }
    unsigned int get_keyoverlaidmodifier_initial_wait(void) const {
      int v = get_essential_config(BRIDGE_ESSENTIAL_CONFIG_INDEX_repeat_keyoverlaidmodifier_initial_wait);
      return getvalue(v, 200);
    }
    unsigned int get_simultaneouskeypresses_delay(void) const {
      int v = get_essential_config(BRIDGE_ESSENTIAL_CONFIG_INDEX_parameter_simultaneouskeypresses_delay);
      return getvalue(v, 5, 1000);
    }
    unsigned int get_simultaneouskeypresses_pointingbutton_delay(void) const {
      int v = get_essential_config(BRIDGE_ESSENTIAL_CONFIG_INDEX_parameter_simultaneouskeypresses_pointingbutton_delay);
      return getvalue(v, 5, 1000);
    }
    unsigned int get_holdingkeytokey_wait(void) const {
      int v = get_essential_config(BRIDGE_ESSENTIAL_CONFIG_INDEX_parameter_holdingkeytokey_wait);
      return getvalue(v, 10);
    }

    // ----------------------------------------
    int debug;
    int debug_pointing;
    int debug_devel;
    int initialized;

    enum { SOCKET_PATH_MAX = 256 };
    char socket_path[SOCKET_PATH_MAX];

    static int essential_config[BRIDGE_ESSENTIAL_CONFIG_INDEX__END__];

  private:
    unsigned int getvalue(int value, int minval) const {
      if (value < minval) return minval;
      return value;
    }
    unsigned int getvalue(int value, int minval, int maxval) const {
      if (value < minval) return minval;
      if (value > maxval) return maxval;
      return value;
    }
  };
  extern Config config;

  void sysctl_register(void);
  void sysctl_unregister(void);
}

#endif
