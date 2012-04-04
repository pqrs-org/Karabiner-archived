#include <boost/algorithm/string.hpp>
#include "pqrs/bridge.h"
#include "pqrs/vector.hpp"
#include "pqrs/xml_compiler.hpp"

namespace pqrs {
  xml_compiler::filter_vector::filter_vector(void)
  {}

  xml_compiler::filter_vector::filter_vector(const symbol_map& symbol_map, const boost::property_tree::ptree& pt)
  {
    for (auto& it : pt) {
      /*  */ if (it.first == "not") {
        add(symbol_map, BRIDGE_FILTERTYPE_APPLICATION_NOT,  "ApplicationType::", it.second.data());
      } else if (it.first == "only") {
        add(symbol_map, BRIDGE_FILTERTYPE_APPLICATION_ONLY, "ApplicationType::", it.second.data());
      } else if (it.first == "device_not") {
        add(symbol_map, BRIDGE_FILTERTYPE_DEVICE_NOT,  "", it.second.data());
      } else if (it.first == "device_only") {
        add(symbol_map, BRIDGE_FILTERTYPE_DEVICE_ONLY, "", it.second.data());
      } else if (it.first == "config_not") {
        add(symbol_map, BRIDGE_FILTERTYPE_CONFIG_NOT,  "ConfigIndex::", it.second.data());
      } else if (it.first == "config_only") {
        add(symbol_map, BRIDGE_FILTERTYPE_CONFIG_ONLY, "ConfigIndex::", it.second.data());
      } else if (it.first == "modifier_not") {
        add(symbol_map, BRIDGE_FILTERTYPE_MODIFIER_NOT,  "", it.second.data());
      } else if (it.first == "modifier_only") {
        add(symbol_map, BRIDGE_FILTERTYPE_MODIFIER_ONLY, "", it.second.data());
      } else if (it.first == "inputmode_not") {
        add(symbol_map, BRIDGE_FILTERTYPE_INPUTMODE_NOT,  "InputMode::", it.second.data());
      } else if (it.first == "inputmode_only") {
        add(symbol_map, BRIDGE_FILTERTYPE_INPUTMODE_ONLY, "InputMode::", it.second.data());
      } else if (it.first == "inputmodedetail_not") {
        add(symbol_map, BRIDGE_FILTERTYPE_INPUTMODEDETAIL_NOT,  "InputModeDetail::", it.second.data());
      } else if (it.first == "inputmodedetail_only") {
        add(symbol_map, BRIDGE_FILTERTYPE_INPUTMODEDETAIL_ONLY, "InputModeDetail::", it.second.data());
      }
    }
  }

  std::vector<uint32_t>&
  xml_compiler::filter_vector::get(void)
  {
    return data_;
  }

  const std::vector<uint32_t>&
  xml_compiler::filter_vector::get(void) const
  {
    return data_;
  }

  bool
  xml_compiler::filter_vector::empty(void) const
  {
    return data_.empty();
  }

  void
  xml_compiler::filter_vector::add(const symbol_map& symbol_map,
                                   uint32_t filter_type,
                                   const std::string& prefix,
                                   const std::string& string)
  {
    std::vector<std::string> values;
    pqrs::string::split_by_comma(values, string);

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
        normalize_identifier(key);
        filter_value |= symbol_map.get(key);
      }
      data_.push_back(filter_value);
    }
  }
}
