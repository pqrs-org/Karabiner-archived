#include <exception>
#include "pqrs/xml_compiler.hpp"
#include "bridge.h"

namespace pqrs {
  bool
  xml_compiler::inputsource::is_rules_matched(const std::string& languagecode,
                                              const std::string& inputsourceid,
                                              const std::string& inputmodeid) const
  {
    for (const auto& r : rules_languagecode_) {
      if (languagecode == r) return true;
    }

    for (const auto& r : rules_inputsourceid_equal_) {
      if (inputsourceid == r) return true;
    }
    for (const auto& r : rules_inputsourceid_prefix_) {
      if (boost::starts_with(inputsourceid, r)) return true;
    }

    for (const auto& r : rules_inputmodeid_equal_) {
      if (inputmodeid == r) return true;
    }
    for (const auto& r : rules_inputmodeid_prefix_) {
      if (boost::starts_with(inputmodeid, r)) return true;
    }

    return false;
  }

  xml_compiler::inputsource_loader::~inputsource_loader(void)
  {
    std::string raw_identifier("system.vk_change_inputsource_definition");
    auto identifier = raw_identifier;
    normalize_identifier_(identifier);

    uint32_t config_index = symbol_map_.add("ConfigIndex", identifier);
    identifier_map_[config_index] = raw_identifier;

    remapclasses_initialize_vector_.start(config_index, raw_identifier);
    {
      for (const auto& it : vk_change_inputsource_map_) {
        remapclasses_initialize_vector_.push_back(2);
        remapclasses_initialize_vector_.push_back(BRIDGE_VK_CHANGE_INPUTSOURCE);
        remapclasses_initialize_vector_.push_back(it.first);
      }
    }
    remapclasses_initialize_vector_.end();
  }

  void
  xml_compiler::inputsource_loader::traverse(const extracted_ptree& pt) const
  {
    for (const auto& it : pt) {
      definition_type::type definition_type = definition_type::none;
      if (it.get_tag_name() == "vkchangeinputsourcedef") {
        definition_type = definition_type::vkchangeinputsourcedef;
      } else if (it.get_tag_name() == "inputsourcedef") {
        definition_type = definition_type::inputsourcedef;
      }

      if (definition_type == definition_type::none) {
        if (! it.children_empty()) {
          traverse(it.children_extracted_ptree());
        }
      } else {
        std::tr1::shared_ptr<inputsource> newinputsource(new inputsource());
        if (! newinputsource) continue;

        // ----------------------------------------
        bool error = false;

        for (const auto& child : it.children_extracted_ptree()) {
          if (child.get_tag_name() == "name") {
            newinputsource->set_name(pqrs::string::remove_whitespaces_copy(child.get_data()));

            if (definition_type == definition_type::vkchangeinputsourcedef) {
              if (! boost::starts_with(*(newinputsource->get_name()), "KeyCode::VK_CHANGE_INPUTSOURCE_")) {
                error = true;
                xml_compiler_.error_information_.set(boost::format("<name> within <vkchangeinputsourcedef> must start with \"KeyCode::VK_CHANGE_INPUTSOURCE_\":\n\n<name>%1%</name>") %
                                                     *(newinputsource->get_name()));
              }
            }

          } else if (child.get_tag_name() == "detail") {
            newinputsource->set_detail(pqrs::string::remove_whitespaces_copy(child.get_data()));

          } else {
            if (child.get_tag_name() == "languagecode") {
              newinputsource->add_rule_languagecode(boost::trim_copy(child.get_data()));
            } else if (child.get_tag_name() == "inputsourceid_equal") {
              newinputsource->add_rule_inputsourceid_equal(boost::trim_copy(child.get_data()));
            } else if (child.get_tag_name() == "inputsourceid_prefix") {
              newinputsource->add_rule_inputsourceid_prefix(boost::trim_copy(child.get_data()));
            } else if (child.get_tag_name() == "inputmodeid_equal") {
              newinputsource->add_rule_inputmodeid_equal(boost::trim_copy(child.get_data()));
            } else if (child.get_tag_name() == "inputmodeid_prefix") {
              newinputsource->add_rule_inputmodeid_prefix(boost::trim_copy(child.get_data()));
            }
          }
        }

        if (error) {
          continue;
        }

        // ----------------------------------------
        // Validation

        // name
        if (! newinputsource->get_name()) {
          xml_compiler_.error_information_.set(boost::format("No <name> within <%1%>.") %
                                               it.get_tag_name());
          continue;
        }

        if (newinputsource->get_name()->empty()) {
          xml_compiler_.error_information_.set(boost::format("Empty <name> within <%1%>.") %
                                               it.get_tag_name());
          continue;
        }

        // detail_ can be empty.

        // ----------------------------------------
        // register to symbol_map_.
        switch (definition_type) {
          case definition_type::none:
            throw xml_compiler_logic_error("invalid definition_type at inputsource_loader::traverse.");

          case definition_type::vkchangeinputsourcedef:
            if (! symbol_map_.get_optional(*(newinputsource->get_name()))) {
              auto keycode = symbol_map_.add("KeyCode",
                                             boost::replace_first_copy(*(newinputsource->get_name()), "KeyCode::", ""));
              vk_change_inputsource_map_[keycode] = newinputsource;
            }
            break;

          case definition_type::inputsourcedef:
            symbol_map_.add("InputSource", *(newinputsource->get_name()));
            if (newinputsource->get_detail()) {
              symbol_map_.add("InputSourceDetail", *(newinputsource->get_detail()));
            } else {
              symbol_map_.add("InputSourceDetail", *(newinputsource->get_name()));
            }
            inputsource_vector_.push_back(newinputsource);

            break;
        }
      }
    }
  }
}
