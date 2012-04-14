// This header intentionally has no include guards.

class remapclasses_initialize_vector {
public:
  remapclasses_initialize_vector(void);
  void clear(void);
  const std::vector<uint32_t>& get(void) const;
  uint32_t get_config_count(void) const;
  void add(const std::vector<uint32_t>& v, uint32_t config_index, const std::string& identifier);
  void freeze(void);

private:
  enum {
    INDEX_OF_FORMAT_VERSION = 0,
    INDEX_OF_CONFIG_COUNT = 1,
  };

  std::vector<uint32_t> data_;
  std::tr1::unordered_map<uint32_t, bool> is_config_index_added_;
  uint32_t max_config_index_;
  bool freezed_;
};

class remapclasses_initialize_vector_loader {
public:
  remapclasses_initialize_vector_loader(const xml_compiler& xml_compiler,
                                        symbol_map& symbol_map,
                                        remapclasses_initialize_vector& remapclasses_initialize_vector,
                                        std::tr1::unordered_map<uint32_t, std::string>& identifier_map) :
    xml_compiler_(xml_compiler),
    symbol_map_(symbol_map),
    remapclasses_initialize_vector_(remapclasses_initialize_vector),
    identifier_map_(identifier_map),
    simultaneous_keycode_index_(0)
  {}

  void reload(void) const;

private:
  const xml_compiler& xml_compiler_;
  symbol_map& symbol_map_;
  remapclasses_initialize_vector& remapclasses_initialize_vector_;
  std::tr1::unordered_map<uint32_t, std::string>& identifier_map_;
  uint32_t simultaneous_keycode_index_;
};
