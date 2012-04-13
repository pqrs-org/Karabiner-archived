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
  xml_compiler::app_loader::reload(void) const
  {
    app_vector_.clear();

    std::vector<xml_file_path_ptr> xml_file_path_ptrs;
    xml_file_path_ptrs.push_back(
      xml_file_path_ptr(new xml_file_path(xml_file_path::base_directory::private_xml, "private.xml")));
    xml_file_path_ptrs.push_back(
      xml_file_path_ptr(new xml_file_path(xml_file_path::base_directory::system_xml,  "appdef.xml")));

    std::vector<ptree_ptr> pt_ptrs;
    xml_compiler_.read_xmls_(pt_ptrs, xml_file_path_ptrs);

    for (auto& pt_ptr : pt_ptrs) {
      if (! pt_ptr->empty()) {
        traverse_(*pt_ptr);
      }
    }
  }

  void
  xml_compiler::app_loader::traverse_(const boost::property_tree::ptree& pt) const
  {
    for (auto& it : pt) {
      // extract include
      {
        ptree_ptr pt_ptr;
        xml_compiler_.extract_include_(pt_ptr, it);
        if (pt_ptr) {
          if (! pt_ptr->empty()) {
            traverse_(*pt_ptr);
          }
          continue;
        }
      }

      // ------------------------------------------------------------
      if (it.first != "appdef") {
        if (! it.second.empty()) {
          traverse_(it.second);
        }
      } else {
        std::tr1::shared_ptr<app> newapp(new app());
        if (! newapp) continue;

        for (auto& child : it.second) {
          if (child.first == "appname") {
            newapp->set_name(boost::trim_copy(child.second.data()));
          } else if (child.first == "equal") {
            newapp->add_rule_equal(boost::trim_copy(child.second.data()));
          } else if (child.first == "prefix") {
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
