// This header intentionally has no include guards.

class extracted_ptree
{
public:
  extracted_ptree(const xml_compiler& xml_compiler,
                  const pqrs::string::replacement& replacement,
                  const boost::property_tree::ptree& pt) :
    xml_compiler_(xml_compiler),
    replacement_(replacement),
    pt_(pt)
  {}

  class node
  {
  public:
    node(const boost::property_tree::ptree::value_type& node,
         const xml_compiler& xml_compiler,
         const pqrs::string::replacement& replacement) :
      node_(node),
      xml_compiler_(xml_compiler),
      replacement_(replacement)
    {}

    const std::string& get_tag_name(void) const { return node_.first; }
    const std::string& get_data(void) const { return node_.second.data(); }
    boost::optional<std::string> get_optional(const std::string& path) const {
      return node_.second.get_optional<std::string>(path);
    }

    bool children_empty(void) const { return node_.second.empty(); }
    extracted_ptree children_extracted_ptree(void) const {
      return extracted_ptree(xml_compiler_, replacement_, node_.second);
    }

  private:
    const boost::property_tree::ptree::value_type& node_;
    const xml_compiler& xml_compiler_;
    const pqrs::string::replacement& replacement_;
  };

  class extracted_ptree_iterator : public boost::iterator_facade<extracted_ptree_iterator,
                                                                 const node,
                                                                 boost::forward_traversal_tag>
  {
  public:
    extracted_ptree_iterator(const xml_compiler& xml_compiler) :
      xml_compiler_(xml_compiler)
    {}

    extracted_ptree_iterator(const xml_compiler& xml_compiler,
                             const pqrs::string::replacement& replacement,
                             const boost::property_tree::ptree& pt) :
      xml_compiler_(xml_compiler)
    {
      if (! pt.empty()) {
        stack_.push(stack_data(pt, replacement));
        extract_include_();
      }
    }

  private:
    friend class boost::iterator_core_access;

    void increment(void);
    const node dereference(void) const;
    bool equal(const extracted_ptree_iterator const& other) const;

    void extract_include_(void);
    void collapse_(void);

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

    private:
      // Keep extracted ptree_ptr, replacement_ptr until we finish traversing.
      const ptree_ptr pt_ptr_;
      const std::tr1::shared_ptr<pqrs::string::replacement> replacement_ptr_;
    };

    const xml_compiler& xml_compiler_;
    std::stack<stack_data> stack_;
  };

  extracted_ptree_iterator begin(void) const {
    return extracted_ptree_iterator(xml_compiler_, replacement_, pt_);
  }

  extracted_ptree_iterator end(void) const {
    return extracted_ptree_iterator(xml_compiler_);
  }

private:
  const xml_compiler& xml_compiler_;
  const pqrs::string::replacement& replacement_;
  const boost::property_tree::ptree& pt_;
};
