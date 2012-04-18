// This header intentionally has no include guards.

class replacement_loader {
public:
  replacement_loader(const xml_compiler& xml_compiler,
                     pqrs::string::replacement& replacement) :
    xml_compiler_(xml_compiler),
    replacement_(replacement)
  {}

  void traverse(const extracted_ptree& pt) const;

private:
  const xml_compiler& xml_compiler_;
  pqrs::string::replacement& replacement_;
};
