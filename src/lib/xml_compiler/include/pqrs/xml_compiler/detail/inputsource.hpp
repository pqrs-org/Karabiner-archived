// This header intentionally has no include guards.

class inputsource {
public:
  const boost::optional<std::string>& get_name(void) const { return name_; }
  void set_name(const std::string& v) { name_ = v; }

  const boost::optional<std::string>& get_detail(void) const { return detail_; }
  void set_detail(const std::string& v) { detail_ = v; }

  void add_rule_bcp47(const std::string& v)                { if (! v.empty()) { rules_bcp47_.push_back(v); } }
  void add_rule_inputsourceid_equal(const std::string& v)  { if (! v.empty()) { rules_inputsourceid_equal_.push_back(v); } }
  void add_rule_inputsourceid_prefix(const std::string& v) { if (! v.empty()) { rules_inputsourceid_prefix_.push_back(v); } }
  void add_rule_inputmodeid_equal(const std::string& v)    { if (! v.empty()) { rules_inputmodeid_equal_.push_back(v); } }
  void add_rule_inputmodeid_prefix(const std::string& v)   { if (! v.empty()) { rules_inputmodeid_prefix_.push_back(v); } }

  bool is_rules_matched(const std::string& bcp47,
                        const std::string& inputsourceid,
                        const std::string& inputmodeid) const;

private:
  boost::optional<std::string> name_;
  boost::optional<std::string> detail_;
  std::vector<std::string> rules_bcp47_;
  std::vector<std::string> rules_inputsourceid_equal_;
  std::vector<std::string> rules_inputsourceid_prefix_;
  std::vector<std::string> rules_inputmodeid_equal_;
  std::vector<std::string> rules_inputmodeid_prefix_;
};

class inputsource_loader {
public:
  inputsource_loader(const xml_compiler& xml_compiler,
                     symbol_map& symbol_map,
                     remapclasses_initialize_vector& remapclasses_initialize_vector,
                     std::tr1::unordered_map<uint32_t, std::string>& identifier_map,
                     std::tr1::unordered_map<uint32_t, std::tr1::shared_ptr<inputsource> >& vk_change_inputsource_map,
                     std::vector<std::tr1::shared_ptr<inputsource> >& inputsource_vector) :
    xml_compiler_(xml_compiler),
    symbol_map_(symbol_map),
    remapclasses_initialize_vector_(remapclasses_initialize_vector),
    identifier_map_(identifier_map),
    vk_change_inputsource_map_(vk_change_inputsource_map),
    inputsource_vector_(inputsource_vector)
  {}
  ~inputsource_loader(void);

  void traverse(const extracted_ptree& pt) const;

private:
  class definition_type {
  public:
    enum type {
      none,
      vkchangeinputsourcedef,
      inputsourcedef,
    };
  };

  const xml_compiler& xml_compiler_;
  symbol_map& symbol_map_;
  remapclasses_initialize_vector& remapclasses_initialize_vector_;
  std::tr1::unordered_map<uint32_t, std::string>& identifier_map_;

  std::tr1::unordered_map<uint32_t, std::tr1::shared_ptr<inputsource> >& vk_change_inputsource_map_;
  std::vector<std::tr1::shared_ptr<inputsource> >& inputsource_vector_;
};
