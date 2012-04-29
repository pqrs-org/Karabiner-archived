#include <boost/algorithm/string.hpp>
#include "bridge.h"
#include "pqrs/vector.hpp"
#include "pqrs/xml_compiler.hpp"

namespace pqrs {
  void
  xml_compiler::filter_vector::traverse(const extracted_ptree& pt)
  {
    for (auto& it : pt) {
      /*  */ if (it.get_tag_name() == "not") {
        add_(BRIDGE_FILTERTYPE_APPLICATION_NOT,  "ApplicationType::", it.get_data());
      } else if (it.get_tag_name() == "only") {
        add_(BRIDGE_FILTERTYPE_APPLICATION_ONLY, "ApplicationType::", it.get_data());
      } else if (it.get_tag_name() == "device_not") {
        add_(BRIDGE_FILTERTYPE_DEVICE_NOT,  "", it.get_data());
      } else if (it.get_tag_name() == "device_only") {
        add_(BRIDGE_FILTERTYPE_DEVICE_ONLY, "", it.get_data());
      } else if (it.get_tag_name() == "config_not") {
        add_(BRIDGE_FILTERTYPE_CONFIG_NOT,  "ConfigIndex::", it.get_data());
      } else if (it.get_tag_name() == "config_only") {
        add_(BRIDGE_FILTERTYPE_CONFIG_ONLY, "ConfigIndex::", it.get_data());
      } else if (it.get_tag_name() == "modifier_not") {
        add_(BRIDGE_FILTERTYPE_MODIFIER_NOT,  "", it.get_data());
      } else if (it.get_tag_name() == "modifier_only") {
        add_(BRIDGE_FILTERTYPE_MODIFIER_ONLY, "", it.get_data());
      } else if (it.get_tag_name() == "inputmode_not") {
        add_(BRIDGE_FILTERTYPE_INPUTMODE_NOT,  "InputMode::", it.get_data());
      } else if (it.get_tag_name() == "inputmode_only") {
        add_(BRIDGE_FILTERTYPE_INPUTMODE_ONLY, "InputMode::", it.get_data());
      } else if (it.get_tag_name() == "inputmodedetail_not") {
        add_(BRIDGE_FILTERTYPE_INPUTMODEDETAIL_NOT,  "InputModeDetail::", it.get_data());
      } else if (it.get_tag_name() == "inputmodedetail_only") {
        add_(BRIDGE_FILTERTYPE_INPUTMODEDETAIL_ONLY, "InputModeDetail::", it.get_data());
      }
    }
  }

  void
  xml_compiler::filter_vector::add_(uint32_t filter_type,
                                    const std::string& prefix,
                                    const std::string& string)
  {
    std::vector<std::string> values;
    pqrs::string::split_by_comma(values, string);
    if (values.empty()) return;

    data_.push_back(values.size() + 1); // +1 == filter_type
    data_.push_back(filter_type);

    for (auto& v : values) {
      // support '|' for <modifier_only>.
      // For example: <modifier_only>ModifierFlag::COMMAND_L|ModifierFlag::CONTROL_L, ModifierFlag::COMMAND_L|ModifierFlag::OPTION_L</modifier_only>
      std::vector<std::string> items;
      pqrs::string::split_by_pipe(items, v);

      uint32_t filter_value = 0;
      for (auto& i : items) {
        std::string key = prefix + i;
        normalize_identifier_(key);
        filter_value |= symbol_map_.get(key);
      }
      data_.push_back(filter_value);
    }
  }
}
