// This header intentionally has no include guards.

class filter_vector {
public:
  filter_vector(void) {}
  filter_vector(const symbol_map& symbol_map, const boost::property_tree::ptree& pt);

  std::vector<uint32_t>& get(void) {
    return data_;
  }
  const std::vector<uint32_t>& get(void) const {
    return data_;
  }
  bool empty(void) const {
    return data_.empty();
  }

private:
  void add(const symbol_map& symbol_map, uint32_t filter_type, const std::string& type, const std::string& string);

  std::vector<uint32_t> data_;
};
