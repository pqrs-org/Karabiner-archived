#include <exception>
#include <boost/algorithm/string.hpp>
#include "pqrs/xml_compiler.hpp"

namespace pqrs {
  void
  xml_compiler::reload_devicedef_(void)
  {
    std::vector<xml_file_path_ptr> xml_file_path_ptrs;
    xml_file_path_ptrs.push_back(
      xml_file_path_ptr(new xml_file_path(xml_file_path::base_directory::private_xml, "private.xml")));
    xml_file_path_ptrs.push_back(
      xml_file_path_ptr(new xml_file_path(xml_file_path::base_directory::system_xml,  "devicevendordef.xml")));
    xml_file_path_ptrs.push_back(
      xml_file_path_ptr(new xml_file_path(xml_file_path::base_directory::system_xml,  "deviceproductdef.xml")));

    std::vector<ptree_ptr> pt_ptrs;
    read_xmls_(pt_ptrs, xml_file_path_ptrs);

    for (auto& pt_ptr : pt_ptrs) {
      traverse_devicedef_(*pt_ptr);
    }
  }

  void
  xml_compiler::traverse_devicedef_(const boost::property_tree::ptree& pt)
  {
    for (auto& it : pt) {
      if (it.first != "devicevendordef" &&
          it.first != "deviceproductdef") {
        traverse_devicedef_(it.second);
      } else {
        std::string type;
        const char* name_tag_name = NULL;
        const char* value_tag_name = NULL;
        boost::optional<std::string> name;
        boost::optional<std::string> value;

        // ----------------------------------------
        if (it.first == "devicevendordef") {
          type           = "DeviceVendor";
          name_tag_name  = "vendorname";
          value_tag_name = "vendorid";

        } else if (it.first == "deviceproductdef") {
          type           = "DeviceProduct";
          name_tag_name  = "productname";
          value_tag_name = "productid";

        } else {
          throw xml_compiler_logic_error("unknown type in traverse_devicedef_");
        }

        // ----------------------------------------
        for (auto& child : it.second) {
          if (child.first == name_tag_name) {
            name = boost::trim_copy(child.second.data());
          } else if (child.first == value_tag_name) {
            value = boost::trim_copy(child.second.data());
          }
        }

        // ----------------------------------------
        // Validation
        if (! name) {
          set_error_message_(std::string("No <") + name_tag_name + "> within <" + it.first + ">.");
          continue;
        }

        if (name->empty()) {
          set_error_message_(std::string("Empty <") + name_tag_name + "> within <" + it.first + ">.");
          continue;
        }

        if (! value) {
          set_error_message_(std::string("No <") + value_tag_name + "> within <" + it.first + ">.");
          continue;
        }

        if (value->empty()) {
          set_error_message_(std::string("Empty <") + value_tag_name + "> within <" + it.first + ">.");
          continue;
        }

        auto v = pqrs::string::to_uint32_t(value);
        if (! v) {
          set_error_message_(std::string("Invalid <") + value_tag_name + "> within <" + it.first + ">:\n\n<" +
                             value_tag_name + ">" + *value + "</" + value_tag_name + ">");
          continue;
        }

        // ----------------------------------------
        symbol_map_.add(type, *name, *v);
      }
    }
  }
}
