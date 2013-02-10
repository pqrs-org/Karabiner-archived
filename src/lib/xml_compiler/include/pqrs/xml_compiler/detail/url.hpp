// This header intentionally has no include guards.

class url {
public:
  const boost::optional<std::string>& get_name(void) const { return name_; }
  void set_name(const std::string& v) { name_ = v; }

  const boost::optional<std::string>& get_url(void) const { return url_; }
  void set_url(const std::string& v) { url_ = v; }

private:
  boost::optional<std::string> name_;
  boost::optional<std::string> url_;
};

class url_loader {
public:
  url_loader(const xml_compiler& xml_compiler,
             symbol_map& symbol_map,
             remapclasses_initialize_vector& remapclasses_initialize_vector,
             std::tr1::unordered_map<uint32_t, std::string>& identifier_map,
             std::tr1::unordered_map<uint32_t, std::tr1::shared_ptr<url> >& vk_open_url_map) :
    xml_compiler_(xml_compiler),
    symbol_map_(symbol_map),
    remapclasses_initialize_vector_(remapclasses_initialize_vector),
    identifier_map_(identifier_map),
    vk_open_url_map_(vk_open_url_map)
  {}
  ~url_loader(void);

  void traverse(const extracted_ptree& pt) const;

private:
  const xml_compiler& xml_compiler_;
  symbol_map& symbol_map_;
  remapclasses_initialize_vector& remapclasses_initialize_vector_;
  std::tr1::unordered_map<uint32_t, std::string>& identifier_map_;

  std::tr1::unordered_map<uint32_t, std::tr1::shared_ptr<url> >& vk_open_url_map_;
};
