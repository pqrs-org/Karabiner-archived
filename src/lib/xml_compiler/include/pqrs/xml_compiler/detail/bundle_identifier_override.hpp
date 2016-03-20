// This header intentionally has no include guards.

class bundle_identifier_override final {
public:
  enum class rule_target {
    bundle_identifiers,
    window_names,
    ui_element_roles,
    end,
  };

  boost::optional<const std::string&> get_new_bundle_identifier(void) const {
    if (!new_bundle_identifier_) return boost::none;
    return *new_bundle_identifier_;
  }
  void set_new_bundle_identifier(const std::string& v) { new_bundle_identifier_ = v; }

  void add_rule_equal(rule_target target, const std::string& v);
  void add_rule_prefix(rule_target target, const std::string& v);
  void add_rule_suffix(rule_target target, const std::string& v);
  void add_rule_regex(rule_target target, const std::string& v);
  bool is_rules_matched(const std::string& bundle_identifier,
                        const std::string& window_name,
                        const std::string& ui_element_role) const;

private:
  bool is_rules_matched(rule_target target, const std::string& v) const;

  boost::optional<std::string> new_bundle_identifier_;
  std::vector<std::string> rules_equal_[static_cast<size_t>(rule_target::end)];
  std::vector<std::string> rules_prefix_[static_cast<size_t>(rule_target::end)];
  std::vector<std::string> rules_suffix_[static_cast<size_t>(rule_target::end)];
  std::vector<std::regex> rules_regex_[static_cast<size_t>(rule_target::end)];
};

class bundle_identifier_override_loader final {
public:
  bundle_identifier_override_loader(const xml_compiler& xml_compiler,
                                    symbol_map& symbol_map,
                                    std::vector<std::shared_ptr<bundle_identifier_override>>& bundle_identifier_override_vector) : xml_compiler_(xml_compiler),
                                                                                                                                   symbol_map_(symbol_map),
                                                                                                                                   bundle_identifier_override_vector_(bundle_identifier_override_vector) {}

  void traverse(const extracted_ptree& pt) const;

private:
  void traverse_definition(const extracted_ptree& pt,
                           std::shared_ptr<bundle_identifier_override> new_bundle_identifier_override,
                           bundle_identifier_override::rule_target current_rule_target) const;

  const xml_compiler& xml_compiler_;
  symbol_map& symbol_map_;
  std::vector<std::shared_ptr<bundle_identifier_override>>& bundle_identifier_override_vector_;
};
