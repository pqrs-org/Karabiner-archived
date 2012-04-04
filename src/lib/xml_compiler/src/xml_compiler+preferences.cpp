#include <boost/algorithm/string.hpp>
#include "pqrs/xml_compiler.hpp"

namespace pqrs {
  xml_compiler::preferences_node::preferences_node(void) :
    name_line_count_(0)
  {}

  bool
  xml_compiler::preferences_node::handle_name_and_appendix(const boost::property_tree::ptree::value_type& it)
  {
    if (it.first == "name") {
      name_ += boost::trim_copy(it.second.data());
      name_ += "\n";

      ++name_line_count_;

      return true;

    } else if (it.first == "appendix") {
      name_ += "  ";
      name_ += boost::trim_copy(it.second.data());
      name_ += "\n";

      ++name_line_count_;

      return true;
    }

    return false;
  }

  xml_compiler::preferences_checkbox_node::preferences_checkbox_node(void)
  {}

  xml_compiler::preferences_checkbox_node::preferences_checkbox_node(const preferences_checkbox_node& parent) :
    name_for_filter_(parent.name_for_filter_ + " ")
  {}

  bool
  xml_compiler::preferences_checkbox_node::handle_item_child(const boost::property_tree::ptree::value_type& it)
  {
    if (preferences_node::handle_name_and_appendix(it)) {
      name_for_filter_ += boost::algorithm::to_lower_copy(boost::trim_copy(it.second.data()));
      name_for_filter_ += " ";

      return true;
    }
    if (it.first == "identifier") {
      identifier_ = boost::trim_copy(it.second.data());
      return true;
    }
    return false;
  }

  xml_compiler::preferences_number_node::preferences_number_node(void) :
    default_value_(0),
    step_(1)
  {}

  xml_compiler::preferences_number_node::preferences_number_node(const preferences_number_node& /*parent*/) :
    preferences_number_node()
  {}

  bool
  xml_compiler::preferences_number_node::handle_item_child(const boost::property_tree::ptree::value_type& it)
  {
    if (preferences_node::handle_name_and_appendix(it)) {
      return true;
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

      return true;
    }

    return false;
  }

  template <class T>
  void
  xml_compiler::preferences_node_tree<T>::clear(void)
  {
    if (children_) {
      children_->clear();
    }
  }

  template <class T>
  void
  xml_compiler::preferences_node_tree<T>::traverse_item(const boost::property_tree::ptree& pt)
  {
    for (auto& it : pt) {
      if (it.first != "item") {
        traverse_item(it.second);

      } else {
        preferences_node_tree_ptr ptr(new preferences_node_tree(node_));

        for (auto& child : it.second) {
          if ((ptr->node_).handle_item_child(child)) {
            continue;
          }
          ptr->traverse_item(child.second);
        }

        if (! children_) {
          children_ = preferences_node_tree_ptrs_ptr(new preferences_node_tree_ptrs());
        }
        children_->push_back(ptr);
      }
    }
  }

  void
  xml_compiler::reload_preferences_(void)
  {
    preferences_checkbox_node_tree_.clear();
    preferences_number_node_tree_.clear();

    // checkbox
    {
      std::vector<xml_file_path_ptr> xml_file_path_ptrs;
      xml_file_path_ptrs.push_back(
        xml_file_path_ptr(new xml_file_path(xml_file_path::base_directory::private_xml, "private.xml")));
      xml_file_path_ptrs.push_back(
        xml_file_path_ptr(new xml_file_path(xml_file_path::base_directory::system_xml,  "checkbox.xml")));

      std::vector<ptree_ptr> pt_ptrs;
      read_xmls_(pt_ptrs, xml_file_path_ptrs);

      for (auto& pt_ptr : pt_ptrs) {
        preferences_checkbox_node_tree_.traverse_item(*pt_ptr);
      }
    }

    // number
    {
      std::vector<xml_file_path_ptr> xml_file_path_ptrs;
      xml_file_path_ptrs.push_back(
        xml_file_path_ptr(new xml_file_path(xml_file_path::base_directory::system_xml, "number.xml")));

      std::vector<ptree_ptr> pt_ptrs;
      read_xmls_(pt_ptrs, xml_file_path_ptrs);

      for (auto& pt_ptr : pt_ptrs) {
        preferences_number_node_tree_.traverse_item(*pt_ptr);
      }
    }
  }
}
