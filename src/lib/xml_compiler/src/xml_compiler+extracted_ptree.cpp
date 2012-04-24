#include "pqrs/xml_compiler.hpp"

namespace pqrs {
  void
  xml_compiler::extracted_ptree::extracted_ptree_iterator::increment(void)
  {
    if (stack_.empty()) return;

    auto& top = stack_.top();
    if (top.it == top.end) return;

    ++(top.it);
    collapse_();
    extract_include_();
  }

  const xml_compiler::extracted_ptree::node
  xml_compiler::extracted_ptree::extracted_ptree_iterator::dereference(void) const
  {
    if (stack_.empty()) {
      throw xml_compiler_logic_error("stack_.empty() in extracted_ptree_iterator.");
    }

    auto& top = stack_.top();
    if (top.it == top.end) {
      throw xml_compiler_logic_error("it == end in extracted_ptree_iterator.");
    }

    return node(*(top.it), xml_compiler_, top.parent_replacement);
  }

  bool
  xml_compiler::extracted_ptree::extracted_ptree_iterator::equal(const extracted_ptree_iterator const& other) const
  {
    if (stack_.empty() && other.stack_.empty()) {
      return true;
    }
    if (stack_.empty() || other.stack_.empty()) {
      return false;
    }

    auto& top1 = stack_.top();
    auto& top2 = other.stack_.top();
    return top1.it == top2.it;
  }

  void
  xml_compiler::extracted_ptree::extracted_ptree_iterator::extract_include_(void)
  {
    if (stack_.empty()) return;

    auto& top = stack_.top();
    auto& it = *(top.it);

    if (it.first != "include") return;

    // ----------------------------------------
    // replacement
    std::tr1::shared_ptr<pqrs::string::replacement> replacement_ptr(new pqrs::string::replacement);
    if (! it.second.empty()) {
      replacement_loader loader(xml_compiler_, *replacement_ptr);
      loader.traverse(extracted_ptree(xml_compiler_, top.parent_replacement, it.second));
    }

    auto end = replacement_ptr->end();
    for (auto& i : top.parent_replacement) {
      if (replacement_ptr->find(i.first) == end) {
        (*replacement_ptr)[i.first] = i.second;
      }
    }

    // ----------------------------------------
    std::string xml_file_path;
    {
      auto path = it.second.get_optional<std::string>("<xmlattr>.path");
      if (path) {
        xml_file_path = xml_compiler_.make_file_path(xml_compiler_.private_xml_directory_, *path);
      }
    }
    {
      auto path = it.second.get_optional<std::string>("<xmlattr>.system_xml_path");
      if (path) {
        xml_file_path = xml_compiler_.make_file_path(xml_compiler_.system_xml_directory_, *path);
      }
    }

    if (! xml_file_path.empty()) {
      ptree_ptr pt_ptr;
      xml_compiler_.read_xml_(pt_ptr,
                              xml_file_path,
                              *replacement_ptr);

      if (pt_ptr) {
        ++(top.it);
        collapse_();

        if (! pt_ptr->empty()) {
          auto root_node = pt_ptr->begin();
          auto& root_children = root_node->second;
          if (! root_children.empty()) {
            stack_.push(stack_data(pt_ptr, replacement_ptr, root_children));
            extract_include_();
          }
        }
      }
    }
  }

  void
  xml_compiler::extracted_ptree::extracted_ptree_iterator::collapse_(void)
  {
    for (;;) {
      if (stack_.empty()) break;

      auto& top = stack_.top();
      if (top.it != top.end) {
        break;
      }

      stack_.pop();
    }
  }
}
