// This header intentionally has no include guards.

// Preparing ConfigIndex, etc for remapclasses_initialize_vector_loader.
// And, it's a valuable opportunity to traverse checkbox.xml. (it takes very long time!)
// We handle other data at this time.
//
// Targets:
//   - ConfigIndex (for <only>,<not> filter)
//   - identifier_map_
//   - KeyCode::VK_CONFIG_*
//   - essential_configurations
//   - preferences_node_tree
//
template <class T_preferences_node_tree>
class remapclasses_initialize_vector_prepare_loader {
public:
  remapclasses_initialize_vector_prepare_loader(const xml_compiler& xml_compiler,
                                                symbol_map& symbol_map,
                                                std::tr1::unordered_map<uint32_t, std::string>& identifier_map,
                                                std::vector<std::tr1::shared_ptr<essential_configuration> >& essential_configurations,
                                                T_preferences_node_tree* preferences_node_tree) :
    xml_compiler_(xml_compiler),
    symbol_map_(symbol_map),
    identifier_map_(identifier_map),
    essential_configurations_(essential_configurations),
    preferences_node_tree_(preferences_node_tree),
    root_preferences_node_tree_(preferences_node_tree)
  {}

  void traverse(const extracted_ptree& pt) {
    for (auto& it : pt) {
      // Hack for speed improvement.
      // We can stop traversing when we met <autogen>.
      if (it.first == "autogen") {
        continue;
      }

      // traverse
      {
        if (it.first != "item") {
          if (! it.second.empty()) {
            traverse(it.children_extracted_ptree());
          }
        } else {
          // preferences_node_tree
          if (! preferences_node_tree_) {
            throw xml_compiler_logic_error("preferences_node_tree_ is nullptr.");
          }
          std::tr1::shared_ptr<T_preferences_node_tree> ptr(new T_preferences_node_tree(preferences_node_tree_->get_node()));

          for (auto& child : it.children_extracted_ptree()) {
            ptr->handle_item_child(child.get_node());

            if (child.first == "identifier") {
              auto raw_identifier = boost::trim_copy(child.second.data());
              if (xml_compiler_.valid_identifier_(raw_identifier, it.first)) {
                auto identifier = raw_identifier;
                normalize_identifier_(identifier);

                // ----------------------------------------
                auto attr_essential = child.second.get_optional<std::string>("<xmlattr>.essential");
                if (attr_essential) {
                  essential_configurations_.push_back(std::tr1::shared_ptr<essential_configuration>(new essential_configuration(ptr->get_node())));
                  // Do not treat essentials anymore.
                  continue;
                }

                // ----------------------------------------
                auto attr_vk_config = child.second.get_optional<std::string>("<xmlattr>.vk_config");
                if (attr_vk_config) {
                  const char* names[] = {
                    "VK_CONFIG_TOGGLE_",
                    "VK_CONFIG_FORCE_ON_",
                    "VK_CONFIG_FORCE_OFF_",
                    "VK_CONFIG_SYNC_KEYDOWNUP_",
                  };
                  for (auto& n : names) {
                    symbol_map_.add("KeyCode", std::string(n) + identifier);
                  }
                }

                // ----------------------------------------
                if (boost::starts_with(identifier, "notsave_")) {
                  identifiers_notsave_.push_back(identifier);
                } else {
                  identifiers_except_notsave_.push_back(identifier);
                }
              }
            }
          }

          auto saved_preferences_node_tree = preferences_node_tree_;
          preferences_node_tree_ = ptr.get();
          {
            if (! it.second.empty()) {
              traverse(it.children_extracted_ptree());
            }
          }
          preferences_node_tree_ = saved_preferences_node_tree;

          preferences_node_tree_->push_back(ptr);
        }
      }
    }
  }

  // call "fixup" at traversing each file.
  void fixup(void) {
    preferences_node_tree_ = root_preferences_node_tree_;
  }

  // call "cleanup" at finished traversing all files.
  void cleanup(void) {
    fixup();

    // "notsave" has higher priority.
    for (auto& it : identifiers_notsave_) {
      symbol_map_.add("ConfigIndex", it);
    }
    identifiers_notsave_.clear();

    for (auto& it : identifiers_except_notsave_) {
      symbol_map_.add("ConfigIndex", it);
    }
    identifiers_except_notsave_.clear();
  }

private:
  const xml_compiler& xml_compiler_;
  symbol_map& symbol_map_;
  std::tr1::unordered_map<uint32_t, std::string>& identifier_map_;
  std::vector<std::tr1::shared_ptr<essential_configuration> >& essential_configurations_;
  T_preferences_node_tree* preferences_node_tree_;
  T_preferences_node_tree* const root_preferences_node_tree_;

  std::vector<std::string> identifiers_notsave_;
  std::vector<std::string> identifiers_except_notsave_;
};
