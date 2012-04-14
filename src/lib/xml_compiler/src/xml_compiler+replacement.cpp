#include <exception>
#include "pqrs/xml_compiler.hpp"

namespace pqrs {
  void
  xml_compiler::replacement_loader::traverse(const boost::property_tree::ptree& pt) const
  {
    for (auto& it : pt) {
      // extract include
      {
        ptree_ptr pt_ptr;
        xml_compiler_.extract_include_(pt_ptr, it);
        if (pt_ptr) {
          if (! pt_ptr->empty()) {
            traverse(*pt_ptr);
          }
          continue;
        }
      }

      // ------------------------------------------------------------
      if (it.first != "replacementdef") {
        if (! it.second.empty()) {
          traverse(it.second);
        }
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
          xml_compiler_.error_information_.set("No <replacementname> within <replacementdef>.");
          continue;
        }
        if (name->empty()) {
          xml_compiler_.error_information_.set("Empty <replacementname> within <replacementdef>.");
          continue;
        }
        if (name->find_first_of("{{") != std::string::npos ||
            name->find_first_of("}}") != std::string::npos) {
          xml_compiler_.error_information_.set(std::string("Do not use '{{' and '}}' within <replacementname>:\n\n") + *name);
        }

        if (! value) {
          xml_compiler_.error_information_.set(std::string("No <replacementvalue> within <replacementdef>:\n\n") + *name);
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
