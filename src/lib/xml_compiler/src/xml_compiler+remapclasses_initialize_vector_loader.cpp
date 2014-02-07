#include <algorithm>
#include <exception>
#include "bridge.h"
#include "pqrs/xml_compiler.hpp"
#include "pqrs/xml_compiler/utilities.hpp"
#include "pqrs/string.hpp"
#include "pqrs/vector.hpp"

namespace pqrs {
  void
  xml_compiler::remapclasses_initialize_vector_loader::traverse(const extracted_ptree& pt,
                                                                const std::string& parent_tag_name)
  {
    for (const auto& it : pt) {
      try {
        if (it.get_tag_name() != "identifier") {
          if (! it.children_empty()) {
            traverse(it.children_extracted_ptree(), it.get_tag_name());
          }

        } else {
          auto attr_essential = it.get_optional("<xmlattr>.essential");
          if (attr_essential) {
            continue;
          }

          // ----------------------------------------
          auto raw_identifier = boost::trim_copy(it.get_data());
          if (! xml_compiler_.valid_identifier_(raw_identifier, parent_tag_name)) {
            continue;
          }
          auto identifier = raw_identifier;
          normalize_identifier_(identifier);

          // ----------------------------------------
          uint32_t config_index = symbol_map_.get("ConfigIndex", identifier);
          identifier_map_[config_index] = raw_identifier;

          // ----------------------------------------
          remapclasses_initialize_vector_.start(config_index, raw_identifier);
          {
            auto attr_vk_config = it.get_optional("<xmlattr>.vk_config");
            if (attr_vk_config) {
              remapclasses_initialize_vector_.push_back(5);
              remapclasses_initialize_vector_.push_back(BRIDGE_VK_CONFIG);

              static const std::string names[] = {
                "VK_CONFIG_TOGGLE_",
                "VK_CONFIG_FORCE_ON_",
                "VK_CONFIG_FORCE_OFF_",
                "VK_CONFIG_SYNC_KEYDOWNUP_",
              };
              for (const auto& n : names) {
                remapclasses_initialize_vector_.push_back(symbol_map_.get("KeyCode", n + identifier));
              }
            }

            filter_vector_.clear();
            traverse_autogen_(pt, identifier, raw_identifier);
          }
          remapclasses_initialize_vector_.end();
        }

      } catch (std::exception& e) {
        xml_compiler_.error_information_.set(e.what());
      }
    }
  }

  void
  xml_compiler::remapclasses_initialize_vector_loader::traverse_autogen_(const extracted_ptree& pt,
                                                                         const std::string& identifier,
                                                                         const std::string& raw_identifier)
  {
    // Add passthrough filter.
    if (filter_vector_.empty() &&
        ! boost::starts_with(identifier, "passthrough_")) {
      filter_vector_.push_back(2); // count
      filter_vector_.push_back(BRIDGE_FILTERTYPE_CONFIG_NOT);
      filter_vector_.push_back(symbol_map_cache_configindex_notsave_passthrough_);
    }

    filter_vector_.traverse(pt);

    // ----------------------------------------
    for (const auto& it : pt) {
      if (it.get_tag_name() == "item") {
        xml_compiler_.error_information_.set(boost::format("You should not write <identifier> in <item> which has child <item> nodes.\nRemove <identifier>%1%</identifier>.") % raw_identifier);

      } else if (it.get_tag_name() != "autogen") {
        size_t s = filter_vector_.size();
        traverse_autogen_(it.children_extracted_ptree(), identifier, raw_identifier);
        filter_vector_.resize(s);

      } else {
        std::string raw_autogen = boost::trim_left_copy(it.get_data());

        // ----------------------------------------
        std::string autogen(raw_autogen);

        // Replacing -- with __ for compatibility.
        // * --KeyToKey-- before version 8.0.0.
        // * __KeyToKey__ since version 8.0.0.
        if (boost::starts_with(autogen, "--")) {
          autogen[0] = '_';
          autogen[1] = '_';
          boost::replace_first(autogen, "--", "__");
        }

        // drop whitespaces for preprocessor. (for FROMKEYCODE_HOME, etc)
        // Note: preserve space when __ShowStatusMessage__.
        if (! boost::starts_with(autogen, "__ShowStatusMessage__")) {
          pqrs::string::remove_whitespaces(autogen);
        }

        handle_autogen(autogen, raw_autogen);
      }
    }
  }

  void
  xml_compiler::remapclasses_initialize_vector_loader::handle_autogen(const std::string& autogen,
                                                                      const std::string& raw_autogen)
  {
    // ------------------------------------------------------------
    // preprocess
    //

    // MODIFIERFLAG_EITHER_LEFT_OR_RIGHT_*
    {
      static const struct {
        const std::string name;
        const std::string flags[2];
      } info[] = {
        { "MODIFIERFLAG_EITHER_LEFT_OR_RIGHT_COMMAND", { "ModifierFlag::COMMAND_L", "ModifierFlag::COMMAND_R" } },
        { "MODIFIERFLAG_EITHER_LEFT_OR_RIGHT_CONTROL", { "ModifierFlag::CONTROL_L", "ModifierFlag::CONTROL_R" } },
        { "MODIFIERFLAG_EITHER_LEFT_OR_RIGHT_SHIFT",   { "ModifierFlag::SHIFT_L",   "ModifierFlag::SHIFT_R"   } },
        { "MODIFIERFLAG_EITHER_LEFT_OR_RIGHT_OPTION",  { "ModifierFlag::OPTION_L",  "ModifierFlag::OPTION_R"  } },

        // for backwards compatibility
        { "VK_COMMAND", { "ModifierFlag::COMMAND_L", "ModifierFlag::COMMAND_R" } },
        { "VK_CONTROL", { "ModifierFlag::CONTROL_L", "ModifierFlag::CONTROL_R" } },
        { "VK_SHIFT",   { "ModifierFlag::SHIFT_L",   "ModifierFlag::SHIFT_R"   } },
        { "VK_OPTION",  { "ModifierFlag::OPTION_L",  "ModifierFlag::OPTION_R"  } },
      };

      for (const auto& it : info) {
        if (autogen.find(it.name) != std::string::npos) {
          for (const auto& f : it.flags) {
            handle_autogen(boost::replace_all_copy(autogen, it.name, f),
                           raw_autogen);
          }
          return;
        }
      }
    }

    // MODIFIERFLAGS_*
    {
      static const struct {
        const std::string name;
        const std::string flag;
      } info[] = {
        { "MODIFIERFLAGS_CCOS_L",
          "ModifierFlag::COMMAND_L|ModifierFlag::CONTROL_L|ModifierFlag::OPTION_L|ModifierFlag::SHIFT_L" },
        { "MODIFIERFLAGS_CCS_L",
          "ModifierFlag::COMMAND_L|ModifierFlag::CONTROL_L|ModifierFlag::SHIFT_L" },
        { "MODIFIERFLAGS_CCO_L",
          "ModifierFlag::COMMAND_L|ModifierFlag::CONTROL_L|ModifierFlag::OPTION_L" },

        // for backwards compatibility
        { "VK_MOD_CCOS_L", "ModifierFlag::COMMAND_L|ModifierFlag::CONTROL_L|ModifierFlag::OPTION_L|ModifierFlag::SHIFT_L" },
        { "VK_MOD_CCS_L",  "ModifierFlag::COMMAND_L|ModifierFlag::CONTROL_L|ModifierFlag::SHIFT_L" },
        { "VK_MOD_CCO_L",  "ModifierFlag::COMMAND_L|ModifierFlag::CONTROL_L|ModifierFlag::OPTION_L" },
      };
      for (const auto& it : info) {
        if (autogen.find(it.name) != std::string::npos) {
          handle_autogen(boost::replace_all_copy(autogen, it.name, it.flag),
                         raw_autogen);
          return;
        }
      }
    }

    // MODIFIERFLAGS_ANY
    {
      static const struct {
        const std::string name;
      } info[] = {
        "MODIFIERFLAGS_ANY",

        // for backwards compatibility
        "VK_MOD_ANY",
      };
      for (const auto& it : info) {
        if (autogen.find(it.name) != std::string::npos) {
          // Making combination at the first time. (reuse it since 2nd time.)
          static std::vector<std::shared_ptr<std::vector<std::string> > > combination;
          if (combination.empty()) {
            // to reduce combination, we ignore same modifier combination such as (COMMAND_L | COMMAND_R).
            const char* seeds[] = { "MODIFIERFLAG_EITHER_LEFT_OR_RIGHT_COMMAND",
                                    "MODIFIERFLAG_EITHER_LEFT_OR_RIGHT_CONTROL",
                                    "ModifierFlag::FN",
                                    "MODIFIERFLAG_EITHER_LEFT_OR_RIGHT_OPTION",
                                    "MODIFIERFLAG_EITHER_LEFT_OR_RIGHT_SHIFT" };
            pqrs::vector::make_combination(combination, seeds, sizeof(seeds) / sizeof(seeds[0]));
          }

          for (const auto& v : combination) {
            handle_autogen(boost::replace_all_copy(autogen, it.name, boost::join(*v, "|") + "|ModifierFlag::NONE"),
                           raw_autogen);
          }
          return;
        }
      }
    }

    // FROMKEYCODE_HOME, FROMKEYCODE_END, ...
    {
      struct preprocess_info {
        std::string fromkeycode;                   // FROMKEYCODE_HOME
        std::string fromkeycode_with_modifierflag; // FROMKEYCODE_HOME,ModifierFlag::
        std::string fromkeycode_with_comma;        // FROMKEYCODE_HOME,
        std::string keycode;                       // KeyCode::HOME
        std::string other_keycode_with_fn_pipe;    // KeyCode::CURSOR_LEFT,ModifierFlag::FN|
        std::string other_keycode_with_fn;         // KeyCode::CURSOR_LEFT,ModifierFlag::FN
      };
      static std::vector<preprocess_info> info;
      // initialize info
      if (info.empty()) {
        const char* keys[][2] = {
          { "HOME",           "CURSOR_LEFT"  },
          { "END",            "CURSOR_RIGHT" },
          { "PAGEUP",         "CURSOR_UP"    },
          { "PAGEDOWN",       "CURSOR_DOWN"  },
          { "FORWARD_DELETE", "DELETE"       },
        };
        for (const auto& k : keys) {
          preprocess_info i;
          i.fromkeycode                   = std::string("FROMKEYCODE_") + k[0];
          i.fromkeycode_with_modifierflag = std::string("FROMKEYCODE_") + k[0] + ",ModifierFlag::";
          i.fromkeycode_with_comma        = std::string("FROMKEYCODE_") + k[0] + ",";
          i.keycode                       = std::string("KeyCode::") + k[0];
          i.other_keycode_with_fn_pipe    = std::string("KeyCode::") + k[1] + ",ModifierFlag::FN|";
          i.other_keycode_with_fn         = std::string("KeyCode::") + k[1] + ",ModifierFlag::FN";
          info.push_back(i);
        }
      }

      for (const auto& it : info) {
        // FROMKEYCODE_HOME,ModifierFlag::
        if (autogen.find(it.fromkeycode_with_modifierflag) != std::string::npos) {
          // FROMKEYCODE_HOME -> KeyCode::HOME
          handle_autogen(boost::replace_all_copy(autogen, it.fromkeycode, it.keycode),
                         raw_autogen);
          // FROMKEYCODE_HOME, -> KeyCode::CURSOR_LEFT,ModifierFlag::FN|
          handle_autogen(boost::replace_all_copy(autogen, it.fromkeycode_with_comma, it.other_keycode_with_fn_pipe),
                         raw_autogen);
          return;
        }
        // FROMKEYCODE_HOME (without ModifierFlag)
        if (autogen.find(it.fromkeycode) != std::string::npos) {
          // FROMKEYCODE_HOME -> KeyCode::HOME
          handle_autogen(boost::replace_all_copy(autogen, it.fromkeycode, it.keycode),
                         raw_autogen);
          // FROMKEYCODE_HOME -> KeyCode::CURSOR_LEFT,ModifierFlag::FN
          handle_autogen(boost::replace_all_copy(autogen, it.fromkeycode, it.other_keycode_with_fn),
                         raw_autogen);
          return;
        }
      }
    }

    // ------------------------------------------------------------
    // For compatibility
    if (boost::starts_with(autogen, "__KeyOverlaidModifierWithRepeat__")) {
      handle_autogen(boost::replace_first_copy(autogen,
                                               "__KeyOverlaidModifierWithRepeat__",
                                               "__KeyOverlaidModifier__Option::KEYOVERLAIDMODIFIER_REPEAT,"),
                     raw_autogen);
      return;
    }

    if (boost::starts_with(autogen, "__StripModifierFromScrollWheel__")) {
      handle_autogen(boost::replace_first_copy(autogen,
                                               "__StripModifierFromScrollWheel__",
                                               "__ScrollWheelToScrollWheel__") + ",ModifierFlag::NONE",
                     raw_autogen);
      return;
    }

    // ------------------------------------------------------------
    // add to remapclasses_initialize_vector_
    //

    {
      static const std::string symbol("__ShowStatusMessage__");
      if (boost::starts_with(autogen, symbol)) {
        std::string params = autogen.substr(symbol.length());
        boost::trim(params);

        size_t length = params.size();
        remapclasses_initialize_vector_.push_back(static_cast<uint32_t>(length + 1));
        remapclasses_initialize_vector_.push_back(BRIDGE_STATUSMESSAGE);

        for (const auto& c : params) {
          remapclasses_initialize_vector_.push_back(c);
        }
        // no need filter_vector
        return;
      }
    }

    {
      static const std::string symbol("__SimultaneousKeyPresses__");
      if (boost::starts_with(autogen, symbol)) {
        std::string params = autogen.substr(symbol.length());
        std::string newkeycode = std::string("VK_SIMULTANEOUSKEYPRESSES_") +
                                 boost::lexical_cast<std::string>(simultaneous_keycode_index_);
        symbol_map_.add("KeyCode", newkeycode);
        ++simultaneous_keycode_index_;

        params = std::string("KeyCode::") + newkeycode + "," + params;
        add_to_initialize_vector(params, BRIDGE_REMAPTYPE_SIMULTANEOUSKEYPRESSES);
        return;
      }
    }

    static const struct {
      const std::string symbol;
      uint32_t type;
    } info[] = {
      { "__KeyToKey__",                       BRIDGE_REMAPTYPE_KEYTOKEY },
      { "__KeyToConsumer__",                  BRIDGE_REMAPTYPE_KEYTOKEY }, // for backwards compatibility
      { "__KeyToPointingButton__",            BRIDGE_REMAPTYPE_KEYTOKEY }, // for backwards compatibility
      { "__DoublePressModifier__",            BRIDGE_REMAPTYPE_DOUBLEPRESSMODIFIER },
      { "__HoldingKeyToKey__",                BRIDGE_REMAPTYPE_HOLDINGKEYTOKEY },
      { "__IgnoreMultipleSameKeyPress__",     BRIDGE_REMAPTYPE_IGNOREMULTIPLESAMEKEYPRESS },
      { "__KeyOverlaidModifier__",            BRIDGE_REMAPTYPE_KEYOVERLAIDMODIFIER },
      { "__ConsumerToConsumer__",             BRIDGE_REMAPTYPE_KEYTOKEY }, // for backwards compatibility
      { "__ConsumerToKey__",                  BRIDGE_REMAPTYPE_KEYTOKEY }, // for backwards compatibility
      { "__PointingButtonToPointingButton__", BRIDGE_REMAPTYPE_KEYTOKEY }, // for backwards compatibility
      { "__PointingButtonToKey__",            BRIDGE_REMAPTYPE_KEYTOKEY }, // for backwards compatibility
      { "__PointingRelativeToScroll__",       BRIDGE_REMAPTYPE_POINTINGRELATIVETOSCROLL },
      { "__DropKeyAfterRemap__",              BRIDGE_REMAPTYPE_DROPKEYAFTERREMAP },
      { "__SetKeyboardType__",                BRIDGE_REMAPTYPE_SETKEYBOARDTYPE },
      { "__ForceNumLockOn__",                 BRIDGE_REMAPTYPE_FORCENUMLOCKON },
      { "__DropPointingRelativeCursorMove__", BRIDGE_REMAPTYPE_DROPPOINTINGRELATIVECURSORMOVE },
      { "__DropScrollWheel__",                BRIDGE_REMAPTYPE_DROPSCROLLWHEEL },
      { "__FlipPointingRelative__",           BRIDGE_REMAPTYPE_FLIPPOINTINGRELATIVE },
      { "__FlipScrollWheel__",                BRIDGE_REMAPTYPE_FLIPSCROLLWHEEL },
      { "__ScrollWheelToScrollWheel__",       BRIDGE_REMAPTYPE_SCROLLWHEELTOSCROLLWHEEL },
      { "__ScrollWheelToKey__",               BRIDGE_REMAPTYPE_SCROLLWHEELTOKEY },
    };
    for (const auto& it : info) {
      if (boost::starts_with(autogen, it.symbol)) {
        std::string params = autogen.substr(it.symbol.length());
        add_to_initialize_vector(params, it.type);
        return;
      }
    }

    throw xml_compiler_runtime_error(boost::format("Invalid <autogen>:\n"
                                                   "\n"
                                                   "<autogen>%1%</autogen>") %
                                     raw_autogen);
  }

  void
  xml_compiler::remapclasses_initialize_vector_loader::add_to_initialize_vector(std::string& params,
                                                                                uint32_t type) const
  {
    // ------------------------------------------------------------
    // for backwards compatibility
    boost::replace_all(params, "SimultaneousKeyPresses::Option::RAW", "Option::SIMULTANEOUSKEYPRESSES_RAW");
    boost::replace_all(params, "KeyCode::VK_CHANGE_INPUTMODE_", "KeyCode::VK_CHANGE_INPUTSOURCE_");
    // aliases
    boost::replace_all(params, "@begin", ",");
    boost::replace_all(params, "@end", ",Option::SEPARATOR,");

    // ------------------------------------------------------------
    size_t count_index = remapclasses_initialize_vector_.size();
    uint32_t count = 0;

    remapclasses_initialize_vector_.push_back(0); // count

    remapclasses_initialize_vector_.push_back(type);
    ++count;

    size_t value_start_index = remapclasses_initialize_vector_.size();

    pqrs::string::tokenizer tokenizer_comma(params, ',');
    std::string arg;
    std::string value;
    while (tokenizer_comma.split_removing_empty(arg)) {
      unsigned int datatype = 0;
      unsigned int newvalue = 0;

      pqrs::string::tokenizer tokenizer_pipe(arg, '|');
      while (tokenizer_pipe.split_removing_empty(value)) {
        unsigned int newdatatype = xml_compiler_utilities::get_datatype(value);
        if (newdatatype == BRIDGE_DATATYPE_NONE) {
          throw xml_compiler_runtime_error("Unknown symbol:\n\n" + value);
        }

        if (datatype) {
          // There are some connection(|).

          if (newdatatype != BRIDGE_DATATYPE_FLAGS &&
              newdatatype != BRIDGE_DATATYPE_POINTINGBUTTON) {
            // Don't connect no-flags. (Example: KeyCode::A | KeyCode::B)
            throw xml_compiler_runtime_error("Cannot connect(|) except ModifierFlag and PointingButton:\n\n" + arg);
          }

          if (newdatatype != datatype) {
            // Don't connect different data type. (Example: PointingButton::A | ModifierFlag::SHIFT_L)
            throw xml_compiler_runtime_error("Cannot connect(|) between different types:\n\n" + arg);
          }
        }

        datatype = newdatatype;
        newvalue |= symbol_map_.get(value);
      }

      remapclasses_initialize_vector_.push_back(datatype);
      ++count;
      remapclasses_initialize_vector_.push_back(newvalue);
      ++count;

      // Unshift Option::USE_SEPARATOR when Option::SEPARATOR is found.
      if (datatype == BRIDGE_DATATYPE_OPTION &&
          newvalue == symbol_map_cache_option_separator_) {
        remapclasses_initialize_vector_.insert(value_start_index, symbol_map_cache_option_use_separator_);
        ++count;
        remapclasses_initialize_vector_.insert(value_start_index, BRIDGE_DATATYPE_OPTION);
        ++count;
      }
    }

    remapclasses_initialize_vector_.update(count_index, count);

    for (const auto& i : filter_vector_.get()) {
      remapclasses_initialize_vector_.push_back(i);
    }
  }
}
