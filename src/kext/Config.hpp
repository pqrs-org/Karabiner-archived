#ifndef CONFIG_HPP
#define CONFIG_HPP

namespace org_pqrs_KeyRemap4MacBook {
  class Config {
  public:
    Config() {
      repeat_initial_wait = 500;
      repeat_wait = 30;
    }

    // ----------------------------------------
    int repeat_initial_wait;
    int repeat_wait;

#include "config/output/include.config.hpp"

    // ----------------------------------------
    int debug;
  };
  extern Config config;

  void sysctl_register(void);
  void sysctl_unregister(void);
}

#endif
