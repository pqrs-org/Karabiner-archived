#include <boost/algorithm/string.hpp>
#include "pqrs/xml_compiler.hpp"

namespace pqrs {
  bool
  xml_compiler::preferences_node::handle_name_and_appendix(const boost::property_tree::ptree::value_type& it)
  {
    if (it.first == "name") {
      if (! name_.empty()) {
        name_ += "\n";
        ++name_line_count_;
      }
      name_ += boost::trim_copy(it.second.data());

      return true;

    } else if (it.first == "appendix") {
      if (! name_.empty()) {
        name_ += "\n";
        ++name_line_count_;
      }
      name_ += "  ";
      name_ += boost::trim_copy(it.second.data());

      return true;
    }

    return false;
  }

  void
  xml_compiler::preferences_checkbox_node::handle_item_child(const boost::property_tree::ptree::value_type& it)
  {
    if (preferences_node::handle_name_and_appendix(it)) {
      name_for_filter_ += boost::algorithm::to_lower_copy(boost::trim_copy(it.second.data()));
      name_for_filter_ += " ";

    } else if (it.first == "identifier") {
      identifier_ = boost::trim_copy(it.second.data());
    }
  }

  void
  xml_compiler::preferences_number_node::handle_item_child(const boost::property_tree::ptree::value_type& it)
  {
    if (preferences_node::handle_name_and_appendix(it)) {
      return;
    }

    if (it.first == "identifier") {
      identifier_ = boost::trim_copy(it.second.data());

      // default
      {
        auto value = it.second.get_optional<std::string>("<xmlattr>.default");
        if (value) {
          auto v = pqrs::string::to_uint32_t(boost::trim_copy(*value));
          if (v) {
            default_value_ = *v;
          }
        }
      }
      // step
      {
        auto value = it.second.get_optional<std::string>("<xmlattr>.step");
        if (value) {
          auto v = pqrs::string::to_uint32_t(boost::trim_copy(*value));
          if (v) {
            step_ = *v;
          }
        }
      }
      // baseunit
      {
        auto value = it.second.get_optional<std::string>("<xmlattr>.baseunit");
        if (value) {
          base_unit_ = boost::trim_copy(*value);
        }
      }
    }
  }

  template <class T>
  void
  xml_compiler::preferences_node_tree<T>::traverse_item(const boost::property_tree::ptree& pt,
                                                        const xml_compiler& xml_compiler)
  {
    for (auto& it : pt) {
      // extract include
      {
        ptree_ptr pt_ptr;
        xml_compiler.extract_include_(pt_ptr, it);
        if (pt_ptr) {
          if (! pt_ptr->empty()) {
            traverse_item(*pt_ptr, xml_compiler);
          }
          continue;
        }
      }

      // ------------------------------------------------------------
      if (it.first != "item") {
        if (! it.second.empty()) {
          traverse_item(it.second, xml_compiler);
        }

      } else {
        preferences_node_tree_ptr ptr(new preferences_node_tree(node_));

        for (auto& child : it.second) {
          (ptr->node_).handle_item_child(child);
        }
        if (! it.second.empty()) {
          ptr->traverse_item(it.second, xml_compiler);
        }

        if (! children_) {
          children_ = preferences_node_tree_ptrs_ptr(new preferences_node_tree_ptrs());
        }
        children_->push_back(ptr);
      }
    }
  }

  void
  xml_compiler::preferences_node_loader::traverse_checkbox(const boost::property_tree::ptree& pt)
  {
    preferences_checkbox_node_tree_.traverse_item(pt, xml_compiler_);
  }

  void
  xml_compiler::preferences_node_loader::traverse_number(const boost::property_tree::ptree& pt)
  {
    preferences_number_node_tree_.traverse_item(pt, xml_compiler_);
  }
}
