#include <exception>
#include <boost/algorithm/string.hpp>
#include "pqrs/xml_compiler.hpp"

namespace pqrs {
  void
  xml_compiler::device_loader::traverse(const boost::property_tree::ptree& pt) const
  {
    for (auto& it : pt) {
      // extract include
      if (loader_wrapper<const device_loader>::extract_include(*this, xml_compiler_, it)) {
        continue;
      }

      // ------------------------------------------------------------
      if (it.first != "devicevendordef" &&
          it.first != "deviceproductdef") {
        if (! it.second.empty()) {
          traverse(it.second);
        }
      } else {
        std::string type;
        const char* name_tag_name = nullptr;
        const char* value_tag_name = nullptr;
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
          throw xml_compiler_logic_error("unknown type in device_loader::traverse");
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
          xml_compiler_.error_information_.set(std::string("No <") + name_tag_name + "> within <" + it.first + ">.");
          continue;
        }

        if (name->empty()) {
          xml_compiler_.error_information_.set(std::string("Empty <") + name_tag_name + "> within <" + it.first + ">.");
          continue;
        }

        if (! value) {
          xml_compiler_.error_information_.set(std::string("No <") + value_tag_name + "> within <" + it.first + ">.");
          continue;
        }

        if (value->empty()) {
          xml_compiler_.error_information_.set(std::string("Empty <") + value_tag_name + "> within <" + it.first + ">.");
          continue;
        }

        auto v = pqrs::string::to_uint32_t(value);
        if (! v) {
          xml_compiler_.error_information_.set(std::string("Invalid <") + value_tag_name + "> within <" + it.first + ">:\n\n<" +
                                               value_tag_name + ">" + *value + "</" + value_tag_name + ">");
          continue;
        }

        // ----------------------------------------
        symbol_map_.add(type, *name, *v);
      }
    }
  }
}
