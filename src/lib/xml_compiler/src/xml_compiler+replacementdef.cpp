#include <exception>
#include "pqrs/xml_compiler.hpp"

namespace pqrs {
  void
  xml_compiler::reload_replacementdef_(void)
  {
    replacement_.clear();

    std::vector<xml_file_path_ptr> xml_file_path_ptrs;
    xml_file_path_ptrs.push_back(
      xml_file_path_ptr(new xml_file_path(xml_file_path::base_directory::private_xml, "private.xml")));
    xml_file_path_ptrs.push_back(
      xml_file_path_ptr(new xml_file_path(xml_file_path::base_directory::system_xml,  "replacementdef.xml")));

    std::vector<ptree_ptr> pt_ptrs;
    read_xmls_(pt_ptrs, xml_file_path_ptrs);

    for (auto& pt_ptr : pt_ptrs) {
      traverse_replacementdef_(*pt_ptr);
    }
  }

  void
  xml_compiler::traverse_replacementdef_(const boost::property_tree::ptree& pt)
  {
    for (auto& it : pt) {
      if (it.first != "replacementdef") {
        traverse_replacementdef_(it.second);
      } else {
        boost::optional<std::string> name;
        boost::optional<std::string> value;
        for (auto& child : it.second) {
          if (child.first == "replacementname") {
            name = child.second.data();
          } else if (child.first == "replacementvalue") {
            value = child.second.data();
          }
        }

        // --------------------------------------------------
        // Validation
        if (! name) {
          set_error_message_("No <replacementname> within <replacementdef>.");
          continue;
        }
        if (name->empty()) {
          set_error_message_("Empty <replacementname> within <replacementdef>.");
          continue;
        }
        if (name->find_first_of("{{") != std::string::npos ||
            name->find_first_of("}}") != std::string::npos) {
          set_error_message_(std::string("Do not use '{{' and '}}' within <replacementname>:\n\n") + *name);
        }

        if (! value) {
          set_error_message_(std::string("No <replacementvalue> within <replacementdef>:\n\n") + *name);
          continue;
        }

        // --------------------------------------------------
        // Adding to replacement_ if name is not found.
        if (replacement_.find(*name) == replacement_.end()) {
          replacement_[*name] = *value;
        }
      }
    }
  }
}
