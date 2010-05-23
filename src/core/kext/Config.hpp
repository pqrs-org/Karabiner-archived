#ifndef CONFIG_HPP
#define CONFIG_HPP

namespace org_pqrs_KeyRemap4MacBook {
  class Config {
  public:
    Config() : debug(0), debug_pointing(0), debug_devel(0), initialized(0) {
#include "config/output/include.config.default.hpp"
    }

#include "config/output/include.config.hpp"

    // Don't put notsave_pointing_relative_to_scroll in checkbox.xml.
    // Because it is dangerous that a user changes setting.
    int notsave_pointing_relative_to_scroll;

    int get_repeat_initial_wait(void) const {
      const int MINVAL = 200;
      int value = repeat_initial_wait;
      if (value < MINVAL) return MINVAL;
      return value;
    }
    int get_repeat_wait(void) const {
      const int MINVAL = 5;
      int value = repeat_wait;
      if (value < MINVAL) return MINVAL;
      return value;
    }
    int get_repeat_consumer_initial_wait(void) const {
      const int MINVAL = 10;
      int value = repeat_consumer_initial_wait;
      if (value < MINVAL) return MINVAL;
      return value;
    }
    int get_repeat_consumer_wait(void) const {
      const int MINVAL = 10;
      int value = repeat_consumer_wait;
      if (value < MINVAL) return MINVAL;
      return value;
    }
    int get_keyoverlaidmodifier_initial_wait(void) {
      return getvalue(repeat_keyoverlaidmodifier_initial_wait, 200);
    }
    int get_simultaneouskeypresses_delay(void) {
      return getvalue(parameter_simultaneouskeypresses_delay, 5, 1000);
    }

    // ----------------------------------------
    int debug;
    int debug_pointing;
    int debug_devel;
    int initialized;

    enum { SOCKET_PATH_MAX = 256 };
    char socket_path[SOCKET_PATH_MAX];

  private:
    unsigned int getvalue(int value, int minval) {
      if (value < minval) return minval;
      return value;
    }
    unsigned int getvalue(int value, int minval, int maxval) {
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
