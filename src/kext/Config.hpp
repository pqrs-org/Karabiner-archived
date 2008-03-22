#ifndef CONFIG_HPP
#define CONFIG_HPP

namespace org_pqrs_KeyRemap4MacBook {
  class Config {
  public:
    Config() {
#include "config/output/include.config.default.hpp"
    }

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
