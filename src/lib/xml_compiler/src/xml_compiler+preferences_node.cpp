#include "pqrs/xml_compiler.hpp"

namespace pqrs {
  bool
  xml_compiler::preferences_node::handle_name_and_appendix_(const extracted_ptree::node& it)
  {
    if (it.get_tag_name() == "name") {
      if (! name_.empty()) {
        name_ += "\n";
        ++name_line_count_;
      }
      name_ += boost::trim_copy(it.get_data());

      // style attribute
      {
        auto attr_style = it.get_optional("<xmlattr>.style");
        if (attr_style) {
          style_ = *attr_style;
          boost::trim(style_);
        }
      }
      // indent
      {
        auto value = it.get_optional("<xmlattr>.indent");
        if (value) {
          auto v = pqrs::string::to_uint32_t(boost::trim_copy(*value));
          if (v) {
            indent_ = *v;
          }
        }
      }

      return true;

    } else if (it.get_tag_name() == "appendix") {
      if (! name_.empty()) {
        name_ += "\n";
        ++name_line_count_;
      }
      name_ += "  ";
      name_ += boost::trim_copy(it.get_data());

      return true;
    }

    return false;
  }

  void
  xml_compiler::preferences_checkbox_node::handle_item_child(const extracted_ptree::node& it)
  {
    if (preferences_node::handle_name_and_appendix_(it)) {
      name_for_filter_ += boost::algorithm::to_lower_copy(boost::trim_copy(it.get_data()));
      name_for_filter_ += " ";

    } else if (it.get_tag_name() == "identifier") {
      identifier_ = boost::trim_copy(it.get_data());
    }
  }

  void
  xml_compiler::preferences_number_node::handle_item_child(const extracted_ptree::node& it)
  {
    if (preferences_node::handle_name_and_appendix_(it)) {
      return;
    }

    if (it.get_tag_name() == "identifier") {
      identifier_ = boost::trim_copy(it.get_data());

      // default
      {
        auto value = it.get_optional("<xmlattr>.default");
        if (value) {
          auto v = pqrs::string::to_uint32_t(boost::trim_copy(*value));
          if (v) {
            default_value_ = *v;
          }
        }
      }
      // step
      {
        auto value = it.get_optional("<xmlattr>.step");
        if (value) {
          auto v = pqrs::string::to_uint32_t(boost::trim_copy(*value));
          if (v) {
            step_ = *v;
          }
        }
      }
      // baseunit
      {
        auto value = it.get_optional("<xmlattr>.baseunit");
        if (value) {
          base_unit_ = boost::trim_copy(*value);
        }
      }
    }
  }
}
