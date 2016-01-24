#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <sys/types.h>
#include <sys/sysctl.h>
#include "bridge.h"
#include "../../bridge/output/include.bridge_essential_configuration_enum.h"

namespace org_pqrs_Karabiner {
class Config final {
public:
  static void sysctl_register(void);
  static void sysctl_unregister(void);

  // ----------------------------------------
  static void set_initialized(bool newvalue);
  static void unset_debug_flags(void);
  static bool get_initialized(void);
  static bool get_debug(void);
  static bool get_debug_devel(void);
  static bool get_debug_pointing(void);

  // ----------------------------------------
  static void set_essential_config(const int32_t* newvalues, size_t num);
  static bool set_essential_config_one(uint32_t index, int32_t value);

  static int get_essential_config(unsigned int index) {
    if (index >= BRIDGE_ESSENTIAL_CONFIG_INDEX__END__) {
      return 0;
    }
    return essential_config_[index];
  }

  static unsigned int get_repeat_initial_wait(void) {
    int v = get_essential_config(BRIDGE_ESSENTIAL_CONFIG_INDEX_repeat_initial_wait);
    return getvalue(v, 1);
  }
  static unsigned int get_repeat_wait(void) {
    int v = get_essential_config(BRIDGE_ESSENTIAL_CONFIG_INDEX_repeat_wait);
    return getvalue(v, 1);
  }
  static unsigned int get_repeat_consumer_initial_wait(void) {
    int v = get_essential_config(BRIDGE_ESSENTIAL_CONFIG_INDEX_repeat_initial_wait);
    return getvalue(v, 250);
  }
  static unsigned int get_repeat_consumer_wait(void) {
    int v = get_essential_config(BRIDGE_ESSENTIAL_CONFIG_INDEX_repeat_wait);
    return getvalue(v, 33);
  }
  static unsigned int get_wait_before_and_after_a_modifier_key_event(void) {
    int v = get_essential_config(BRIDGE_ESSENTIAL_CONFIG_INDEX_parameter_wait_before_and_after_a_modifier_key_event);
    return getvalue(v, 0, 1000);
  }
  static unsigned int get_wait_before_and_after_a_click_event(void) {
    int v = get_essential_config(BRIDGE_ESSENTIAL_CONFIG_INDEX_parameter_wait_before_and_after_a_click_event);
    return getvalue(v, 0, 1000);
  }
  static unsigned int get_keyoverlaidmodifier_initial_modifier_wait(void) {
    int v = get_essential_config(BRIDGE_ESSENTIAL_CONFIG_INDEX_parameter_keyoverlaidmodifier_initial_modifier_wait);
    return getvalue(v, 0);
  }
  static unsigned int get_keyoverlaidmodifier_initial_wait(void) {
    int v = get_essential_config(BRIDGE_ESSENTIAL_CONFIG_INDEX_repeat_keyoverlaidmodifier_initial_wait);
    return getvalue(v, 200);
  }
  static unsigned int get_keyoverlaidmodifier_timeout(void) {
    int v = get_essential_config(BRIDGE_ESSENTIAL_CONFIG_INDEX_parameter_keyoverlaidmodifier_timeout);
    return getvalue(v, 0);
  }
  static unsigned int get_ignore_bouncing_threshold_for_keyboard(void) {
    int v = get_essential_config(BRIDGE_ESSENTIAL_CONFIG_INDEX_parameter_ignore_bouncing_threshold_for_keyboard);
    return getvalue(v, 0, 1000);
  }
  static unsigned int get_ignore_bouncing_threshold_for_mice(void) {
    int v = get_essential_config(BRIDGE_ESSENTIAL_CONFIG_INDEX_parameter_ignore_bouncing_threshold_for_mice);
    return getvalue(v, 0, 1000);
  }
  static unsigned int get_simultaneouskeypresses_delay(void) {
    int v = get_essential_config(BRIDGE_ESSENTIAL_CONFIG_INDEX_parameter_simultaneouskeypresses_delay);
    return getvalue(v, 5, 1000);
  }
  static unsigned int get_simultaneouskeypresses_pointingbutton_delay(void) {
    int v = get_essential_config(BRIDGE_ESSENTIAL_CONFIG_INDEX_parameter_simultaneouskeypresses_pointingbutton_delay);
    return getvalue(v, 5, 1000);
  }
  static unsigned int get_holdingkeytokey_wait(void) {
    int v = get_essential_config(BRIDGE_ESSENTIAL_CONFIG_INDEX_parameter_holdingkeytokey_wait);
    return getvalue(v, 10);
  }
  static unsigned int get_mousekey_high_speed_of_pointer(void) {
    int v = get_essential_config(BRIDGE_ESSENTIAL_CONFIG_INDEX_parameter_mousekey_high_speed_of_pointer);
    return getvalue(v, 0);
  }
  static unsigned int get_mousekey_initial_wait_of_pointer(void) {
    int v = get_essential_config(BRIDGE_ESSENTIAL_CONFIG_INDEX_parameter_mousekey_initial_wait_of_pointer);
    return getvalue(v, 0);
  }
  static unsigned int get_mousekey_repeat_wait_of_pointer(void) {
    int v = get_essential_config(BRIDGE_ESSENTIAL_CONFIG_INDEX_parameter_mousekey_repeat_wait_of_pointer);
    return getvalue(v, 0);
  }
  static unsigned int get_mousekey_high_speed_of_scroll(void) {
    int v = get_essential_config(BRIDGE_ESSENTIAL_CONFIG_INDEX_parameter_mousekey_high_speed_of_scroll);
    return getvalue(v, 0);
  }
  static unsigned int get_mousekey_initial_wait_of_scroll(void) {
    int v = get_essential_config(BRIDGE_ESSENTIAL_CONFIG_INDEX_parameter_mousekey_initial_wait_of_scroll);
    return getvalue(v, 0);
  }
  static unsigned int get_mousekey_repeat_wait_of_scroll(void) {
    int v = get_essential_config(BRIDGE_ESSENTIAL_CONFIG_INDEX_parameter_mousekey_repeat_wait_of_scroll);
    return getvalue(v, 0);
  }
  static unsigned int get_fixed_distance_magnification(void) {
    int v = get_essential_config(BRIDGE_ESSENTIAL_CONFIG_INDEX_parameter_fixed_distance_magnification);
    return getvalue(v, 0);
  }
  static unsigned int get_fixed_distance_scroll_magnification(void) {
    int v = get_essential_config(BRIDGE_ESSENTIAL_CONFIG_INDEX_parameter_fixed_distance_scroll_magnification);
    return getvalue(v, 0);
  }

private:
  static unsigned int getvalue(int value, int minval) {
    if (value < minval) return minval;
    return value;
  }
  static unsigned int getvalue(int value, int minval, int maxval) {
    if (value < minval) return minval;
    if (value > maxval) return maxval;
    return value;
  }

  static void load_essential_config_default(void);

  static int essential_config_[BRIDGE_ESSENTIAL_CONFIG_INDEX__END__];
  static const int essential_config_default_[BRIDGE_ESSENTIAL_CONFIG_INDEX__END__];
};
}

#endif
