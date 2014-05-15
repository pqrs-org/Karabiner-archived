// This header intentionally has no include guards.

class window_name {
public:
  boost::optional<const std::string&> get_name(void) const {
    if (! name_) return boost::none;
    return *name_;
  }
  void set_name(const std::string& v) { name_ = v; }
  void add_rule_regex(const std::string& regex);
  bool is_rules_matched(const std::string& window_name) const;

private:
  boost::optional<std::string> name_;
  std::vector<std::regex> rules_regex_;
};

class window_name_loader {
public:
  window_name_loader(const xml_compiler& xml_compiler,
                     symbol_map& symbol_map,
                     std::vector<std::shared_ptr<window_name> >& window_name_vector) :
    xml_compiler_(xml_compiler),
    symbol_map_(symbol_map),
    window_name_vector_(window_name_vector)
  {}

  void traverse(const extracted_ptree& pt) const;

private:
  const xml_compiler& xml_compiler_;
  symbol_map& symbol_map_;
  std::vector<std::shared_ptr<window_name> >& window_name_vector_;
};
