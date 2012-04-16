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

// Preparing ConfigIndex, etc for remapclasses_initialize_vector_loader.
// And, it's a valuable opportunity to traverse checkbox.xml. (it takes very long time!)
// We handle other data at this time.
//
// Targets:
//   - ConfigIndex (for <only>,<not> filter)
//   - identifier_map_
//   - KeyCode::VK_CONFIG_*
//
class remapclasses_initialize_vector_prepare_loader {
public:
  remapclasses_initialize_vector_prepare_loader(const xml_compiler& xml_compiler,
                                                symbol_map& symbol_map,
                                                std::tr1::unordered_map<uint32_t, std::string>& identifier_map) :
    xml_compiler_(xml_compiler),
    symbol_map_(symbol_map),
    identifier_map_(identifier_map),
    parent_tag_name_(NULL)
  {}

  void traverse(const boost::property_tree::ptree& pt);
  void fixup(void); // call at traversing each file.

  void cleanup(void); // call at finished traversing all files.

private:
  const xml_compiler& xml_compiler_;
  symbol_map& symbol_map_;
  std::tr1::unordered_map<uint32_t, std::string>& identifier_map_;

  const std::string* parent_tag_name_;
  std::vector<std::string> identifiers_notsave_;
  std::vector<std::string> identifiers_except_notsave_;
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
