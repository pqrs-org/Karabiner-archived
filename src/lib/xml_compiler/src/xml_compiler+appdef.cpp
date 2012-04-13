#include <exception>
#include <boost/algorithm/string.hpp>
#include "pqrs/xml_compiler.hpp"

namespace pqrs {
  void
  xml_compiler::reload_appdef_(symbol_map& symbol_map,
                               std::vector<std::tr1::shared_ptr<appdef> >& app) const
  {
    app.clear();

    std::vector<xml_file_path_ptr> xml_file_path_ptrs;
    xml_file_path_ptrs.push_back(
      xml_file_path_ptr(new xml_file_path(xml_file_path::base_directory::private_xml, "private.xml")));
    xml_file_path_ptrs.push_back(
      xml_file_path_ptr(new xml_file_path(xml_file_path::base_directory::system_xml,  "appdef.xml")));

    std::vector<ptree_ptr> pt_ptrs;
    read_xmls_(pt_ptrs, xml_file_path_ptrs);

    for (auto& pt_ptr : pt_ptrs) {
      traverse_appdef_(*pt_ptr, symbol_map, app);
    }
  }

  void
  xml_compiler::traverse_appdef_(const boost::property_tree::ptree& pt,
                                 symbol_map& symbol_map,
                                 std::vector<std::tr1::shared_ptr<appdef> >& app) const
  {
    for (auto& it : pt) {
      // extract include
      {
        ptree_ptr pt_ptr;
        extract_include_(pt_ptr, it);
        if (pt_ptr) {
          traverse_appdef_(*pt_ptr, symbol_map, app);
          continue;
        }
      }

      // ------------------------------------------------------------
      if (it.first != "appdef") {
        traverse_appdef_(it.second, symbol_map, app);
      } else {
        std::tr1::shared_ptr<appdef> newappdef(new appdef());
        if (! newappdef) continue;

        for (auto& child : it.second) {
          if (child.first == "appname") {
            newappdef->set_name(boost::trim_copy(child.second.data()));
          } else if (child.first == "equal") {
            newappdef->add_rule_equal(boost::trim_copy(child.second.data()));
          } else if (child.first == "prefix") {
            newappdef->add_rule_prefix(boost::trim_copy(child.second.data()));
          }
        }

        if (! newappdef->get_name()) {
          error_information_.set("No <appname> within <appdef>.");
          continue;
        }
        if (newappdef->get_name()->empty()) {
          error_information_.set("Empty <appname> within <appdef>.");
          continue;
        }

        symbol_map.add("ApplicationType", *(newappdef->get_name()));
        app.push_back(newappdef);
      }
    }
  }

  void
  xml_compiler::appdef::add_rule_equal(const std::string& v)
  {
    if (! v.empty()) {
      rules_equal_.push_back(v);
    }
  }

  void
  xml_compiler::appdef::add_rule_prefix(const std::string& v)
  {
    if (! v.empty()) {
      rules_prefix_.push_back(v);
    }
  }

  bool
  xml_compiler::appdef::is_rules_matched(const std::string& identifier) const
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
}
