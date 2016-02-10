#include "pqrs/xml_compiler.hpp"

namespace pqrs {
bool xml_compiler::preferences_node::handle_name_and_appendix_(const extracted_ptree::node& it) {
  if (it.get_tag_name() == "name") {
    if (!name_.empty()) {
      name_ += "\n";
    }
    auto s = boost::trim_copy(it.get_data());
    if (!s.empty()) {
      name_ += s;
    }

    // style attribute
    {
      auto attr_style = it.get_optional("<xmlattr>.style");
      if (attr_style) {
        style_ = *attr_style;
        boost::trim(style_);
      }
    }

    return true;

  } else if (it.get_tag_name() == "appendix") {
    if (!name_.empty()) {
      name_ += "\n";
    }
    auto s = boost::trim_copy(it.get_data());
    if (!s.empty()) {
      name_ += "  ";
      name_ += s;
    }

    return true;
  }

  return false;
}

void xml_compiler::preferences_checkbox_node::handle_item_child(const extracted_ptree::node& it) {
  if (preferences_node::handle_name_and_appendix_(it)) {
    // do nothing
  } else if (it.get_tag_name() == "identifier") {
    identifier_ = boost::trim_copy(it.get_data());
  }
}

void xml_compiler::preferences_number_node::handle_item_child(const extracted_ptree::node& it) {
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
