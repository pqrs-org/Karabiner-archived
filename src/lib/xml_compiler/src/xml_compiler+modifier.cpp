#include <exception>
#include "pqrs/xml_compiler.hpp"
#include "bridge.h"

namespace pqrs {
  xml_compiler::modifier_loader::~modifier_loader(void)
  {
    std::string raw_identifier("system.vk_modifier_definition");
    auto identifier = raw_identifier;
    normalize_identifier_(identifier);

    uint32_t config_index = symbol_map_.add("ConfigIndex", identifier);
    identifier_map_[config_index] = raw_identifier;

    try {
      remapclasses_initialize_vector_.start(config_index, raw_identifier);
      {
        for (const auto& it : modifier_map_) {
          if (! it.second) continue;

          if (! (it.second)->get_name()) continue;
          auto& name = *((it.second)->get_name());

          remapclasses_initialize_vector_.push_back(9);
          remapclasses_initialize_vector_.push_back(BRIDGE_VK_MODIFIER);
          remapclasses_initialize_vector_.push_back(it.first);

          remapclasses_initialize_vector_.push_back(
            symbol_map_.add("KeyCode", std::string("VK_MODIFIER_") + name));
          remapclasses_initialize_vector_.push_back(
            symbol_map_.add("KeyCode", std::string("VK_LOCK_") + name));
          remapclasses_initialize_vector_.push_back(
            symbol_map_.add("KeyCode", std::string("VK_LOCK_") + name + "_FORCE_ON"));
          remapclasses_initialize_vector_.push_back(
            symbol_map_.add("KeyCode", std::string("VK_LOCK_") + name + "_FORCE_OFF"));
          remapclasses_initialize_vector_.push_back(
            symbol_map_.add("KeyCode", std::string("VK_STICKY_") + name));
          remapclasses_initialize_vector_.push_back(
            symbol_map_.add("KeyCode", std::string("VK_STICKY_") + name + "_FORCE_ON"));
          remapclasses_initialize_vector_.push_back(
            symbol_map_.add("KeyCode", std::string("VK_STICKY_") + name + "_FORCE_OFF"));

          remapclasses_initialize_vector_.push_back(2 + static_cast<uint32_t>(name.length()) + 1);
          remapclasses_initialize_vector_.push_back(BRIDGE_MODIFIERNAME);
          remapclasses_initialize_vector_.push_back(it.first);
          for (size_t i = 0; i < name.length(); ++i) {
            remapclasses_initialize_vector_.push_back(name[i]);
          }
          remapclasses_initialize_vector_.push_back('\0');
        }
      }
      remapclasses_initialize_vector_.end();
    } catch (std::exception& e) {
      assert(! "exception in ~modifier_loader");
    }
  }

  void
  xml_compiler::modifier_loader::traverse(const extracted_ptree& pt) const
  {
    for (const auto& it : pt) {
      if (it.get_tag_name() != "modifierdef") {
        if (! it.children_empty()) {
          traverse(it.children_extracted_ptree());
        }
      } else {
        std::shared_ptr<modifier> newmodifier(new modifier());
        if (! newmodifier) continue;

        newmodifier->set_name(it.get_data());

        // ----------------------------------------
        // register to symbol_map_.
        if (! symbol_map_.get_optional("ModifierFlag", it.get_data())) {
          auto v = symbol_map_.add("ModifierFlag", it.get_data());
          modifier_map_[v] = newmodifier;
        }
      }
    }
  }
}
