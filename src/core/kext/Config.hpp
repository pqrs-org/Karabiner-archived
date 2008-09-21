#ifndef CONFIG_HPP
#define CONFIG_HPP

namespace org_pqrs_KeyRemap4MacBook {
  class Config {
  public:
    Config() {
#include "config/output/include.config.default.hpp"
    }

#include "config/output/include.config.hpp"

    int get_repeat_initial_wait(void) {
      const int MINVAL = 200;
      int value = repeat_initial_wait;
      if (value < MINVAL) return MINVAL;
      return value;
    }
    int get_repeat_wait(void) {
      const int MINVAL = 5;
      int value = repeat_wait;
      if (value < MINVAL) return MINVAL;
      return value;
    }
    int get_keyoverlaidmodifier_initial_wait(void) {
      const int MINVAL = 200;
      int value = repeat_keyoverlaidmodifier_initial_wait;
      if (value < MINVAL) return MINVAL;
      return value;
    }

    // ----------------------------------------
    int debug;
    int debug_pointing;
    int debug_devel;
  };
  extern Config config;

  void sysctl_register(void);
  void sysctl_unregister(void);
}

#endif
