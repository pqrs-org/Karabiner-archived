// This header intentionally has no include guards.

class filter_vector {
public:
  filter_vector(const symbol_map& symbol_map) :
    symbol_map_(symbol_map)
  {}
  void traverse(const extracted_ptree& pt);

  const std::vector<uint32_t>& get(void) const {
    return data_;
  }

  bool empty(void) const {
    return data_.empty();
  }
  size_t size(void) const {
    return data_.size();
  }
  void clear(void) {
    data_.clear();
  }
  void resize(size_t s) {
    data_.resize(s);
  }
  void push_back(uint32_t v) {
    data_.push_back(v);
  }

private:
  void add_(uint32_t filter_type, const std::string& type, const std::string& string);

  const symbol_map& symbol_map_;
  std::vector<uint32_t> data_;
};
