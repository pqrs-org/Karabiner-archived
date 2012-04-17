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

  const boost::property_tree::ptree::value_type&
  xml_compiler::extracted_ptree::extracted_ptree_iterator::dereference(void) const
  {
    if (stack_.empty()) {
      throw xml_compiler_logic_error("stack_.empty() in extracted_ptree_iterator.");
    }

    auto& top = stack_.top();
    if (top.it == top.end) {
      throw xml_compiler_logic_error("it == end in extracted_ptree_iterator.");
    }

    return *(top.it);
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
    pqrs::string::replacement r;
    if (! it.second.empty()) {
      replacement_loader loader(xml_compiler_, r);
      loader.traverse(it.second);
    }

    for (auto& i : xml_compiler_.replacement_) {
      if (r.find(i.first) == r.end()) {
        r[i.first] = i.second;
      }
    }

    // ----------------------------------------
    ptree_ptr pt_ptr;
    {
      auto path = it.second.get_optional<std::string>("<xmlattr>.path");
      if (path) {
        xml_compiler_.read_xml_(pt_ptr, xml_compiler_.private_xml_directory_, *path, r);
      }
    }
    {
      auto path = it.second.get_optional<std::string>("<xmlattr>.system_xml_path");
      if (path) {
        xml_compiler_.read_xml_(pt_ptr, xml_compiler_.system_xml_directory_, *path, r);
      }
    }

    // ----------------------------------------
    if (pt_ptr) {
      ++(top.it);
      collapse_();

      if (! pt_ptr->empty()) {
        auto root_node = pt_ptr->begin();
        auto& root_children = root_node->second;
        if (! root_children.empty()) {
          stack_.push(stack_data(pt_ptr, root_children));
          extract_include_();
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
