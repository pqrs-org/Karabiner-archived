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
  xml_compiler::symbol_map_loader::traverse(const extracted_ptree& pt) const
  {
    for (auto& it : pt) {
      if (it.first != "symbol_map") {
        if (! it.second.empty()) {
          traverse(it.children_extracted_ptree());
        }

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
            xml_compiler_.error_information_.set(std::string("No '") + attrname + "' Attribute within <symbol_map>.");
            break;
          }
          if (v->empty()) {
            xml_compiler_.error_information_.set(std::string("Empty '") + attrname + "' Attribute within <symbol_map>.");
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
          xml_compiler_.error_information_.set(boost::format("Invalid 'value' Attribute within <symbol_map>:\n"
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
