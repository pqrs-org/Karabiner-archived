// This header intentionally has no include guards.

class extracted_ptree
{
public:
  extracted_ptree(const xml_compiler& xml_compiler,
                  const pqrs::string::replacement& replacement,
                  const boost::property_tree::ptree& pt,
                  const std::string& xml_file_path) :
    xml_compiler_(xml_compiler),
    replacement_(replacement),
    pt_(pt),
    included_files_ptr_(new std::deque<std::string>),
    included_files_(*included_files_ptr_),
    stack_ptr_(new std::stack<stack_data>),
    stack_(*stack_ptr_)
  {
    included_files_.push_back(xml_file_path);
  }

  class node
  {
  public:
    node(const boost::property_tree::ptree::value_type& node,
         const extracted_ptree& extracted_ptree,
         const pqrs::string::replacement& replacement) :
      node_(node),
      extracted_ptree_(extracted_ptree),
      replacement_(replacement)
    {}

    const std::string& get_tag_name(void) const { return node_.first; }
    const std::string& get_data(void) const { return node_.second.data(); }
    boost::optional<std::string> get_optional(const std::string& path) const {
      return node_.second.get_optional<std::string>(path);
    }

    bool children_empty(void) const { return node_.second.empty(); }
    extracted_ptree children_extracted_ptree(void) const {
      return extracted_ptree(extracted_ptree_, replacement_, node_.second);
    }

  private:
    const boost::property_tree::ptree::value_type& node_;
    const extracted_ptree& extracted_ptree_;
    const pqrs::string::replacement& replacement_;
  };

  class stack_data {
  public:
    stack_data(const boost::property_tree::ptree& pt,
               const pqrs::string::replacement& r) :
      it(pt.begin()),
      end(pt.end()),
      parent_replacement(r)
    {}

    // For extracted ptree.
    stack_data(const ptree_ptr& p,
               const std::tr1::shared_ptr<pqrs::string::replacement>& r,
               const boost::property_tree::ptree& root_children) :
      it(root_children.begin()),
      end(root_children.end()),
      parent_replacement(*r),
      pt_ptr_(p),
      replacement_ptr_(r)
    {}

    boost::property_tree::ptree::const_iterator it;
    boost::property_tree::ptree::const_iterator end;
    const pqrs::string::replacement& parent_replacement;

    bool extracted(void) const { return pt_ptr_; }

  private:
    // Keep extracted ptree_ptr, replacement_ptr until we finish traversing.
    const ptree_ptr pt_ptr_;
    const std::tr1::shared_ptr<pqrs::string::replacement> replacement_ptr_;
  };

  class extracted_ptree_iterator : public boost::iterator_facade<extracted_ptree_iterator,
                                                                 const node,
                                                                 boost::forward_traversal_tag>
  {
  public:
    extracted_ptree_iterator(const extracted_ptree& extracted_ptree) :
      extracted_ptree_(extracted_ptree),
      stack_size_(0),
      node_(nullptr)
    {
      set_node_();
    }

    extracted_ptree_iterator(const extracted_ptree& extracted_ptree,
                             const pqrs::string::replacement& replacement,
                             const boost::property_tree::ptree& pt) :
      extracted_ptree_(extracted_ptree),
      stack_size_(extracted_ptree.stack_.size() + 1),
      node_(nullptr)
    {
      if (! pt.empty()) {
        extracted_ptree_.stack_.push(stack_data(pt, replacement));
        extract_include_();
      }
      set_node_();
    }

    ~extracted_ptree_iterator(void) {
      if (node_) {
        delete node_;
        node_ = nullptr;
      }

      while (! ended_()) {
        auto& top = extracted_ptree_.stack_.top();
        top.it = top.end;
        collapse_();
      }
    }

  private:
    friend class boost::iterator_core_access;

    void increment(void) {
      if (ended_()) return;

      auto& top = extracted_ptree_.stack_.top();
      if (top.it == top.end) return;

      ++(top.it);
      collapse_();
      extract_include_();

      set_node_();
    }

    const node& dereference(void) const {
      if (! node_) {
        throw xml_compiler_logic_error("node_ == nullptr in extracted_ptree_iterator::dereference.");
      }
      return *node_;
    }

    bool equal(const extracted_ptree_iterator& other) const {
      if (is_end_() && other.is_end_()) {
        return true;
      }
      if (! is_end_() && ! other.is_end_()) {
        throw xml_compiler_logic_error("extracted_ptree_iterator::equal supports only it == end().");
      }

      // ----------------------------------------
      if (is_end_()) {
        other.equal(*this);
      }

      // ----------------------------------------
      if (ended_()) {
        return true;
      }

      return false;
    }

    void set_node_(void) {
      if (node_) {
        delete node_;
      }
      node_ = nullptr;

      if (ended_()) {
        return;
      }

      const auto& top = extracted_ptree_.stack_.top();
      if (top.it == top.end) {
        return;
      }

      node_ = new node(*(top.it), extracted_ptree_, top.parent_replacement);
    }

    void extract_include_(void) const {
      if (ended_()) return;

      const auto& xml_compiler = extracted_ptree_.xml_compiler_;

      auto& top = extracted_ptree_.stack_.top();
      const auto& it = *(top.it);

      if (it.first != "include") return;

      // ----------------------------------------
      // replacement
      std::tr1::shared_ptr<pqrs::string::replacement> replacement_ptr(new pqrs::string::replacement);
      if (! it.second.empty()) {
        replacement_loader loader(xml_compiler, *replacement_ptr);
        loader.traverse(extracted_ptree(extracted_ptree_, top.parent_replacement, it.second));
      }

      auto end = replacement_ptr->end();
      for (const auto& i : top.parent_replacement) {
        if (replacement_ptr->find(i.first) == end) {
          (*replacement_ptr)[i.first] = i.second;
        }
      }

      // ----------------------------------------
      std::string xml_file_path;
      {
        auto path = it.second.get_optional<std::string>("<xmlattr>.path");
        if (path) {
          if (extracted_ptree_.included_files_.empty()) {
            throw xml_compiler_logic_error("included_files_.empty() in extracted_ptree_iterator::extract_include_.");
          }
          xml_file_path = xml_compiler.make_file_path(pqrs::file_path::dirname(extracted_ptree_.included_files_.back()),
                                                      *path);
        }
      }

      if (! xml_file_path.empty()) {
        for (const auto& i : extracted_ptree_.included_files_) {
          if (i == xml_file_path) {
            xml_compiler.error_information_.set("An infinite include loop is detected:\n" + xml_file_path);
            return;
          }
        }

        ptree_ptr pt_ptr;
        xml_compiler.read_xml_(pt_ptr,
                               xml_file_path,
                               *replacement_ptr);

        if (pt_ptr) {
          // Skip <include> next time.
          ++(top.it);
          // Do not call collapse_ here.
          // (Keep included_files_ to detect an infinite include loop.)

          if (! pt_ptr->empty()) {
            auto root_node = pt_ptr->begin();
            const auto& root_children = root_node->second;
            if (! root_children.empty()) {
              extracted_ptree_.stack_.push(stack_data(pt_ptr, replacement_ptr, root_children));
              extracted_ptree_.included_files_.push_back(xml_file_path);
              extract_include_();
            }
          }

          collapse_();
        }
      }
    }

    void collapse_(void) const {
      for (;;) {
        if (ended_()) break;

        const auto& top = extracted_ptree_.stack_.top();
        if (top.it != top.end) {
          break;
        }

        if (top.extracted()) {
          if (extracted_ptree_.included_files_.empty()) {
            throw xml_compiler_logic_error("included_files_.empty() in extracted_ptree_iterator::collapse_.");
          }
          extracted_ptree_.included_files_.pop_back();
        }
        extracted_ptree_.stack_.pop();
      }
    }

    bool is_end_(void) const { return stack_size_ == 0; }
    bool ended_(void) const {
      return (stack_size_ == 0) ||
             (extracted_ptree_.stack_.size() < stack_size_);
    }

    const extracted_ptree& extracted_ptree_;
    const size_t stack_size_;
    node* node_;
  };

  extracted_ptree_iterator begin(void) const {
    return extracted_ptree_iterator(*this, replacement_, pt_);
  }

  extracted_ptree_iterator end(void) const {
    return extracted_ptree_iterator(*this);
  }

private:
  extracted_ptree(const extracted_ptree& extracted_ptree,
                  const pqrs::string::replacement& replacement,
                  const boost::property_tree::ptree& pt) :
    xml_compiler_(extracted_ptree.xml_compiler_),
    replacement_(replacement),
    pt_(pt),
    included_files_(extracted_ptree.included_files_),
    stack_(extracted_ptree.stack_)
  {}

  const xml_compiler& xml_compiler_;
  const pqrs::string::replacement& replacement_;
  const boost::property_tree::ptree& pt_;

  // shared_ptr for included_files_.
  std::tr1::shared_ptr<std::deque<std::string> > included_files_ptr_;
  std::deque<std::string>& included_files_;

  // shared_ptr for stack_.
  std::tr1::shared_ptr<std::stack<stack_data> > stack_ptr_;
  std::stack<stack_data>& stack_;
};
