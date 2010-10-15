#ifndef CONFIG_HPP
#define CONFIG_HPP

namespace org_pqrs_KeyRemap4MacBook {
  class Config {
  public:
    Config() : debug(0), debug_pointing(0), debug_devel(0), initialized(0) {
#include "config/output/include.config.default.hpp"
    }

#include "config/output/include.config.hpp"

    unsigned int get_repeat_initial_wait(void) const {
      return getvalue(repeat_initial_wait, 200);
    }
    unsigned int get_repeat_wait(void) const {
      return getvalue(repeat_wait, 5);
    }
    unsigned int get_repeat_consumer_initial_wait(void) const {
      return getvalue(repeat_consumer_initial_wait, 10);
    }
    unsigned int get_repeat_consumer_wait(void) const {
      return getvalue(repeat_consumer_wait, 10);
    }
    unsigned int get_keyoverlaidmodifier_initial_wait(void) const {
      return getvalue(repeat_keyoverlaidmodifier_initial_wait, 200);
    }
    unsigned int get_simultaneouskeypresses_delay(void) const {
      return getvalue(parameter_simultaneouskeypresses_delay, 5, 1000);
    }
    unsigned int get_simultaneouskeypresses_pointingbutton_delay(void) const {
      return getvalue(parameter_simultaneouskeypresses_pointingbutton_delay, 5, 1000);
    }
    unsigned int get_holdingkeytokey_wait(void) const {
      return getvalue(parameter_holdingkeytokey_wait, 10);
    }

    // ----------------------------------------
    int debug;
    int debug_pointing;
    int debug_devel;
    int initialized;

    enum { SOCKET_PATH_MAX = 256 };
    char socket_path[SOCKET_PATH_MAX];

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
