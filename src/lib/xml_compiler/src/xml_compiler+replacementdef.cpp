#include <exception>
#include "pqrs/xml_compiler.hpp"

namespace pqrs {
  void
  xml_compiler::reload_replacementdef_(pqrs::string::replacement& replacement) const
  {
    replacement.clear();

    std::vector<xml_file_path_ptr> xml_file_path_ptrs;
    xml_file_path_ptrs.push_back(
      xml_file_path_ptr(new xml_file_path(xml_file_path::base_directory::private_xml, "private.xml")));
    xml_file_path_ptrs.push_back(
      xml_file_path_ptr(new xml_file_path(xml_file_path::base_directory::system_xml,  "replacementdef.xml")));

    std::vector<ptree_ptr> pt_ptrs;
    read_xmls_(pt_ptrs, xml_file_path_ptrs);

    for (auto& pt_ptr : pt_ptrs) {
      traverse_replacementdef_(*pt_ptr, replacement);
    }
  }

  void
  xml_compiler::traverse_replacementdef_(const boost::property_tree::ptree& pt,
                                         pqrs::string::replacement& replacement) const
  {
    for (auto& it : pt) {
      // extract include
      {
        ptree_ptr pt_ptr;
        extract_include_(pt_ptr, it);
        if (pt_ptr) {
          traverse_replacementdef_(*pt_ptr, replacement);
          continue;
        }
      }

      // ------------------------------------------------------------
      if (it.first != "replacementdef") {
        traverse_replacementdef_(it.second, replacement);
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
          error_information_.set("No <replacementname> within <replacementdef>.");
          continue;
        }
        if (name->empty()) {
          error_information_.set("Empty <replacementname> within <replacementdef>.");
          continue;
        }
        if (name->find_first_of("{{") != std::string::npos ||
            name->find_first_of("}}") != std::string::npos) {
          error_information_.set(std::string("Do not use '{{' and '}}' within <replacementname>:\n\n") + *name);
        }

        if (! value) {
          error_information_.set(std::string("No <replacementvalue> within <replacementdef>:\n\n") + *name);
          continue;
        }

        // --------------------------------------------------
        // Adding to replacement_ if name is not found.
        if (replacement.find(*name) == replacement.end()) {
          replacement[*name] = *value;
        }
      }
    }
  }
}
