// This header intentionally has no include guards.

class extracted_ptree
{
public:
  extracted_ptree(const xml_compiler& xml_compiler,
                  const boost::property_tree::ptree& pt) :
    xml_compiler_(xml_compiler),
    pt_(pt)
  {}

  class extracted_ptree_iterator : public boost::iterator_facade<extracted_ptree_iterator,
                                                                 const boost::property_tree::ptree::value_type,
                                                                 boost::forward_traversal_tag>
  {
  public:
    extracted_ptree_iterator(const xml_compiler& xml_compiler) :
      xml_compiler_(xml_compiler)
    {}

    extracted_ptree_iterator(const xml_compiler& xml_compiler,
                             const boost::property_tree::ptree& pt) :
      xml_compiler_(xml_compiler)
    {
      if (! pt.empty()) {
        stack_.push(stack_data(pt));
        extract_include_();
      }
    }

  private:
    friend class boost::iterator_core_access;

    void increment(void);
    const boost::property_tree::ptree::value_type& dereference(void) const;
    bool equal(const extracted_ptree_iterator const& other) const;

    void extract_include_(void);
    void collapse_(void);

    class stack_data {
    public:
      stack_data(const boost::property_tree::ptree& pt) :
        it(pt.begin()),
        end(pt.end())
      {}

      // For extracted ptree.
      stack_data(const ptree_ptr& p, const boost::property_tree::ptree& root_children) :
        it(root_children.begin()),
        end(root_children.end()),
        pt_ptr_(p)
      {}

      boost::property_tree::ptree::const_iterator it;
      boost::property_tree::ptree::const_iterator end;

    private:
      ptree_ptr pt_ptr_; // Keep extracted ptree_ptr until we finish traversing.
    };

    const xml_compiler& xml_compiler_;
    std::stack<stack_data> stack_;
  };

  extracted_ptree_iterator begin(void) const {
    return extracted_ptree_iterator(xml_compiler_, pt_);
  }

  extracted_ptree_iterator end(void) const {
    return extracted_ptree_iterator(xml_compiler_);
  }

private:
  const xml_compiler& xml_compiler_;
  const boost::property_tree::ptree& pt_;
};
