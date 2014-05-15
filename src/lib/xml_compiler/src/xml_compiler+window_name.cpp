#include <exception>
#include "pqrs/xml_compiler.hpp"

namespace pqrs {

  void
  xml_compiler::window_name::add_rule_regex(const std::string& regex)
  {
    if (! regex.empty()) {
      rules_regex_.push_back(std::regex(regex));
    }
  }

  bool
  xml_compiler::window_name::is_rules_matched(const std::string& window_name) const
  {
    for (const auto& r : rules_regex_) {
      if (regex_search(window_name.begin(), window_name.end(), r)) {
        return true;
      }
    }
    return false;
  }

  // ============================================================
  void
  xml_compiler::window_name_loader::traverse(const extracted_ptree& pt) const
  {
    for (const auto& it : pt) {
      if (it.get_tag_name() != "windownamedef") {
        if (! it.children_empty()) {
          traverse(it.children_extracted_ptree());
        }
      } else {
        std::shared_ptr<window_name> newwindow_name(new window_name());
        if (! newwindow_name) continue;

        for (const auto& child : it.children_extracted_ptree()) {
          if (child.get_tag_name() == "name") {
            newwindow_name->set_name(pqrs::string::remove_whitespaces_copy(child.get_data()));
          } else if (child.get_tag_name() == "regex") {
            newwindow_name->add_rule_regex(boost::trim_copy(child.get_data()));
          }
        }

        if (! newwindow_name->get_name()) {
          xml_compiler_.error_information_.set("No <name> within <windownamedef>.");
          continue;
        }
        if (newwindow_name->get_name()->empty()) {
          xml_compiler_.error_information_.set("Empty <name> within <windownamedef>.");
          continue;
        }

        symbol_map_.add("WindowName", *(newwindow_name->get_name()));
        window_name_vector_.push_back(newwindow_name);
      }
    }
  }
}
