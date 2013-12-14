#ifndef CONFIG_HPP
#define CONFIG_HPP

#include "../../../src/bridge/include/bridge.h"
#include "../../../src/bridge/output/include.bridge_essential_configuration_enum.h"

class Config {
public:
  static int get_essential_config(unsigned int index) { return 0; }
};
extern Config config;

#endif
