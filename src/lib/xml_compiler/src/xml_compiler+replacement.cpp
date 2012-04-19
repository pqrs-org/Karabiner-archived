#include <exception>
#include "pqrs/xml_compiler.hpp"

namespace pqrs {
  void
  xml_compiler::replacement_loader::traverse(const extracted_ptree& pt) const
  {
    for (auto& it : pt) {
      if (it.get_tag_name() != "replacementdef") {
        if (! it.children_empty()) {
          traverse(it.children_extracted_ptree());
        }
      } else {
        boost::optional<std::string> name;
        boost::optional<std::string> value;
        for (auto& child : it.children_extracted_ptree()) {
          if (child.get_tag_name() == "replacementname") {
            name = child.get_data();
          } else if (child.get_tag_name() == "replacementvalue") {
            value = child.get_data();
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
