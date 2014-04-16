// This header intentionally has no include guards.

class modifier {
public:
  boost::optional<const std::string&> get_name(void) const {
    if (! name_) return boost::none;
    return *name_;
  }
  void set_name(const std::string& v) { name_ = v; }

private:
  boost::optional<std::string> name_;
};

class modifier_loader {
public:
  modifier_loader(const xml_compiler& xml_compiler,
                  symbol_map& symbol_map,
                  remapclasses_initialize_vector& remapclasses_initialize_vector,
                  boost::unordered_map<uint32_t, std::string>& identifier_map,
                  boost::unordered_map<uint32_t, std::shared_ptr<modifier> >& modifier_map) :
    xml_compiler_(xml_compiler),
    symbol_map_(symbol_map),
    remapclasses_initialize_vector_(remapclasses_initialize_vector),
    identifier_map_(identifier_map),
    modifier_map_(modifier_map)
  {}
  ~modifier_loader(void);

  void traverse(const extracted_ptree& pt) const;

private:
  const xml_compiler& xml_compiler_;
  symbol_map& symbol_map_;
  remapclasses_initialize_vector& remapclasses_initialize_vector_;
  boost::unordered_map<uint32_t, std::string>& identifier_map_;

  boost::unordered_map<uint32_t, std::shared_ptr<modifier> >& modifier_map_;
};
