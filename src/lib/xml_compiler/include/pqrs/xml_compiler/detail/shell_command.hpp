// This header intentionally has no include guards.

class shell_command final {
public:
  boost::optional<const std::string&> get_name(void) const {
    if (!name_) return boost::none;
    return *name_;
  }
  void set_name(const std::string& v) { name_ = v; }

  boost::optional<const std::string&> get_command(void) const {
    if (!command_) return boost::none;
    return *command_;
  }
  void set_command(const std::string& v) { command_ = v; }

private:
  boost::optional<std::string> name_;
  boost::optional<std::string> command_;
};

class shell_command_loader final {
public:
  shell_command_loader(const xml_compiler& xml_compiler,
                     symbol_map& symbol_map,
                     std::vector<std::shared_ptr<shell_command>>& shell_command_vector) : xml_compiler_(xml_compiler),
                                                                                      symbol_map_(symbol_map),
                                                                                      shell_command_vector_(shell_command_vector) {}

  void traverse(const extracted_ptree& pt) const;

private:
  const xml_compiler& xml_compiler_;
  symbol_map& symbol_map_;
  std::vector<std::shared_ptr<shell_command>>& shell_command_vector_;
};
