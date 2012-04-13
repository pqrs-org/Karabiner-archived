// This header intentionally has no include guards.

class device_loader {
public:
  device_loader(const xml_compiler& xml_compiler,
                symbol_map& symbol_map) :
    xml_compiler_(xml_compiler),
    symbol_map_(symbol_map)
  {}

  void reload(void) const;

private:
  void traverse_(const boost::property_tree::ptree& pt) const;

  const xml_compiler& xml_compiler_;
  symbol_map& symbol_map_;
};
