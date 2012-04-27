// This header intentionally has no include guards.

class remapclasses_initialize_vector {
public:
  remapclasses_initialize_vector(void);
  void clear(void);
  const std::vector<uint32_t>& get(void) const;
  uint32_t get_config_count(void) const;

  void start(uint32_t config_index, const std::string& raw_identifier);
  void push_back(uint32_t v) {
    // increment size
    ++(data_[start_index_]);

    data_.push_back(v);
  }
  void end(void);

  void freeze(void);

private:
  void cleanup_(void);

  enum {
    INDEX_OF_FORMAT_VERSION = 0,
    INDEX_OF_CONFIG_COUNT = 1,
  };

  std::vector<uint32_t> data_;
  std::tr1::unordered_map<uint32_t, bool> is_config_index_added_;
  uint32_t max_config_index_;
  bool freezed_;

  // variables for start() and end()
  size_t start_index_;
  bool ended_;
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

  void traverse(const extracted_ptree& pt,
                const std::string& parent_tag_name);

private:
  void traverse_autogen_(const extracted_ptree& pt,
                         const std::string& identifier,
                         const filter_vector& filter_vector);

  void handle_autogen(const std::string& autogen,
                      const std::string& raw_autogen,
                      const filter_vector& filter_vector);

  void add_to_initialize_vector(const std::string& params,
                                uint32_t type,
                                const filter_vector& filter_vector) const;

  const xml_compiler& xml_compiler_;
  symbol_map& symbol_map_;
  remapclasses_initialize_vector& remapclasses_initialize_vector_;
  std::tr1::unordered_map<uint32_t, std::string>& identifier_map_;
  uint32_t simultaneous_keycode_index_;
};
