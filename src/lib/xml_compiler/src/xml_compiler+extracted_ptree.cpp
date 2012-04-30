#include "pqrs/xml_compiler.hpp"

namespace pqrs {
  void
  xml_compiler::extracted_ptree::extracted_ptree_iterator::increment(void)
  {
    if (ended_()) return;

    auto& top = extracted_ptree_.stack_.top();
    if (top.it == top.end) return;

    ++(top.it);
    collapse_();
    extract_include_();
  }

  const xml_compiler::extracted_ptree::node
  xml_compiler::extracted_ptree::extracted_ptree_iterator::dereference(void) const
  {
    if (ended_()) {
      throw xml_compiler_logic_error("extracted_ptree_iterator::dereference is called with invalid iterator.");
    }

    auto& top = extracted_ptree_.stack_.top();
    if (top.it == top.end) {
      throw xml_compiler_logic_error("it == end in extracted_ptree_iterator::dereference.");
    }

    return node(*(top.it), extracted_ptree_, top.parent_replacement);
  }

  void
  xml_compiler::extracted_ptree::extracted_ptree_iterator::extract_include_(void)
  {
    if (ended_()) return;

    auto& xml_compiler = extracted_ptree_.xml_compiler_;

    auto& top = extracted_ptree_.stack_.top();
    auto& it = *(top.it);

    if (it.first != "include") return;

    // ----------------------------------------
    // replacement
    std::tr1::shared_ptr<pqrs::string::replacement> replacement_ptr(new pqrs::string::replacement);
    if (! it.second.empty()) {
      replacement_loader loader(xml_compiler, *replacement_ptr);
      loader.traverse(extracted_ptree(extracted_ptree_, top.parent_replacement, it.second));
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
        if (extracted_ptree_.included_files_.empty()) {
          throw xml_compiler_logic_error("included_files_.empty() in extracted_ptree_iterator::extract_include_.");
        }
        xml_file_path = xml_compiler.make_file_path(pqrs::file_path::dirname(extracted_ptree_.included_files_.back()),
                                                    *path);
      }
    }

    if (! xml_file_path.empty()) {
      for (auto& i : extracted_ptree_.included_files_) {
        if (i == xml_file_path) {
          xml_compiler.error_information_.set("An infinite include loop is detected:\n" + xml_file_path);
          return;
        }
      }

      ptree_ptr pt_ptr;
      xml_compiler.read_xml_(pt_ptr,
                             xml_file_path,
                             *replacement_ptr);

      if (pt_ptr) {
        // Skip <include> next time.
        ++(top.it);
        // Do not call collapse_ here.
        // (Keep included_files_ to detect an infinite include loop.)

        if (! pt_ptr->empty()) {
          auto root_node = pt_ptr->begin();
          auto& root_children = root_node->second;
          if (! root_children.empty()) {
            extracted_ptree_.stack_.push(stack_data(pt_ptr, replacement_ptr, root_children));
            extracted_ptree_.included_files_.push_back(xml_file_path);
            extract_include_();
          }
        }

        collapse_();
      }
    }
  }

  void
  xml_compiler::extracted_ptree::extracted_ptree_iterator::collapse_(void)
  {
    for (;;) {
      if (ended_()) break;

      auto& top = extracted_ptree_.stack_.top();
      if (top.it != top.end) {
        break;
      }

      if (top.extracted()) {
        if (extracted_ptree_.included_files_.empty()) {
          throw xml_compiler_logic_error("included_files_.empty() in extracted_ptree_iterator::collapse_.");
        }
        extracted_ptree_.included_files_.pop_back();
      }
      extracted_ptree_.stack_.pop();
    }
  }
}
