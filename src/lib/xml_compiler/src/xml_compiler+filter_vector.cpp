#include "bridge.h"
#include "pqrs/vector.hpp"
#include "pqrs/xml_compiler.hpp"
#include "pqrs/xml_compiler/utilities.hpp"

namespace pqrs {
void xml_compiler::filter_vector::traverse(const extracted_ptree& pt) {
  for (const auto& it : pt) {
    /*  */ if (it.get_tag_name() == "not") {
      add_(BRIDGE_FILTERTYPE_APPLICATION_NOT, "ApplicationType::", it.get_data());
    } else if (it.get_tag_name() == "only") {
      add_(BRIDGE_FILTERTYPE_APPLICATION_ONLY, "ApplicationType::", it.get_data());
    } else if (it.get_tag_name() == "windowname_not") {
      add_(BRIDGE_FILTERTYPE_WINDOWNAME_NOT, "WindowName::", it.get_data());
    } else if (it.get_tag_name() == "windowname_only") {
      add_(BRIDGE_FILTERTYPE_WINDOWNAME_ONLY, "WindowName::", it.get_data());
    } else if (it.get_tag_name() == "uielementrole_not") {
      add_(BRIDGE_FILTERTYPE_UIELEMENTROLE_NOT, "UIElementRole::", it.get_data());
    } else if (it.get_tag_name() == "uielementrole_only") {
      add_(BRIDGE_FILTERTYPE_UIELEMENTROLE_ONLY, "UIElementRole::", it.get_data());
    } else if (it.get_tag_name() == "shellcommand_not") {
      add_(BRIDGE_FILTERTYPE_SHELLCOMMAND_NOT, "", it.get_data());
    } else if (it.get_tag_name() == "shellcommand_only") {
      add_(BRIDGE_FILTERTYPE_SHELLCOMMAND_ONLY, "", it.get_data());
    } else if (it.get_tag_name() == "config_not") {
      add_(BRIDGE_FILTERTYPE_CONFIG_NOT, "ConfigIndex::", it.get_data());
    } else if (it.get_tag_name() == "config_only") {
      add_(BRIDGE_FILTERTYPE_CONFIG_ONLY, "ConfigIndex::", it.get_data());
    } else if (it.get_tag_name() == "device_not") {
      add_(BRIDGE_FILTERTYPE_DEVICE_NOT, "", it.get_data());
    } else if (it.get_tag_name() == "device_only") {
      add_(BRIDGE_FILTERTYPE_DEVICE_ONLY, "", it.get_data());
    } else if (it.get_tag_name() == "deviceexists_not") {
      add_(BRIDGE_FILTERTYPE_DEVICEEXISTS_NOT, "", it.get_data());
    } else if (it.get_tag_name() == "deviceexists_only") {
      add_(BRIDGE_FILTERTYPE_DEVICEEXISTS_ONLY, "", it.get_data());
    } else if (it.get_tag_name() == "elapsedtimesincelastpressed_greaterthan") {
      add_(BRIDGE_FILTERTYPE_ELAPSEDTIMESINCELASTPRESSED_GREATERTHAN, "", it.get_data());
    } else if (it.get_tag_name() == "elapsedtimesincelastpressed_lessthan") {
      add_(BRIDGE_FILTERTYPE_ELAPSEDTIMESINCELASTPRESSED_LESSTHAN, "", it.get_data());
    } else if (it.get_tag_name() == "elapsedtimesincelastreleased_greaterthan") {
      add_(BRIDGE_FILTERTYPE_ELAPSEDTIMESINCELASTRELEASED_GREATERTHAN, "", it.get_data());
    } else if (it.get_tag_name() == "elapsedtimesincelastreleased_lessthan") {
      add_(BRIDGE_FILTERTYPE_ELAPSEDTIMESINCELASTRELEASED_LESSTHAN, "", it.get_data());
    } else if (it.get_tag_name() == "modifier_not") {
      add_(BRIDGE_FILTERTYPE_MODIFIER_NOT, "", it.get_data());
    } else if (it.get_tag_name() == "modifier_only") {
      add_(BRIDGE_FILTERTYPE_MODIFIER_ONLY, "", it.get_data());
    } else if (it.get_tag_name() == "modifierlocked_not") {
      add_(BRIDGE_FILTERTYPE_MODIFIER_LOCKED_NOT, "", it.get_data());
    } else if (it.get_tag_name() == "modifierlocked_only") {
      add_(BRIDGE_FILTERTYPE_MODIFIER_LOCKED_ONLY, "", it.get_data());
    } else if (it.get_tag_name() == "modifierstuck_not") {
      add_(BRIDGE_FILTERTYPE_MODIFIER_STUCK_NOT, "", it.get_data());
    } else if (it.get_tag_name() == "modifierstuck_only") {
      add_(BRIDGE_FILTERTYPE_MODIFIER_STUCK_ONLY, "", it.get_data());
    } else if (it.get_tag_name() == "inputsource_not" ||
               it.get_tag_name() == "inputmode_not" ||
               it.get_tag_name() == "inputsourcedetail_not" ||
               it.get_tag_name() == "inputmodedetail_not") {
      // We allow "inputmode_*", "inputmodedetail_*", "inputsourcedetail_*" for compatibility.
      add_(BRIDGE_FILTERTYPE_INPUTSOURCE_NOT, "InputSource::", it.get_data());
    } else if (it.get_tag_name() == "inputsource_only" ||
               it.get_tag_name() == "inputmode_only" ||
               it.get_tag_name() == "inputsourcedetail_only" ||
               it.get_tag_name() == "inputmodedetail_only") {
      // We allow "inputmode_*", "inputmodedetail_*", "inputsourcedetail_*" for compatibility.
      add_(BRIDGE_FILTERTYPE_INPUTSOURCE_ONLY, "InputSource::", it.get_data());
    } else if (it.get_tag_name() == "lastpressedphysicalkey_not") {
      add_(BRIDGE_FILTERTYPE_LASTPRESSEDPHYSICALKEY_NOT, "", it.get_data());
    } else if (it.get_tag_name() == "lastpressedphysicalkey_only") {
      add_(BRIDGE_FILTERTYPE_LASTPRESSEDPHYSICALKEY_ONLY, "", it.get_data());
    } else if (it.get_tag_name() == "lastreleasedphysicalkey_not") {
      add_(BRIDGE_FILTERTYPE_LASTRELEASEDPHYSICALKEY_NOT, "", it.get_data());
    } else if (it.get_tag_name() == "lastreleasedphysicalkey_only") {
      add_(BRIDGE_FILTERTYPE_LASTRELEASEDPHYSICALKEY_ONLY, "", it.get_data());
    } else if (it.get_tag_name() == "lastsentevent_not") {
      add_(BRIDGE_FILTERTYPE_LASTSENTEVENT_NOT, "", it.get_data());
    } else if (it.get_tag_name() == "lastsentevent_only") {
      add_(BRIDGE_FILTERTYPE_LASTSENTEVENT_ONLY, "", it.get_data());
    } else if (it.get_tag_name() == "pressingphysicalkeys_greaterthan") {
      add_(BRIDGE_FILTERTYPE_PRESSINGPHYSICALKEYS_GREATERTHAN, "Count::RawValue::", it.get_data());
    } else if (it.get_tag_name() == "pressingphysicalkeys_lessthan") {
      add_(BRIDGE_FILTERTYPE_PRESSINGPHYSICALKEYS_LESSTHAN, "Count::RawValue::", it.get_data());
    }
  }
}

void xml_compiler::filter_vector::add_(uint32_t filter_type,
                                       const std::string& prefix,
                                       const std::string& string) {
  std::string string_no_whitespaces(string);
  pqrs::string::remove_whitespaces(string_no_whitespaces);

  size_t count_index = data_.size();
  uint32_t count = 0;

  data_.push_back(count);

  data_.push_back(filter_type);
  ++count;

  pqrs::string::tokenizer tokenizer_comma(string_no_whitespaces, ',');
  std::string arg;
  while (tokenizer_comma.split_removing_empty(arg)) {
    switch (filter_type) {
    case BRIDGE_FILTERTYPE_MODIFIER_NOT:
    case BRIDGE_FILTERTYPE_MODIFIER_ONLY:
    case BRIDGE_FILTERTYPE_MODIFIER_LOCKED_NOT:
    case BRIDGE_FILTERTYPE_MODIFIER_LOCKED_ONLY:
    case BRIDGE_FILTERTYPE_MODIFIER_STUCK_NOT:
    case BRIDGE_FILTERTYPE_MODIFIER_STUCK_ONLY: {
      // support '|' for <modifier_only>.
      // For example: <modifier_only>ModifierFlag::COMMAND_L|ModifierFlag::CONTROL_L, ModifierFlag::COMMAND_L|ModifierFlag::OPTION_L</modifier_only>
      pqrs::string::tokenizer tokenizer_pipe(arg, '|');
      std::string value;
      while (tokenizer_pipe.split_removing_empty(value)) {
        std::string key = prefix + value;
        normalize_identifier_(key);
        data_.push_back(BRIDGE_DATATYPE_MODIFIERFLAG);
        ++count;
        data_.push_back(symbol_map_.get(key));
        ++count;
      }
      data_.push_back(BRIDGE_DATATYPE_MODIFIERFLAGS_END);
      ++count;
      data_.push_back(1);
      ++count;
      break;
    }

    case BRIDGE_FILTERTYPE_ELAPSEDTIMESINCELASTPRESSED_GREATERTHAN:
    case BRIDGE_FILTERTYPE_ELAPSEDTIMESINCELASTPRESSED_LESSTHAN:
    case BRIDGE_FILTERTYPE_ELAPSEDTIMESINCELASTRELEASED_GREATERTHAN:
    case BRIDGE_FILTERTYPE_ELAPSEDTIMESINCELASTRELEASED_LESSTHAN:
    case BRIDGE_FILTERTYPE_LASTPRESSEDPHYSICALKEY_NOT:
    case BRIDGE_FILTERTYPE_LASTPRESSEDPHYSICALKEY_ONLY:
    case BRIDGE_FILTERTYPE_LASTRELEASEDPHYSICALKEY_NOT:
    case BRIDGE_FILTERTYPE_LASTRELEASEDPHYSICALKEY_ONLY:
    case BRIDGE_FILTERTYPE_LASTSENTEVENT_NOT:
    case BRIDGE_FILTERTYPE_LASTSENTEVENT_ONLY: {
      normalize_identifier_(arg);
      data_.push_back(xml_compiler_utilities::get_datatype(arg));
      ++count;
      data_.push_back(symbol_map_.get(arg));
      ++count;
      break;
    }

    case BRIDGE_FILTERTYPE_DEVICE_NOT:
    case BRIDGE_FILTERTYPE_DEVICE_ONLY:
    case BRIDGE_FILTERTYPE_DEVICEEXISTS_NOT:
    case BRIDGE_FILTERTYPE_DEVICEEXISTS_ONLY: {
      if (boost::starts_with(arg, "DeviceVendor")) {
        fill_omitted_device_specifying(count);
      }
      // pass-through (== no break)
    }

    default: {
      std::string key = prefix + arg;
      normalize_identifier_(key);
      data_.push_back(symbol_map_.get(key));
      ++count;
      break;
    }
    }
  }

  // ------------------------------------------------------------
  // Fill DeviceProduct, DeviceLocation if needed.
  if (filter_type == BRIDGE_FILTERTYPE_DEVICE_NOT ||
      filter_type == BRIDGE_FILTERTYPE_DEVICE_ONLY ||
      filter_type == BRIDGE_FILTERTYPE_DEVICEEXISTS_NOT ||
      filter_type == BRIDGE_FILTERTYPE_DEVICEEXISTS_ONLY) {
    fill_omitted_device_specifying(count);
  }

  // ------------------------------------------------------------
  if (count == 1) {
    // Rollback if filter is empty.
    data_.resize(count_index);
  } else {
    data_[count_index] = count;
  }
}

void xml_compiler::filter_vector::fill_omitted_device_specifying(uint32_t& count) {
  // <device_not> and <device_only> require a strict arguments order.
  // 1st: DeviceVendor
  // 2nd: DeviceProduct
  // 3rd: DeviceLocation
  //
  // For example:
  // <device_not>DeviceVendor::XXX, DeviceProduct::YYY, DeviceLocation::ZZZ</device_not>
  //
  // However, for compatibility,
  // we need to append DeviceLocation automatically if it is not given.
  //
  // We need to convert
  //   <device_not>DeviceVendor::XXX, DeviceProduct::YYY</device_not>
  // to
  //   <device_not>DeviceVendor::XXX, DeviceProduct::YYY, DeviceLocation::ANY</device_not>
  //

  switch ((count - 1) % 3) {
  case 0:
    break;

  case 1:
    // pass-through (== no break)
    data_.push_back(symbol_map_.get("DeviceProduct::ANY"));
    ++count;

  case 2:
    data_.push_back(symbol_map_.get("DeviceLocation::ANY"));
    ++count;
  }
}
}
