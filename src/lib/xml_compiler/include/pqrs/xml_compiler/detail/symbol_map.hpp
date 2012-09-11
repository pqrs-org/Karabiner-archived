// This header intentionally has no include guards.

class symbol_map {
public:
  symbol_map(void);
  void clear(void);
  void dump(void) const; // For debug

  uint32_t get(const std::string& name) const;
  uint32_t get(const std::string& type, const std::string& name) const;

  boost::optional<uint32_t> get_optional(const std::string& name) const;
  boost::optional<uint32_t> get_optional(const std::string& type, const std::string& name) const;

  // Call add("KeyCode", "RETURN", 36) to register "KeyCode::RETURN = 36".
  uint32_t add(const std::string& type, const std::string& name, uint32_t value);
  uint32_t add(const std::string& type, const std::string& name);

private:
  std::tr1::unordered_map<std::string, uint32_t> symbol_map_;
};

class symbol_map_loader {
public:
  symbol_map_loader(const xml_compiler& xml_compiler,
                    symbol_map& symbol_map) :
    xml_compiler_(xml_compiler),
    symbol_map_(symbol_map)
  {}

  void traverse(const extracted_ptree& pt) const;

private:
  const xml_compiler& xml_compiler_;
  symbol_map& symbol_map_;
};
