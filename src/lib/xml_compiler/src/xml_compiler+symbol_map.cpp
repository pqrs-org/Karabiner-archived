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

    map_for_get_name_.clear();
  }

  void
  xml_compiler::symbol_map::dump(void) const
  {
    for (const auto& it : symbol_map_) {
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
      // XXX::RawValue::YYY does not appear frequently.
      // Therefore, we don't check "XXX::RawValue::" prefix at first in order to improve performance.
      // Treat XXX::RawValue::XXX at here.
      auto found = name.find("::RawValue::");
      if (found != std::string::npos) {
        return pqrs::string::to_uint32_t(name.c_str() + found + strlen("::RawValue::"));
      }

      return boost::none;
    }

    return it->second;
  }

  boost::optional<uint32_t>
  xml_compiler::symbol_map::get_optional(const std::string& type, const std::string& name) const
  {
    return get_optional(type + "::" + name);
  }

  uint32_t
  xml_compiler::symbol_map::add(const std::string& type, const std::string& name, uint32_t value)
  {
    assert(! type.empty());
    assert(! name.empty());

    // register value if the definition does not exists.
    auto n = type + "::" + name;
    symbol_map_.emplace(n, value);
    map_for_get_name_.emplace(type + "::" + boost::lexical_cast<std::string>(value), n);

    return value;
  }

  uint32_t
  xml_compiler::symbol_map::add(const std::string& type, const std::string& name)
  {
    auto n = type + "::VK__AUTOINDEX__BEGIN__";
    auto v = get_optional(n);

    assert(v);
    assert(*v != std::numeric_limits<uint32_t>::max());

    symbol_map_[n] = *v + 1;
    return add(type, name, *v);
  }

  boost::optional<const std::string&>
  xml_compiler::symbol_map::get_name(const std::string& type, uint32_t value) const
  {
    auto it = map_for_get_name_.find(type + "::" + boost::lexical_cast<std::string>(value));
    if (it == map_for_get_name_.end()) {
      return boost::none;
    }

    return it->second;
  }

  // ============================================================
  void
  xml_compiler::symbol_map_loader::traverse(const extracted_ptree& pt) const
  {
    for (const auto& it : pt) {
      if (it.get_tag_name() != "symbol_map") {
        if (! it.children_empty()) {
          traverse(it.children_extracted_ptree());
        }

      } else {
        std::vector<boost::optional<std::string> > vector;
        const char* attrs[] = {
          "<xmlattr>.type",
          "<xmlattr>.name",
          "<xmlattr>.value",
        };
        for (const auto& attr : attrs) {
          const char* attrname = attr + strlen("<xmlattr>.");

          auto v = it.get_optional(attr);
          if (! v) {
            xml_compiler_.error_information_.set(std::string("No '") + attrname + "' Attribute within <symbol_map>.");
            break;
          }

          std::string value = pqrs::string::remove_whitespaces_copy(*v);
          if (value.empty()) {
            xml_compiler_.error_information_.set(std::string("Empty '") + attrname + "' Attribute within <symbol_map>.");
            continue;
          }

          vector.push_back(value);
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
