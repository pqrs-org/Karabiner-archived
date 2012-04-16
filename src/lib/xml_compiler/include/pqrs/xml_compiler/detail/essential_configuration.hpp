// This header intentionally has no include guards.

class essential_configuration {
public:
  essential_configuration(const preferences_node& node) :
    identifier_(node.get_identifier()),
    raw_identifier_(node.get_identifier()),
    default_value_(node.get_default_value())
  {
    xml_compiler::normalize_identifier_(identifier_);
  }

  const std::string& get_identifier(void) const { return identifier_; }
  const std::string& get_raw_identifier(void) const { return raw_identifier_; }
  int get_default_value(void) const { return default_value_; }

private:
  std::string identifier_;
  std::string raw_identifier_;
  int default_value_;
};
