#include <exception>
#include <boost/algorithm/string.hpp>
#include "pqrs/xml_compiler.hpp"

namespace pqrs {

  void
  xml_compiler::app::add_rule_equal(const std::string& v)
  {
    if (! v.empty()) {
      rules_equal_.push_back(v);
    }
  }

  void
  xml_compiler::app::add_rule_prefix(const std::string& v)
  {
    if (! v.empty()) {
      rules_prefix_.push_back(v);
    }
  }

  bool
  xml_compiler::app::is_rules_matched(const std::string& identifier) const
  {
    for (auto& r : rules_equal_) {
      if (identifier == r) {
        return true;
      }
    }
    for (auto& r : rules_prefix_) {
      if (boost::starts_with(identifier, r)) {
        return true;
      }
    }

    return false;
  }

  // ============================================================
  void
  xml_compiler::app_loader::traverse(const extracted_ptree& pt) const
  {
    for (auto& it : pt) {
      if (it.get_tag_name() != "appdef") {
        if (! it.second.empty()) {
          traverse(it.children_extracted_ptree());
        }
      } else {
        std::tr1::shared_ptr<app> newapp(new app());
        if (! newapp) continue;

        for (auto& child : it.children_extracted_ptree()) {
          if (child.get_tag_name() == "appname") {
            newapp->set_name(boost::trim_copy(child.second.data()));
          } else if (child.get_tag_name() == "equal") {
            newapp->add_rule_equal(boost::trim_copy(child.second.data()));
          } else if (child.get_tag_name() == "prefix") {
            newapp->add_rule_prefix(boost::trim_copy(child.second.data()));
          }
        }

        if (! newapp->get_name()) {
          xml_compiler_.error_information_.set("No <appname> within <appdef>.");
          continue;
        }
        if (newapp->get_name()->empty()) {
          xml_compiler_.error_information_.set("Empty <appname> within <appdef>.");
          continue;
        }

        symbol_map_.add("ApplicationType", *(newapp->get_name()));
        app_vector_.push_back(newapp);
      }
    }
  }
}
