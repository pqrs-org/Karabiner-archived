// This header intentionally has no include guards.

class language {
public:
  class value_type {
  public:
    enum type {
      none,
      bcp47,
      input_source_equal,
      input_source_prefix,
      input_mode_equal,
      input_mode_prefix,
    };
  };

  const boost::optional<std::string>& get_name(void) const { return name_; }
  void set_name(const std::string& v) { name_ = v; }

  const boost::optional<std::string>& get_value(void) const { return value_; }
  void set_value(const std::string& v) { value_ = v; }

  value_type::type get_type(void) const { return value_type_; }
  void set_type(value_type::type v) { value_type_ = v; }

private:
  boost::optional<std::string> name_;
  boost::optional<std::string> value_;
  value_type::type value_type_;
};

class language_loader {
public:
  language_loader(const xml_compiler& xml_compiler,
                  remapclasses_initialize_vector& remapclasses_initialize_vector,
                  symbol_map& symbol_map,
                  std::vector<std::tr1::shared_ptr<language> >& language_vector) :
    xml_compiler_(xml_compiler),
    remapclasses_initialize_vector_(remapclasses_initialize_vector),
    symbol_map_(symbol_map),
    language_vector_(language_vector)
  {}

  void traverse(const extracted_ptree& pt) const;

private:
  const xml_compiler& xml_compiler_;
  remapclasses_initialize_vector& remapclasses_initialize_vector_;
  symbol_map& symbol_map_;
  std::vector<std::tr1::shared_ptr<language> >& language_vector_;
};
