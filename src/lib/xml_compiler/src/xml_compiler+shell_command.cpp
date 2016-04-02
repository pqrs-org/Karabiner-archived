#include <exception>
#include "pqrs/xml_compiler.hpp"

namespace pqrs {
void xml_compiler::shell_command_loader::traverse(const extracted_ptree& pt) const {
  for (const auto& it : pt) {
    if (it.get_tag_name() != "shellcommanddef") {
      if (!it.children_empty()) {
        traverse(it.children_extracted_ptree());
      }
    } else {
      std::shared_ptr<shell_command> newshell_command(new shell_command());
      if (!newshell_command) continue;

      for (const auto& child : it.children_extracted_ptree()) {
        if (child.get_tag_name() == "name") {
          newshell_command->set_name(pqrs::string::remove_whitespaces_copy(child.get_data()));
        } else if (child.get_tag_name() == "command") {
          newshell_command->set_command(boost::trim_copy(child.get_data()));
        }
      }

      if (!newshell_command->get_name()) {
        xml_compiler_.error_information_.set("No <name> within <shellcommanddef>.");
        continue;
      }
      if (newshell_command->get_name()->empty()) {
        xml_compiler_.error_information_.set("Empty <name> within <shellcommanddef>.");
        continue;
      }

      symbol_map_.add("ShellCommand", *(newshell_command->get_name()));
      shell_command_vector_.push_back(newshell_command);
    }
  }
}
}
