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
    if (! value_) return false;

    switch (value_type_) {
      case value_type::none:
        throw xml_compiler_logic_error("language::is_rules_matched is called with value_type::none.");

      case value_type::bcp47:
        return bcp47 == *value_;

      case value_type::inputsourceid_equal:
        return inputsourceid == *value_;

      case value_type::inputsourceid_prefix:
        return boost::starts_with(inputsourceid, *value_);

      case value_type::inputmodeid_equal:
        return inputmodeid == *value_;

      case value_type::inputmodeid_prefix:
        return boost::starts_with(inputmodeid, *value_);
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
      if (it.get_tag_name() != "vkchangeinputsourcedef") {
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

            if (! boost::starts_with(*(newlanguage->get_name()), "VK_CHANGE_INPUTSOURCE_")) {
              error = true;
              xml_compiler_.error_information_.set(boost::format("<name> within <vkchangeinputsourcedef> must start with \"VK_CHANGE_INPUTSOURCE_\"\n\n<name>%1%</name>") %
                                                   *(newlanguage->get_name()));
            }

          } else {
            language::value_type::type value_type = language::value_type::get_type_from_string(child.get_tag_name());
            if (value_type != language::value_type::none) {
              bool value_is_not_none = newlanguage->get_value();
              newlanguage->set_type(value_type);
              newlanguage->set_value(boost::trim_copy(child.get_data()));

              if (value_is_not_none) {
                error = true;
                xml_compiler_.error_information_.set(boost::format("<vkchangeinputsourcedef> must not have multiple values.\n\n<%1%>%2%</%3%>") %
                                                     child.get_tag_name() %
                                                     *(newlanguage->get_value()) %
                                                     child.get_tag_name());
              }
            }
          }
        }

        if (error) {
          continue;
        }

        // ----------------------------------------
        // Validation
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

        if (newlanguage->get_type() == language::value_type::none ||
            ! newlanguage->get_value()) {
          xml_compiler_.error_information_.set(boost::format("No value definition within <%1%>.") %
                                               it.get_tag_name());
          continue;
        }

        if (newlanguage->get_value() && newlanguage->get_value()->empty()) {
          xml_compiler_.error_information_.set(boost::format("Empty value definition within <%1%>.") %
                                               it.get_tag_name());
          continue;
        }

        if (it.get_tag_name() == "vkchangeinputsourcedef") {
          if (! symbol_map_.get_optional(std::string("KeyCode::") + *(newlanguage->get_name()))) {
            auto keycode = symbol_map_.add("KeyCode", *(newlanguage->get_name()));
            vk_change_inputsource_map_[keycode] = newlanguage;
          }
        }
      }
    }
  }
}
