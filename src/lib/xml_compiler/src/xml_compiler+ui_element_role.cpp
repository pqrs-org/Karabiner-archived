#include <exception>
#include "pqrs/xml_compiler.hpp"

namespace pqrs {
void xml_compiler::ui_element_role_loader::traverse(const extracted_ptree& pt) const {
  for (const auto& it : pt) {
    if (it.get_tag_name() != "uielementroledef") {
      if (!it.children_empty()) {
        traverse(it.children_extracted_ptree());
      }
    } else {
      std::string name = (pqrs::string::remove_whitespaces_copy(it.get_data()));

      if (name.empty()) {
        xml_compiler_.error_information_.set("Empty <uielementroledef>.");
        continue;
      }

      symbol_map_.add("UIElementRole", name);
    }
  }
}
}
