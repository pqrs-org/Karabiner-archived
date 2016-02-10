// This header intentionally has no include guards.

// Preparing ConfigIndex, etc for remapclasses_initialize_vector_loader.
// And, it's a valuable opportunity to traverse checkbox.xml. (it takes very long time!)
// We handle other data at this time.
//
// Targets:
//   - ConfigIndex (for <only>,<not> filter)
//   - KeyCode::VK_CONFIG_*
//   - essential_configurations
//   - preferences_node_tree
//
template <class preferences_node_tree_t>
class remapclasses_initialize_vector_prepare_loader final {
public:
  remapclasses_initialize_vector_prepare_loader(const xml_compiler& xml_compiler,
                                                symbol_map& symbol_map,
                                                std::vector<std::shared_ptr<essential_configuration>>& essential_configurations,
                                                preferences_node_tree_t* preferences_node_tree) : xml_compiler_(xml_compiler),
                                                                                                  symbol_map_(symbol_map),
                                                                                                  essential_configurations_(essential_configurations),
                                                                                                  preferences_node_tree_(preferences_node_tree),
                                                                                                  root_preferences_node_tree_(preferences_node_tree) {}

  void traverse(const extracted_ptree& pt, const std::string& parent_tag_name) {
    for (const auto& it : pt) {
      // Hack for speed improvement.
      // We can stop traversing when we met <autogen>.
      if (it.get_tag_name() == "autogen") {
        continue;
      }

      // traverse
      if (it.get_tag_name() == "item") {
        assert(preferences_node_tree_);
        std::shared_ptr<preferences_node_tree_t> ptr(new preferences_node_tree_t());
        assert(ptr);

        auto saved_preferences_node_tree = preferences_node_tree_;
        preferences_node_tree_ = ptr.get();
        {
          if (!it.children_empty()) {
            traverse(it.children_extracted_ptree(), it.get_tag_name());
          }
        }
        preferences_node_tree_ = saved_preferences_node_tree;

        auto attr_hidden = it.get_optional("<xmlattr>.hidden");
        if (!attr_hidden) {
          ptr->trim_name();
          preferences_node_tree_->push_back(ptr);
        }

      } else {
        assert(preferences_node_tree_);
        if (parent_tag_name == "item") {
          preferences_node_tree_->handle_item_child(it);
        }

        if (it.get_tag_name() == "identifier") {
          auto raw_identifier = boost::trim_copy(it.get_data());
          if (xml_compiler_.valid_identifier_(raw_identifier, parent_tag_name)) {
            auto identifier = raw_identifier;
            normalize_identifier_(identifier);

            // ----------------------------------------
            auto attr_essential = it.get_optional("<xmlattr>.essential");
            if (attr_essential) {
              essential_configurations_.push_back(std::shared_ptr<essential_configuration>(new essential_configuration(preferences_node_tree_->get_node())));
              // Do not treat essentials anymore.
              continue;
            }

            // ----------------------------------------
            auto attr_vk_config = it.get_optional("<xmlattr>.vk_config");
            if (attr_vk_config) {
              const char* names[] = {
                  "VK_CONFIG_TOGGLE_",
                  "VK_CONFIG_FORCE_ON_",
                  "VK_CONFIG_FORCE_OFF_",
                  "VK_CONFIG_SYNC_KEYDOWNUP_",
              };
              for (const auto& n : names) {
                symbol_map_.add("KeyCode", std::string(n) + identifier);
              }
            }

            // ----------------------------------------
            auto attr_high_priority = it.get_optional("<xmlattr>.high_priority");
            if (boost::starts_with(identifier, "notsave_") ||
                attr_high_priority) {
              identifiers_high_priority_.push_back(identifier);
            } else {
              identifiers_normal_priority_.push_back(identifier);
            }
          }
        }

        if (!it.children_empty()) {
          traverse(it.children_extracted_ptree(), it.get_tag_name());
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
    for (const auto& it : identifiers_high_priority_) {
      symbol_map_.add("ConfigIndex", it);
    }
    identifiers_high_priority_.clear();

    for (const auto& it : identifiers_normal_priority_) {
      symbol_map_.add("ConfigIndex", it);
    }
    identifiers_normal_priority_.clear();
  }

private:
  const xml_compiler& xml_compiler_;
  symbol_map& symbol_map_;
  std::vector<std::shared_ptr<essential_configuration>>& essential_configurations_;
  preferences_node_tree_t* preferences_node_tree_;
  preferences_node_tree_t* const root_preferences_node_tree_;

  std::vector<std::string> identifiers_high_priority_;
  std::vector<std::string> identifiers_normal_priority_;
};
