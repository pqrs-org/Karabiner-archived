// This header intentionally has no include guards.

class xml_compiler_runtime_error : public std::runtime_error {
public:
  xml_compiler_runtime_error(const std::string& what) : std::runtime_error(what) {}
  xml_compiler_runtime_error(const boost::format& what) : std::runtime_error(what.str()) {}
};
