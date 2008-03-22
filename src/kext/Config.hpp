#ifndef CONFIG_HPP
#define CONFIG_HPP

namespace org_pqrs_KeyRemap4MacBook {
  class Config {
  public:
    Config() {
      repeat_initial_wait = 500;
      repeat_wait = 30;
      pointing_relative2scroll_threshold = 16;
    }

    // ----------------------------------------
    int repeat_initial_wait;
    int repeat_wait;

    int pointing_relative2scroll_threshold;

#include "config/output/include.config.hpp"

    // ----------------------------------------
    int debug;
    int debug_pointing;
  };
  extern Config config;

  void sysctl_register(void);
  void sysctl_unregister(void);
}

#endif
