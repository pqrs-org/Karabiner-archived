#include <exception>
#include <boost/algorithm/string.hpp>
#include "pqrs/xml_compiler.hpp"
#include "bridge.h"

namespace pqrs {
  bool
  xml_compiler::language::is_rules_matched(const std::string& bcp47,
                                           const std::string& inputsourceid,
                                           const std::string& inputmodeid) const
  {
    for (auto& r : rules_bcp47_) {
      if (bcp47 == r) return true;
    }

    for (auto& r : rules_inputsourceid_equal_) {
      if (inputsourceid == r) return true;
    }
    for (auto& r : rules_inputsourceid_prefix_) {
      if (boost::starts_with(inputsourceid, r)) return true;
    }

    for (auto& r : rules_inputmodeid_equal_) {
      if (inputmodeid == r) return true;
    }
    for (auto& r : rules_inputmodeid_prefix_) {
      if (boost::starts_with(inputmodeid, r)) return true;
    }

    return false;
  }

  xml_compiler::language_loader::~language_loader(void)
  {
    std::string raw_identifier("system.vk_change_inputsource_definition");
    auto identifier = raw_identifier;
    normalize_identifier_(identifier);

    uint32_t config_index = symbol_map_.add("ConfigIndex", identifier);
    identifier_map_[config_index] = raw_identifier;

    remapclasses_initialize_vector_.start(config_index, raw_identifier);
    {
      for (auto& it : vk_change_inputsource_map_) {
        remapclasses_initialize_vector_.push_back(2);
        remapclasses_initialize_vector_.push_back(BRIDGE_VK_CHANGE_INPUTSOURCE);
        remapclasses_initialize_vector_.push_back(it.first);
      }
    }
    remapclasses_initialize_vector_.end();
  }

  void
  xml_compiler::language_loader::traverse(const extracted_ptree& pt) const
  {
    for (auto& it : pt) {
      definition_type::type definition_type = definition_type::none;
      if (it.get_tag_name() == "vkchangeinputsourcedef") {
        definition_type = definition_type::vkchangeinputsourcedef;
      } else if (it.get_tag_name() == "languagedef") {
        definition_type = definition_type::languagedef;
      }

      if (definition_type == definition_type::none) {
        if (! it.children_empty()) {
          traverse(it.children_extracted_ptree());
        }
      } else {
        std::tr1::shared_ptr<language> newlanguage(new language());
        if (! newlanguage) continue;

        // ----------------------------------------
        bool error = false;

        for (auto& child : it.children_extracted_ptree()) {
          if (child.get_tag_name() == "name") {
            newlanguage->set_name(pqrs::string::remove_whitespaces_copy(child.get_data()));

            if (definition_type == definition_type::vkchangeinputsourcedef) {
              if (! boost::starts_with(*(newlanguage->get_name()), "KeyCode::VK_CHANGE_INPUTSOURCE_")) {
                error = true;
                xml_compiler_.error_information_.set(boost::format("<name> within <vkchangeinputsourcedef> must start with \"KeyCode::VK_CHANGE_INPUTSOURCE_\":\n\n<name>%1%</name>") %
                                                     *(newlanguage->get_name()));
              }
            }

          } else if (child.get_tag_name() == "detail") {
            newlanguage->set_detail(pqrs::string::remove_whitespaces_copy(child.get_data()));

          } else {
            if (child.get_tag_name() == "bcp47") {
              newlanguage->add_rule_bcp47(boost::trim_copy(child.get_data()));
            } else if (child.get_tag_name() == "inputsourceid_equal") {
              newlanguage->add_rule_inputsourceid_equal(boost::trim_copy(child.get_data()));
            } else if (child.get_tag_name() == "inputsourceid_prefix") {
              newlanguage->add_rule_inputsourceid_prefix(boost::trim_copy(child.get_data()));
            } else if (child.get_tag_name() == "inputmodeid_equal") {
              newlanguage->add_rule_inputmodeid_equal(boost::trim_copy(child.get_data()));
            } else if (child.get_tag_name() == "inputmodeid_prefix") {
              newlanguage->add_rule_inputmodeid_prefix(boost::trim_copy(child.get_data()));
            }
          }
        }

        if (error) {
          continue;
        }

        // ----------------------------------------
        // Validation

        // name
        if (! newlanguage->get_name()) {
          xml_compiler_.error_information_.set(boost::format("No <name> within <%1%>.") %
                                               it.get_tag_name());
          continue;
        }

        if (newlanguage->get_name()->empty()) {
          xml_compiler_.error_information_.set(boost::format("Empty <name> within <%1%>.") %
                                               it.get_tag_name());
          continue;
        }

        // detail_ can be empty.

        // ----------------------------------------
        // register to symbol_map_.
        switch (definition_type) {
          case definition_type::none:
            throw xml_compiler_logic_error("invalid definition_type at language_loader::traverse.");

          case definition_type::vkchangeinputsourcedef:
            if (! symbol_map_.get_optional(*(newlanguage->get_name()))) {
              auto keycode = symbol_map_.add("KeyCode",
                                             boost::replace_first_copy(*(newlanguage->get_name()), "KeyCode::", ""));
              vk_change_inputsource_map_[keycode] = newlanguage;
            }
            break;

          case definition_type::languagedef:
            symbol_map_.add("Language", *(newlanguage->get_name()));
            if (newlanguage->get_detail()) {
              symbol_map_.add("LanguageDetail", *(newlanguage->get_detail()));
            } else {
              symbol_map_.add("LanguageDetail", *(newlanguage->get_name()));
            }
            language_vector_.push_back(newlanguage);

            break;
        }
      }
    }
  }
}
