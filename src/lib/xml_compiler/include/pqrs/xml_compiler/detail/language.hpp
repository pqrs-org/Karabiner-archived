// This header intentionally has no include guards.

class language {
public:
  class value_type {
  public:
    enum type {
      none,
      bcp47,
      inputsourceid_equal,
      inputsourceid_prefix,
      inputmodeid_equal,
      inputmodeid_prefix,
    };

    static type get_type_from_string(const std::string& string) {
      if (string == "bcp47")                { return type::bcp47; }
      if (string == "inputsourceid_equal")  { return type::inputsourceid_equal; }
      if (string == "inputsourceid_prefix") { return type::inputsourceid_prefix; }
      if (string == "inputmodeid_equal")    { return type::inputmodeid_equal; }
      if (string == "inputmodeid_prefix")   { return type::inputmodeid_prefix; }
      return type::none;
    }
  };

  uint32_t get_keycode(void) const { return keycode_; }
  void set_keycode(uint32_t v) { keycode_ = v; }

  const boost::optional<std::string>& get_name(void) const { return name_; }
  void set_name(const std::string& v) { name_ = v; }

  const boost::optional<std::string>& get_value(void) const { return value_; }
  void set_value(const std::string& v) { value_ = v; }

  value_type::type get_type(void) const { return value_type_; }
  void set_type(value_type::type v) { value_type_ = v; }

  bool is_rules_matched(uint32_t keycode,
                        const std::string& bcp47,
                        const std::string& inputsourceid,
                        const std::string& inputmodeid) const;

private:
  uint32_t keycode_;
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
