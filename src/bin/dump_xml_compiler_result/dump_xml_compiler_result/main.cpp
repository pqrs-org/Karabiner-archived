#include <iostream>
#include <boost/algorithm/string.hpp>
#include "pqrs/xml_compiler.hpp"

namespace {
  int total_identifier_count_ = 0;

  void
  escapeHTML(std::string& string)
  {
    boost::replace_all(string, "&", "&amp;");
    boost::replace_all(string, "<", "&lt;");
    boost::replace_all(string, ">", "&gt;");
  }

  void
  dump_tree(const pqrs::xml_compiler::preferences_node_tree<pqrs::xml_compiler::preferences_checkbox_node>& node_tree,
            bool dump_all)
  {
    auto children_ptr = node_tree.get_children();
    if (children_ptr) {
      auto& children = *children_ptr;

      std::cout << "<ul>" << std::endl;

      for (auto& it : children) {
        auto& node = it->get_node();

        std::string name = node.get_name();
        escapeHTML(name);
        std::cout << "<li>" << name << "</li>" << std::endl;

        if (dump_all) {
          std::cout << "<identifier>" << node.get_identifier() << "</identifier>" << std::endl;
          std::cout << "<name_for_filter>" << node.get_name_for_filter() << "</name_for_filter>" << std::endl;
        }

        dump_tree(*it, dump_all);

        auto& identifier = node.get_identifier();
        if (! identifier.empty()) {
          ++total_identifier_count_;
        }
      }

      std::cout << "</ul>" << std::endl;
    }
  }

  void
  dump_number(const pqrs::xml_compiler::preferences_node_tree<pqrs::xml_compiler::preferences_number_node>& node_tree)
  {
    auto children_ptr = node_tree.get_children();
    if (children_ptr) {
      auto& children = *children_ptr;

      for (auto& it : children) {
        auto& node = it->get_node();

        std::cout << node.get_name() << std::endl
                  << "  default_value:" << node.get_default_value() << std::endl
                  << "           step:" << node.get_step() << std::endl
                  << "      base_unit:" << node.get_base_unit() << std::endl
                  << std::endl;

        dump_number(*it);
      }
    }
  }
}

int
main(int argc, const char* argv[])
{
  if (argc != 4) {
    std::cerr << "Usage: " << argv[0] << " system_xml_directory private_xml_directory command" << std::endl
              << std::endl
              << "Example: " << argv[0]
              << " /Library/org.pqrs/KeyRemap4MacBook/app/KeyRemap4MacBook.app/Contents/Resources"
              << " ~/Library/Application\\ Support/KeyRemap4MacBook"
              << " dump_data"
              << std::endl;
    exit(1);
  }

  pqrs::xml_compiler xml_compiler(argv[1], argv[2]);
  xml_compiler.reload();

  std::string command(argv[3]);

  if (command == "dump_data") {
    auto v = xml_compiler.get_remapclasses_initialize_vector().get();
    for (auto& it : v) {
      std::cout << it << std::endl;
    }

  } else if (command == "dump_tree") {
    dump_tree(xml_compiler.get_preferences_checkbox_node_tree(), false);
    std::cout << "Total items: " << total_identifier_count_ << std::endl;

  } else if (command == "dump_tree_all") {
    dump_tree(xml_compiler.get_preferences_checkbox_node_tree(), true);
    std::cout << "Total items: " << total_identifier_count_ << std::endl;

  } else if (command == "dump_number") {
    dump_number(xml_compiler.get_preferences_number_node_tree());

  } else if (command == "dump_identifier_except_essential") {
    for (int i = 0;; ++i) {
      auto identifier = xml_compiler.get_identifier(i);
      if (! identifier) break;

      std::cout << *identifier << std::endl;
    }

  } else if (command == "output_bridge_essential_configuration_enum_h") {
    std::cout << "enum {" << std::endl;

    for (size_t i = 0;; ++i) {
      auto essential_configuration = xml_compiler.get_essential_configuration(i);
      if (! essential_configuration) {
        std::cout << "BRIDGE_ESSENTIAL_CONFIG_INDEX__END__ = " << i << std::endl;
        break;
      }

      std::cout << "BRIDGE_ESSENTIAL_CONFIG_INDEX_" << essential_configuration->get_identifier()
                << " = " << i << ","
                << std::endl;
    }

    std::cout << "};" << std::endl;

  } else if (command == "output_bridge_essential_configuration_default_values_c") {
    for (size_t i = 0;; ++i) {
      auto essential_configuration = xml_compiler.get_essential_configuration(i);
      if (! essential_configuration) {
        break;
      }
      std::cout << essential_configuration->get_default_value()
                << ","
                << std::endl;
    }

  } else if (command == "output_bridge_essential_configuration_identifiers_m") {
    for (size_t i = 0;; ++i) {
      auto essential_configuration = xml_compiler.get_essential_configuration(i);
      if (! essential_configuration) {
        std::cout << "nil" << std::endl;
        break;
      }

      std::cout << "@\"" << essential_configuration->get_raw_identifier() << "\""
                << ","
                << std::endl;
    }
  }

  return 0;
}
