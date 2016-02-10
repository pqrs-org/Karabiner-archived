// This header intentionally has no include guards.

class preferences_node {
protected:
  preferences_node(void) : default_value_(0) {}

public:
  virtual ~preferences_node(void) {}

  const std::string& get_name(void) const { return name_; }
  const std::string& get_identifier(void) const { return identifier_; }
  int get_default_value(void) const { return default_value_; }
  const std::string& get_style(void) const { return style_; }

  void trim_name(void) { boost::trim(name_); }

protected:
  bool handle_name_and_appendix_(const extracted_ptree::node& it);

  std::string name_;
  std::string identifier_;
  int default_value_;
  std::string style_;
};

class preferences_checkbox_node final : public preferences_node {
public:
  void handle_item_child(const extracted_ptree::node& it);
};

class preferences_number_node final : public preferences_node {
public:
  preferences_number_node(void) : step_(1) {}

  void handle_item_child(const extracted_ptree::node& it);

  int get_step(void) const { return step_; }
  const std::string& get_base_unit(void) const { return base_unit_; }

private:
  int step_;
  std::string base_unit_;
};

template <class T>
class preferences_node_tree final {
public:
  typedef std::shared_ptr<const preferences_node_tree> preferences_node_tree_ptr;
  typedef std::vector<preferences_node_tree_ptr> preferences_node_tree_ptrs;
  typedef std::shared_ptr<preferences_node_tree_ptrs> preferences_node_tree_ptrs_ptr;

  preferences_node_tree(void) {}
  preferences_node_tree(const T& parent_node) : node_(parent_node) {}

  void clear(void) {
    if (children_) {
      children_->clear();
    }
  }
  void handle_item_child(const extracted_ptree::node& it) {
    node_.handle_item_child(it);
  }
  void trim_name() {
    node_.trim_name();
  }

  const T& get_node(void) const { return node_; }
  const preferences_node_tree_ptrs_ptr& get_children(void) const { return children_; }

  void push_back(const preferences_node_tree_ptr& ptr) {
    if (!children_) {
      children_ = preferences_node_tree_ptrs_ptr(new preferences_node_tree_ptrs());
    }
    children_->push_back(ptr);
  }

private:
  T node_;

  // We use shared_ptr<vector> to store children node by these reason.
  // * sizeof(shared_ptr) < sizeof(vector).
  // * children_ is mostly empty.
  preferences_node_tree_ptrs_ptr children_;
};
