#ifndef CONFIG_HPP
#define CONFIG_HPP

class Config {
public:
  Config(void) : general_disable_numpad_hack(0) {}
  int general_disable_numpad_hack;
};
extern Config config;

#endif
