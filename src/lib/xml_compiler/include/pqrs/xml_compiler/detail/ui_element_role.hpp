// This header intentionally has no include guards.

class ui_element_role_loader final {
public:
  ui_element_role_loader(const xml_compiler& xml_compiler,
                         symbol_map& symbol_map) : xml_compiler_(xml_compiler),
                                                   symbol_map_(symbol_map) {}

  void traverse(const extracted_ptree& pt) const;

private:
  const xml_compiler& xml_compiler_;
  symbol_map& symbol_map_;
};
