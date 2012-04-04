#include <algorithm>
#include <exception>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include "pqrs/xml_compiler.hpp"
#include "pqrs/bridge.h"
#include "pqrs/string.hpp"
#include "pqrs/vector.hpp"

namespace pqrs {
  void
  xml_compiler::reload_autogen_(void)
  {
    confignamemap_.clear();
    remapclasses_initialize_vector_.clear();
    simultaneous_keycode_index_ = 0;

    std::vector<xml_file_path_ptr> xml_file_path_ptrs;
    xml_file_path_ptrs.push_back(
      xml_file_path_ptr(new xml_file_path(xml_file_path::base_directory::private_xml, "private.xml")));
    xml_file_path_ptrs.push_back(
      xml_file_path_ptr(new xml_file_path(xml_file_path::base_directory::system_xml,  "checkbox.xml")));

    std::vector<ptree_ptr> pt_ptrs;
    read_xmls_(pt_ptrs, xml_file_path_ptrs);

    // ----------------------------------------
    // add_configindex_and_keycode_to_symbol_map_
    //   1st loop: <identifier>notsave.*</identifier>
    //   2nd loop: other <identifier>
    //
    // We need to assign higher priority to notsave.* settings.
    // So, adding configindex by 2steps.
    for (auto& pt_ptr : pt_ptrs) {
      add_configindex_and_keycode_to_symbol_map_(*pt_ptr, "", true);
    }
    for (auto& pt_ptr : pt_ptrs) {
      add_configindex_and_keycode_to_symbol_map_(*pt_ptr, "", false);
    }

    // ----------------------------------------
    for (auto& pt_ptr : pt_ptrs) {
      traverse_identifier_(*pt_ptr, "");
    }

    remapclasses_initialize_vector_.freeze();
  }

  bool
  xml_compiler::valid_identifier_(const std::string identifier, const std::string parent_tag_name)
  {
    if (identifier.empty()) {
      set_error_message_("Empty <identifier>.");
      return false;
    }

    if (parent_tag_name != "item") {
      set_error_message_(boost::format("<identifier> must be placed directly under <item>:\n"
                                       "\n"
                                       "<identifier>%1%</identifier>") %
                         identifier);
      return false;
    }

    return true;
  }

  void
  xml_compiler::add_configindex_and_keycode_to_symbol_map_(const boost::property_tree::ptree& pt,
                                                           const std::string& parent_tag_name,
                                                           bool handle_notsave)
  {
    for (auto& it : pt) {
      if (it.first != "identifier") {
        add_configindex_and_keycode_to_symbol_map_(it.second, it.first, handle_notsave);
      } else {
        auto identifier = boost::trim_copy(it.second.data());
        if (! valid_identifier_(identifier, parent_tag_name)) {
          continue;
        }
        normalize_identifier(identifier);

        // ----------------------------------------
        // Do not treat essentials.
        auto attr_essential = it.second.get_optional<std::string>("<xmlattr>.essential");
        if (attr_essential) {
          continue;
        }

        // ----------------------------------------
        if (handle_notsave != boost::starts_with(identifier, "notsave_")) {
          continue;
        }

        // ----------------------------------------
        auto attr_vk_config = it.second.get_optional<std::string>("<xmlattr>.vk_config");
        if (attr_vk_config) {
          const char* names[] = {
            "VK_CONFIG_TOGGLE_",
            "VK_CONFIG_FORCE_ON_",
            "VK_CONFIG_FORCE_OFF_",
            "VK_CONFIG_SYNC_KEYDOWNUP_",
          };
          for (auto& n : names) {
            symbol_map_.add("KeyCode", std::string(n) + identifier);
          }
        }

        // ----------------------------------------
        symbol_map_.add("ConfigIndex", identifier);
      }
    }
  }

  void
  xml_compiler::traverse_identifier_(const boost::property_tree::ptree& pt,
                                     const std::string& parent_tag_name)
  {
    for (auto& it : pt) {
      try {
        if (it.first != "identifier") {
          traverse_identifier_(it.second, it.first);

        } else {
          auto attr_essential = it.second.get_optional<std::string>("<xmlattr>.essential");
          if (attr_essential) {
            continue;
          }

          std::vector<uint32_t> initialize_vector;
          auto raw_identifier = boost::trim_copy(it.second.data());
          if (! valid_identifier_(raw_identifier, parent_tag_name)) {
            continue;
          }
          auto identifier = raw_identifier;
          normalize_identifier(identifier);

          auto attr_vk_config = it.second.get_optional<std::string>("<xmlattr>.vk_config");
          if (attr_vk_config) {
            initialize_vector.push_back(5); // count
            initialize_vector.push_back(BRIDGE_VK_CONFIG);

            const char* names[] = {
              "VK_CONFIG_TOGGLE_",
              "VK_CONFIG_FORCE_ON_",
              "VK_CONFIG_FORCE_OFF_",
              "VK_CONFIG_SYNC_KEYDOWNUP_",
            };
            for (auto& n : names) {
              initialize_vector.push_back(symbol_map_.get("KeyCode", std::string(n) + identifier));
            }
          }

          filter_vector fv;
          traverse_autogen_(pt, identifier, fv, initialize_vector);

          uint32_t configindex = symbol_map_.get("ConfigIndex", identifier);
          remapclasses_initialize_vector_.add(initialize_vector, configindex, raw_identifier);
          confignamemap_[configindex] = raw_identifier;
        }

      } catch (std::exception& e) {
        set_error_message_(e.what());
      }
    }
  }

  void
  xml_compiler::traverse_autogen_(const boost::property_tree::ptree& pt,
                                  const std::string& identifier,
                                  const filter_vector& parent_filter_vector,
                                  std::vector<uint32_t>& initialize_vector)
  {
    filter_vector fv(symbol_map_, pt);

    // Add passthrough filter.
    if (parent_filter_vector.empty() &&
        ! boost::starts_with(identifier, "passthrough_")) {
      fv.get().push_back(2); // count
      fv.get().push_back(BRIDGE_FILTERTYPE_CONFIG_NOT);
      fv.get().push_back(symbol_map_.get("ConfigIndex::notsave_passthrough"));
    }

    // Add parent filters.
    pqrs::vector::push_back(fv.get(), parent_filter_vector.get());

    // ----------------------------------------
    for (auto& it : pt) {
      try {
        if (it.first != "autogen") {
          traverse_autogen_(it.second, identifier, fv, initialize_vector);

        } else {
          std::string raw_autogen = boost::trim_copy(it.second.data());

          // drop whitespaces for preprocessor. (for FROMKEYCODE_HOME, etc)
          // Note: preserve space when --ShowStatusMessage--.
          std::string autogen(raw_autogen);
          if (! boost::starts_with(autogen, "--ShowStatusMessage--")) {
            pqrs::string::remove_whitespaces(autogen);
          }

          handle_autogen(autogen, raw_autogen, fv, initialize_vector);
        }

      } catch (std::exception& e) {
        set_error_message_(e.what());
      }
    }
  }

  void
  xml_compiler::handle_autogen(const std::string& autogen,
                               const std::string& raw_autogen,
                               const filter_vector& filter_vector,
                               std::vector<uint32_t>& initialize_vector)
  {
    // ------------------------------------------------------------
    // preprocess
    //

    // VK_COMMAND, VK_CONTROL, VK_SHIFT, VK_OPTION
    {
      const char* keys[] = { "COMMAND", "CONTROL", "SHIFT", "OPTION" };
      for (auto& k : keys) {
        std::string vk = std::string("VK_") + k;
        if (autogen.find(vk) != std::string::npos) {
          const char* suffix[] = { "_L", "_R" };
          for (auto& s : suffix) {
            handle_autogen(boost::replace_all_copy(autogen, vk, std::string("ModifierFlag::") + k + s),
                           raw_autogen, filter_vector, initialize_vector);
          }
          return;
        }
      }
    }

    // VK_MOD_*
    {
      const char* keys[][2] = {
        { "VK_MOD_CCOS_L", "ModifierFlag::COMMAND_L|ModifierFlag::CONTROL_L|ModifierFlag::OPTION_L|ModifierFlag::SHIFT_L" },
        { "VK_MOD_CCS_L",  "ModifierFlag::COMMAND_L|ModifierFlag::CONTROL_L|ModifierFlag::SHIFT_L" },
        { "VK_MOD_CCO_L",  "ModifierFlag::COMMAND_L|ModifierFlag::CONTROL_L|ModifierFlag::OPTION_L" },
      };
      for (auto& k : keys) {
        if (autogen.find(k[0]) != std::string::npos) {
          handle_autogen(boost::replace_all_copy(autogen, k[0], k[1]),
                         raw_autogen, filter_vector, initialize_vector);
          return;
        }
      }
    }

    // VK_MOD_ANY
    if (autogen.find("VK_MOD_ANY") != std::string::npos) {
      // to reduce combination, we ignore same modifier combination such as (COMMAND_L | COMMAND_R).
      const char* seeds[] = { "VK_COMMAND", "VK_CONTROL", "ModifierFlag::FN", "VK_OPTION", "VK_SHIFT" };
      std::vector<std::tr1::shared_ptr<std::vector<std::string> > > combination;
      pqrs::vector::make_combination(combination, seeds, sizeof(seeds) / sizeof(seeds[0]));

      for (auto& v : combination) {
        handle_autogen(boost::replace_all_copy(autogen, "VK_MOD_ANY", boost::join(*v, "|") + "|ModifierFlag::NONE"),
                       raw_autogen, filter_vector, initialize_vector);
      }
      return;
    }

    // FROMKEYCODE_HOME, FROMKEYCODE_END, ...
    {
      const char* keys[][2] = {
        { "HOME",           "CURSOR_LEFT"  },
        { "END",            "CURSOR_RIGHT" },
        { "PAGEUP",         "CURSOR_UP"    },
        { "PAGEDOWN",       "CURSOR_DOWN"  },
        { "FORWARD_DELETE", "DELETE"       },
      };
      for (auto& k : keys) {
        std::string fromkeycode = std::string("FROMKEYCODE_") + k[0];
        if (autogen.find(fromkeycode + ",ModifierFlag::") != std::string::npos) {
          handle_autogen(boost::replace_all_copy(autogen, fromkeycode, std::string("KeyCode::") + k[0]),
                         raw_autogen, filter_vector, initialize_vector);
          handle_autogen(boost::replace_all_copy(autogen,
                                                 fromkeycode + ",",
                                                 std::string("KeyCode::") + k[1] + ",ModifierFlag::FN|"),
                         raw_autogen, filter_vector, initialize_vector);
          return;
        }
        if (autogen.find(fromkeycode) != std::string::npos) {
          handle_autogen(boost::replace_all_copy(autogen, fromkeycode, std::string("KeyCode::") + k[0]),
                         raw_autogen, filter_vector, initialize_vector);
          handle_autogen(boost::replace_all_copy(autogen,
                                                 fromkeycode,
                                                 std::string("KeyCode::") + k[1] + ",ModifierFlag::FN"),
                         raw_autogen, filter_vector, initialize_vector);
          return;
        }
      }
    }

    // For compatibility
    if (boost::starts_with(autogen, "--KeyOverlaidModifierWithRepeat--")) {
      handle_autogen(boost::replace_first_copy(autogen,
                                               "--KeyOverlaidModifierWithRepeat--",
                                               "--KeyOverlaidModifier--Option::KEYOVERLAIDMODIFIER_REPEAT,"),
                     raw_autogen, filter_vector, initialize_vector);
      return;
    }

    if (boost::starts_with(autogen, "--StripModifierFromScrollWheel--")) {
      handle_autogen(boost::replace_first_copy(autogen,
                                               "--StripModifierFromScrollWheel--",
                                               "--ScrollWheelToScrollWheel--") + ",ModifierFlag::NONE",
                     raw_autogen, filter_vector, initialize_vector);
      return;
    }

    if (autogen.find("SimultaneousKeyPresses::Option::RAW") != std::string::npos) {
      handle_autogen(boost::replace_all_copy(autogen,
                                             "SimultaneousKeyPresses::Option::RAW",
                                             "Option::SIMULTANEOUSKEYPRESSES_RAW"),
                     raw_autogen, filter_vector, initialize_vector);
      return;
    }

    // ------------------------------------------------------------
    // add to initialize_vector
    //

    {
      const char* symbol = "--ShowStatusMessage--";
      if (boost::starts_with(autogen, symbol)) {
        std::string params = autogen.substr(strlen(symbol));
        boost::trim(params);

        size_t length = params.size();
        initialize_vector.push_back(length + 1);
        initialize_vector.push_back(BRIDGE_STATUSMESSAGE);

        std::copy(params.begin(), params.end(), std::back_inserter(initialize_vector));
        // no need filter_vector
        return;
      }
    }

    {
      const char* symbol = "--SimultaneousKeyPresses--";
      if (boost::starts_with(autogen, symbol)) {
        std::string params = autogen.substr(strlen(symbol));
        boost::trim(params);

        std::string newkeycode = std::string("VK_SIMULTANEOUSKEYPRESSES_") +
                                 boost::lexical_cast<std::string>(simultaneous_keycode_index_);
        symbol_map_.add("KeyCode", newkeycode);
        ++simultaneous_keycode_index_;

        params = std::string("KeyCode::") + newkeycode + "," + params;
        add_to_initialize_vector(params, BRIDGE_REMAPTYPE_SIMULTANEOUSKEYPRESSES, filter_vector, initialize_vector);
        return;
      }
    }

    static struct {
      const char* symbol;
      uint32_t type;
    } info[] = {
      { "--KeyToKey--",                       BRIDGE_REMAPTYPE_KEYTOKEY },
      { "--KeyToConsumer--",                  BRIDGE_REMAPTYPE_KEYTOCONSUMER },
      { "--KeyToPointingButton--",            BRIDGE_REMAPTYPE_KEYTOPOINTINGBUTTON },
      { "--DoublePressModifier--",            BRIDGE_REMAPTYPE_DOUBLEPRESSMODIFIER },
      { "--HoldingKeyToKey--",                BRIDGE_REMAPTYPE_HOLDINGKEYTOKEY },
      { "--IgnoreMultipleSameKeyPress--",     BRIDGE_REMAPTYPE_IGNOREMULTIPLESAMEKEYPRESS },
      { "--KeyOverlaidModifier--",            BRIDGE_REMAPTYPE_KEYOVERLAIDMODIFIER },
      { "--ConsumerToConsumer--",             BRIDGE_REMAPTYPE_CONSUMERTOCONSUMER },
      { "--ConsumerToKey--",                  BRIDGE_REMAPTYPE_CONSUMERTOKEY },
      { "--PointingButtonToPointingButton--", BRIDGE_REMAPTYPE_POINTINGBUTTONTOPOINTINGBUTTON },
      { "--PointingButtonToKey--",            BRIDGE_REMAPTYPE_POINTINGBUTTONTOKEY },
      { "--PointingRelativeToScroll--",       BRIDGE_REMAPTYPE_POINTINGRELATIVETOSCROLL },
      { "--DropKeyAfterRemap--",              BRIDGE_REMAPTYPE_DROPKEYAFTERREMAP },
      { "--SetKeyboardType--",                BRIDGE_REMAPTYPE_SETKEYBOARDTYPE },
      { "--ForceNumLockOn--",                 BRIDGE_REMAPTYPE_FORCENUMLOCKON },
      { "--DropPointingRelativeCursorMove--", BRIDGE_REMAPTYPE_DROPPOINTINGRELATIVECURSORMOVE },
      { "--DropScrollWheel--",                BRIDGE_REMAPTYPE_DROPSCROLLWHEEL },
      { "--ScrollWheelToScrollWheel--",       BRIDGE_REMAPTYPE_SCROLLWHEELTOSCROLLWHEEL },
      { "--ScrollWheelToKey--",               BRIDGE_REMAPTYPE_SCROLLWHEELTOKEY },
    };
    for (auto& it : info) {
      if (boost::starts_with(autogen, it.symbol)) {
        std::string params = autogen.substr(strlen(it.symbol));
        boost::trim(params);

        add_to_initialize_vector(params, it.type, filter_vector, initialize_vector);
        return;
      }
    }

    throw xml_compiler_runtime_error(boost::format("Invalid <autogen>:\n"
                                                   "\n"
                                                   "<autogen>%1%</autogen>") %
                                     raw_autogen);
  }

  void
  xml_compiler::add_to_initialize_vector(const std::string& params,
                                         uint32_t type,
                                         const filter_vector& filter_vector,
                                         std::vector<uint32_t>& initialize_vector)
  {
    std::vector<uint32_t> vector;
    vector.push_back(type);

    std::vector<std::string> args;
    pqrs::string::split_by_comma(args, params);
    for (auto& a : args) {
      unsigned int datatype = 0;
      unsigned int newvalue = 0;

      std::vector<std::string> values;
      pqrs::string::split_by_pipe(values, a);

      for (auto& v : values) {
        unsigned int newdatatype = 0;
        /*  */ if (boost::starts_with(v, "KeyCode::")) {
          newdatatype = BRIDGE_DATATYPE_KEYCODE;
        } else if (boost::starts_with(v, "ModifierFlag::")) {
          newdatatype = BRIDGE_DATATYPE_FLAGS;
        } else if (boost::starts_with(v, "ConsumerKeyCode::")) {
          newdatatype = BRIDGE_DATATYPE_CONSUMERKEYCODE;
        } else if (boost::starts_with(v, "PointingButton::")) {
          newdatatype = BRIDGE_DATATYPE_POINTINGBUTTON;
        } else if (boost::starts_with(v, "ScrollWheel::")) {
          newdatatype = BRIDGE_DATATYPE_SCROLLWHEEL;
        } else if (boost::starts_with(v, "KeyboardType::")) {
          newdatatype = BRIDGE_DATATYPE_KEYBOARDTYPE;
        } else if (boost::starts_with(v, "DeviceVendor::")) {
          newdatatype = BRIDGE_DATATYPE_DEVICEVENDOR;
        } else if (boost::starts_with(v, "DeviceProduct::")) {
          newdatatype = BRIDGE_DATATYPE_DEVICEPRODUCT;
        } else if (boost::starts_with(v, "Option::")) {
          newdatatype = BRIDGE_DATATYPE_OPTION;
        } else {
          throw xml_compiler_runtime_error("Unknown symbol:\n\n" + v);
        }

        if (datatype) {
          // There are some connection(|).

          if (newdatatype != BRIDGE_DATATYPE_FLAGS &&
              newdatatype != BRIDGE_DATATYPE_POINTINGBUTTON) {
            // Don't connect no-flags. (Example: KeyCode::A | KeyCode::B)
            throw xml_compiler_runtime_error("Cannot connect(|) except ModifierFlag and PointingButton:\n\n" + a);
          }

          if (newdatatype != datatype) {
            // Don't connect different data type. (Example: PointingButton::A | ModifierFlag::SHIFT_L)
            throw xml_compiler_runtime_error("Cannot connect(|) between different types:\n\n" + a);
          }
        }

        datatype = newdatatype;
        newvalue |= symbol_map_.get(v);
      }

      vector.push_back(datatype);
      vector.push_back(newvalue);
    }

    initialize_vector.push_back(vector.size());
    pqrs::vector::push_back(initialize_vector, vector);
    pqrs::vector::push_back(initialize_vector, filter_vector.get());
  }
}
