#ifndef CONFIG_HPP
#define CONFIG_HPP

#include "../../../src/core/bridge/bridge.h"

class Config {
public:
  static int get_essential_config(unsigned int index) { return 0; }
};
extern Config config;

#endif
