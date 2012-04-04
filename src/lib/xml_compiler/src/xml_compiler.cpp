#include <exception>
#include <iostream>
#include <sstream>
#include <boost/algorithm/string.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include "pqrs/string.hpp"
#include "pqrs/xml_compiler.hpp"

namespace pqrs {
  xml_compiler::xml_compiler(const std::string& system_xml_directory, const std::string& private_xml_directory) :
    system_xml_directory_(system_xml_directory),
    private_xml_directory_(private_xml_directory),
    error_count_(0)
  {}

  void
  xml_compiler::reload(void)
  {
    error_message_.clear();
    error_count_ = 0;

    reload_replacementdef_();
    reload_symbol_map_();
    reload_appdef_();
    reload_devicedef_();
    reload_autogen_();
    reload_preferences_();
  }

  const xml_compiler::remapclasses_initialize_vector&
  xml_compiler::get_remapclasses_initialize_vector(void) const
  {
    return remapclasses_initialize_vector_;
  }

  const std::string&
  xml_compiler::get_error_message(void) const
  {
    return error_message_;
  }

  int
  xml_compiler::get_error_count(void) const
  {
    return error_count_;
  }

  boost::optional<uint32_t>
  xml_compiler::get_symbol_map_value(const std::string& name) const
  {
    return symbol_map_.get_optional(name);
  }

  void
  xml_compiler::dump_symbol_map(void) const
  {
    symbol_map_.dump();
  }

  void
  xml_compiler::read_xmls_(std::vector<ptree_ptr>& pt_ptrs, const std::vector<xml_file_path_ptr>& xml_file_path_ptrs)
  {
    pt_ptrs.clear();

    for (auto& path_ptr : xml_file_path_ptrs) {
      try {
        ptree_ptr pt_ptr(new boost::property_tree::ptree());

        std::string path;
        switch (path_ptr->get_base_directory()) {
          case xml_file_path::base_directory::system_xml:
            path += system_xml_directory_;
            break;
          case xml_file_path::base_directory::private_xml:
            path += private_xml_directory_;
            break;
        }
        path += "/";
        path += path_ptr->get_relative_path();

        int flags = boost::property_tree::xml_parser::no_comments;

        std::string xml;
        if (replacement_.empty()) {
          pqrs::string::string_from_file(xml, path.c_str());
        } else {
          pqrs::string::string_by_replacing_double_curly_braces_from_file(xml, path.c_str(), replacement_);
        }
        std::stringstream istream(xml, std::stringstream::in);
        boost::property_tree::read_xml(istream, *pt_ptr, flags);

        pt_ptrs.push_back(pt_ptr);

      } catch (std::exception& e) {
        std::string what = e.what();

        // Hack:
        // boost::property_tree::read_xml throw exception with filename.
        // But, when we call read_xml with stream, the filename becomes "unspecified file" as follow.
        //
        // <unspecified file>(4): expected element name
        //
        // So, we change "unspecified file" to file name by ourself.
        boost::replace_first(what, "<unspecified file>", std::string("<") + path_ptr->get_relative_path() + ">");

        set_error_message_(what);
      }
    }
  }

  void
  xml_compiler::set_error_message_(const std::string& message)
  {
    if (error_message_.empty()) {
      error_message_ = message;
    }
    ++error_count_;
  }

  void
  xml_compiler::set_error_message_(const boost::format& message)
  {
    set_error_message_(message.str());
  }

  void
  xml_compiler::normalize_identifier(std::string& identifier)
  {
    boost::replace_all(identifier, ".", "_");
  }
}
