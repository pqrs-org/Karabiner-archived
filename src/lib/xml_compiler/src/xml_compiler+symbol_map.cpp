#include "pqrs/xml_compiler.hpp"

namespace pqrs {
  xml_compiler::symbol_map::symbol_map(void)
  {
    clear();
  }

  void
  xml_compiler::symbol_map::clear(void)
  {
    symbol_map_.clear();
    symbol_map_["ConfigIndex::VK__AUTOINDEX__BEGIN__"] = 0;
  }

  void
  xml_compiler::symbol_map::dump(void) const
  {
    for (auto& it : symbol_map_) {
      std::cout << it.first << " " << it.second << std::endl;
    }
  }

  uint32_t
  xml_compiler::symbol_map::get(const std::string& name) const
  {
    auto v = get_optional(name);
    if (! v) {
      throw xml_compiler_runtime_error("Unknown symbol:\n\n" + name);
    }
    return *v;
  }

  uint32_t
  xml_compiler::symbol_map::get(const std::string& type, const std::string& name) const
  {
    return get(type + "::" + name);
  }

  boost::optional<uint32_t>
  xml_compiler::symbol_map::get_optional(const std::string& name) const
  {
    auto it = symbol_map_.find(name);
    if (it == symbol_map_.end()) {
      return boost::none;
    }

    return it->second;
  }

  boost::optional<uint32_t>
  xml_compiler::symbol_map::get_optional(const std::string& type, const std::string& name) const
  {
    return get_optional(type + "::" + name);
  }

  void
  xml_compiler::symbol_map::add(const std::string& type, const std::string& name, uint32_t value)
  {
    if (type.empty()) {
      throw xml_compiler_logic_error("Empty type:\n\n::" + name);
    }
    if (name.empty()) {
      throw xml_compiler_logic_error("Empty name:\n\n" + type + "::");
    }

    auto n = type + "::" + name;

    // register value if the definition does not exists.
    auto it = symbol_map_.find(n);
    if (it == symbol_map_.end()) {
      symbol_map_[n] = value;
    }
  }

  void
  xml_compiler::symbol_map::add(const std::string& type, const std::string& name)
  {
    auto n = type + "::VK__AUTOINDEX__BEGIN__";
    auto v = get(n);
    symbol_map_[n] = v + 1;
    return add(type, name, v);
  }

  // ============================================================
  void
  xml_compiler::reload_symbol_map_(void)
  {
    symbol_map_.clear();

    std::vector<xml_file_path_ptr> xml_file_path_ptrs;
    xml_file_path_ptrs.push_back(
      xml_file_path_ptr(new xml_file_path(xml_file_path::base_directory::private_xml, "private.xml")));
    xml_file_path_ptrs.push_back(
      xml_file_path_ptr(new xml_file_path(xml_file_path::base_directory::system_xml,  "symbol_map.xml")));

    std::vector<ptree_ptr> pt_ptrs;
    read_xmls_(pt_ptrs, xml_file_path_ptrs);

    for (auto& pt_ptr : pt_ptrs) {
      traverse_symbol_map_(*pt_ptr);
    }
  }

  void
  xml_compiler::traverse_symbol_map_(const boost::property_tree::ptree& pt)
  {
    for (auto& it : pt) {
      if (it.first != "symbol_map") {
        traverse_symbol_map_(it.second);

      } else {
        std::vector<boost::optional<std::string> > vector;
        const char* attrs[] = {
          "<xmlattr>.type",
          "<xmlattr>.name",
          "<xmlattr>.value",
        };
        for (auto& attr : attrs) {
          const char* attrname = attr + strlen("<xmlattr>.");

          auto v = it.second.get_optional<std::string>(attr);
          if (! v) {
            set_error_message_(std::string("No '") + attrname + "' Attribute within <symbol_map>.");
            break;
          }
          if (v->empty()) {
            set_error_message_(std::string("Empty '") + attrname + "' Attribute within <symbol_map>.");
            continue;
          }
          vector.push_back(v);
        }
        // An error has occured when vector.size != attrs.size.
        if (vector.size() != sizeof(attrs) / sizeof(attrs[0])) {
          continue;
        }

        auto value = pqrs::string::to_uint32_t(vector[2]);
        if (! value) {
          set_error_message_(boost::format("Invalid 'value' Attribute within <symbol_map>:\n"
                                           "\n"
                                           "<symbol_map type=\"%1%\" name=\"%2%\" value=\"%3%\" />") %
                             *(vector[0]) % *(vector[1]) % *(vector[2]));
          continue;
        }

        symbol_map_.add(*(vector[0]), *(vector[1]), *value);
      }
    }
  }
}
