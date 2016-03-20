#include <exception>
#include "pqrs/xml_compiler.hpp"

namespace pqrs {

void xml_compiler::bundle_identifier_override::add_rule_equal(xml_compiler::bundle_identifier_override::rule_target target, const std::string& v) {
  if (target == rule_target::end) return;

  if (!v.empty()) {
    rules_equal_[static_cast<size_t>(target)].push_back(v);
  }
}

void xml_compiler::bundle_identifier_override::add_rule_prefix(xml_compiler::bundle_identifier_override::rule_target target, const std::string& v) {
  if (target == rule_target::end) return;

  if (!v.empty()) {
    rules_prefix_[static_cast<size_t>(target)].push_back(v);
  }
}

void xml_compiler::bundle_identifier_override::add_rule_suffix(xml_compiler::bundle_identifier_override::rule_target target, const std::string& v) {
  if (target == rule_target::end) return;

  if (!v.empty()) {
    rules_suffix_[static_cast<size_t>(target)].push_back(v);
  }
}

void xml_compiler::bundle_identifier_override::add_rule_regex(xml_compiler::bundle_identifier_override::rule_target target, const std::string& v) {
  if (target == rule_target::end) return;

  if (!v.empty()) {
    rules_regex_[static_cast<size_t>(target)].push_back(std::regex(v));
  }
}

bool xml_compiler::bundle_identifier_override::is_rules_matched(rule_target target, const std::string& v) const {
  if (target == rule_target::end) return false;

  // ----------------------------------------
  // If there are no rules, return true.
  if (rules_equal_[static_cast<size_t>(target)].empty() &&
      rules_prefix_[static_cast<size_t>(target)].empty() &&
      rules_suffix_[static_cast<size_t>(target)].empty() &&
      rules_regex_[static_cast<size_t>(target)].empty()) {
    return true;
  }

  // ----------------------------------------
  // Check rules
  for (const auto& r : rules_equal_[static_cast<size_t>(target)]) {
    if (v == r) {
      return true;
    }
  }
  for (const auto& r : rules_prefix_[static_cast<size_t>(target)]) {
    if (boost::starts_with(v, r)) {
      return true;
    }
  }
  for (const auto& r : rules_suffix_[static_cast<size_t>(target)]) {
    if (boost::ends_with(v, r)) {
      return true;
    }
  }
  for (const auto& r : rules_regex_[static_cast<size_t>(target)]) {
    if (regex_search(v.begin(), v.end(), r)) {
      return true;
    }
  }

  return false;
}

bool xml_compiler::bundle_identifier_override::is_rules_matched(const std::string& bundle_identifier,
                                                                const std::string& window_name,
                                                                const std::string& ui_element_role) const {
  return is_rules_matched(rule_target::bundle_identifiers, bundle_identifier) &&
         is_rules_matched(rule_target::window_names, window_name) &&
         is_rules_matched(rule_target::ui_element_roles, ui_element_role);
}

// ============================================================
void xml_compiler::bundle_identifier_override_loader::traverse(const extracted_ptree& pt) const {
  for (const auto& it : pt) {
    if (it.get_tag_name() != "bundleidentifieroverridedef") {
      if (!it.children_empty()) {
        traverse(it.children_extracted_ptree());
      }
    } else {
      std::shared_ptr<bundle_identifier_override> new_bundle_identifier_override(new bundle_identifier_override());
      if (!new_bundle_identifier_override) continue;

      traverse_definition(it.children_extracted_ptree(),
                          new_bundle_identifier_override,
                          bundle_identifier_override::rule_target::end);

      if (!new_bundle_identifier_override->get_new_bundle_identifier()) {
        xml_compiler_.error_information_.set("No <newbundleidentifier> within <bundleidentifieroverridedef>.");
        continue;
      }
      if (new_bundle_identifier_override->get_new_bundle_identifier()->empty()) {
        xml_compiler_.error_information_.set("Empty <newbundleidentifier> within <bundleidentifieroverridedef>.");
        continue;
      }

      bundle_identifier_override_vector_.push_back(new_bundle_identifier_override);
    }
  }
}

void xml_compiler::bundle_identifier_override_loader::traverse_definition(const extracted_ptree& pt,
                                                                          std::shared_ptr<bundle_identifier_override> new_bundle_identifier_override,
                                                                          bundle_identifier_override::rule_target current_rule_target) const {
  for (const auto& it : pt) {
    if (it.get_tag_name() == "newbundleidentifier") {
      new_bundle_identifier_override->set_new_bundle_identifier(boost::trim_copy(it.get_data()));

    } else if (it.get_tag_name() == "bundleidentifiers") {
      traverse_definition(it.children_extracted_ptree(), new_bundle_identifier_override, bundle_identifier_override::rule_target::bundle_identifiers);
    } else if (it.get_tag_name() == "windownames") {
      traverse_definition(it.children_extracted_ptree(), new_bundle_identifier_override, bundle_identifier_override::rule_target::window_names);
    } else if (it.get_tag_name() == "uielementroles") {
      traverse_definition(it.children_extracted_ptree(), new_bundle_identifier_override, bundle_identifier_override::rule_target::ui_element_roles);

    } else if (it.get_tag_name() == "equal") {
      if (current_rule_target == bundle_identifier_override::rule_target::end) {
        xml_compiler_.error_information_.set("Orphan <equal> within <bundleidentifieroverridedef>.");
      } else {
        new_bundle_identifier_override->add_rule_equal(current_rule_target, boost::trim_copy(it.get_data()));
      }
    } else if (it.get_tag_name() == "prefix") {
      if (current_rule_target == bundle_identifier_override::rule_target::end) {
        xml_compiler_.error_information_.set("Orphan <prefix> within <bundleidentifieroverridedef>.");
      } else {
        new_bundle_identifier_override->add_rule_prefix(current_rule_target, boost::trim_copy(it.get_data()));
      }
    } else if (it.get_tag_name() == "suffix") {
      if (current_rule_target == bundle_identifier_override::rule_target::end) {
        xml_compiler_.error_information_.set("Orphan <suffix> within <bundleidentifieroverridedef>.");
      } else {
        new_bundle_identifier_override->add_rule_suffix(current_rule_target, boost::trim_copy(it.get_data()));
      }
    } else if (it.get_tag_name() == "regex") {
      if (current_rule_target == bundle_identifier_override::rule_target::end) {
        xml_compiler_.error_information_.set("Orphan <regex> within <bundleidentifieroverridedef>.");
      } else {
        new_bundle_identifier_override->add_rule_regex(current_rule_target, boost::trim_copy(it.get_data()));
      }
    }
  }
}
}
