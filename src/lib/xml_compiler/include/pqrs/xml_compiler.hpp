#ifndef PQRS_XML_COMPILER_HPP
#define PQRS_XML_COMPILER_HPP

#include <string>
#include <stdexcept>
#include <vector>
#include <tr1/memory>
#include <tr1/unordered_map>
#include <boost/format.hpp>
#include <boost/optional.hpp>
#include <boost/property_tree/ptree.hpp>
#include "pqrs/string.hpp"

namespace pqrs {
  class xml_compiler {
  public:
    class remapclasses_initialize_vector;

    xml_compiler(const std::string& system_xml_directory, const std::string& private_xml_directory);

    void reload(void);
    const remapclasses_initialize_vector& get_remapclasses_initialize_vector(void) const;

    const std::string& get_error_message(void) const;
    int get_error_count(void) const;

    boost::optional<uint32_t> get_symbol_map_value(const std::string& name) const;
    void dump_symbol_map(void) const;

    static void normalize_identifier(std::string& identifier);

    // ============================================================
    class xml_compiler_runtime_error : public std::runtime_error {
    public:
      xml_compiler_runtime_error(const std::string& what) : std::runtime_error(what) {}
      xml_compiler_runtime_error(const boost::format& what) : std::runtime_error(what.str()) {}
    };
    class xml_compiler_logic_error : public std::logic_error {
    public:
      xml_compiler_logic_error(const std::string& what) : std::logic_error(what) {}
      xml_compiler_logic_error(const boost::format& what) : std::logic_error(what.str()) {}
    };

    // ============================================================
    class xml_file_path {
    public:
      class base_directory {
      public:
        enum type {
          system_xml,
          private_xml,
        };
      };

      xml_file_path(base_directory::type base_directory_type, const std::string& relative_path) :
        base_directory_type_(base_directory_type),
        relative_path_(relative_path)
      {}

      base_directory::type get_base_directory(void) const { return base_directory_type_; }
      const std::string& get_relative_path(void) const { return relative_path_; }

    private:
      base_directory::type base_directory_type_;
      const std::string relative_path_;
    };
    typedef std::tr1::shared_ptr<xml_file_path> xml_file_path_ptr;

    // ============================================================
    class symbol_map {
    public:
      symbol_map(void);
      void clear(void);
      void dump(void) const; // For debug

      uint32_t get(const std::string& name) const;
      uint32_t get(const std::string& type, const std::string& name) const;

      boost::optional<uint32_t> get_optional(const std::string& name) const;
      boost::optional<uint32_t> get_optional(const std::string& type, const std::string& name) const;

      // Call add("KeyCode", "RETURN", 36) to register "KeyCode::RETURN = 36".
      void add(const std::string& type, const std::string& name, uint32_t value);
      void add(const std::string& type, const std::string& name);

    private:
      std::tr1::unordered_map<std::string, uint32_t> symbol_map_;
    };

    // ============================================================
    class appdef {
    public:
      const boost::optional<std::string>& get_name(void) const { return name_; }
      void set_name(const std::string& v) { name_ = v; }
      void add_rule_equal(const std::string& v);
      void add_rule_prefix(const std::string& v);
      bool is_rules_matched(const std::string& identifier);

    private:
      boost::optional<std::string> name_;
      std::vector<std::string> rules_equal_;
      std::vector<std::string> rules_prefix_;
    };

    // ============================================================
    class remapclasses_initialize_vector {
    public:
      remapclasses_initialize_vector(void);
      void clear(void);
      const std::vector<uint32_t>& get(void) const;
      void add(const std::vector<uint32_t>& v, uint32_t configindex, const std::string& identifier);
      void freeze(void);

    private:
      enum {
        INDEX_OF_FORMAT_VERSION = 0,
        INDEX_OF_COUNT = 1,
      };

      std::vector<uint32_t> data_;
      std::tr1::unordered_map<uint32_t, bool> is_configindex_added_;
      uint32_t max_configindex_;
      bool freezed_;
    };

    // ============================================================
    class filter_vector {
    public:
      filter_vector(void);
      filter_vector(const symbol_map& symbol_map, const boost::property_tree::ptree& pt);
      std::vector<uint32_t>& get(void);
      const std::vector<uint32_t>& get(void) const;
      bool empty(void) const;

    private:
      void add(const symbol_map& symbol_map, uint32_t filter_type, const std::string& type, const std::string& string);

      std::vector<uint32_t> data_;
    };

    // ============================================================
    class preferences_node {
    public:
      preferences_node(void);
      virtual ~preferences_node(void) {}

      bool handle_name_and_appendix(const boost::property_tree::ptree::value_type& it);

      const std::string& get_name(void) const { return name_; }
      int get_name_line_count(void) const { return name_line_count_; }
      const std::string& get_identifier(void) const { return identifier_; }

    protected:
      std::string name_;
      int name_line_count_;

      std::string identifier_;
    };

    class preferences_checkbox_node : public preferences_node {
    public:
      preferences_checkbox_node(void);
      preferences_checkbox_node(const preferences_checkbox_node& parent_node);

      bool handle_item_child(const boost::property_tree::ptree::value_type& it);

      const std::string& get_name_for_filter(void) const { return name_for_filter_; }

    private:
      std::string name_for_filter_;
    };

    class preferences_number_node : public preferences_node {
    public:
      preferences_number_node(void);
      preferences_number_node(const preferences_number_node& parent_node);

      bool handle_item_child(const boost::property_tree::ptree::value_type& it);

      int get_default_value(void) const { return default_value_; }
      int get_step(void) const { return step_; }
      const std::string get_base_unit(void) const { return base_unit_; }

    private:
      int default_value_;
      int step_;
      std::string base_unit_;
    };

    template <class T>
    class preferences_node_tree {
    public:
      typedef std::tr1::shared_ptr<preferences_node_tree> preferences_node_tree_ptr;
      typedef std::vector<preferences_node_tree_ptr> preferences_node_tree_ptrs;
      typedef std::tr1::shared_ptr<preferences_node_tree_ptrs> preferences_node_tree_ptrs_ptr;

      preferences_node_tree(void) {}
      preferences_node_tree(const T& parent_node) : node_(parent_node) {}

      void clear(void);
      void traverse_item(const boost::property_tree::ptree& pt);

    private:
      T node_;

      // We use shared_ptr<vector> to store children node by these reason.
      // * sizeof(shared_ptr) < sizeof(vector).
      // * children_ is mostly empty.
      preferences_node_tree_ptrs_ptr children_;
    };

  private:
    typedef std::tr1::shared_ptr<boost::property_tree::ptree> ptree_ptr;
    void read_xmls_(std::vector<ptree_ptr>& pt_ptrs, const std::vector<xml_file_path_ptr>& xml_file_path_ptrs);

    void set_error_message_(const std::string& message);
    void set_error_message_(const boost::format& message);

    void reload_replacementdef_(void);
    void traverse_replacementdef_(const boost::property_tree::ptree& pt);

    void reload_symbol_map_(void);
    void traverse_symbol_map_(const boost::property_tree::ptree& pt);

    void reload_appdef_(void);
    void traverse_appdef_(const boost::property_tree::ptree& pt);

    void reload_devicedef_(void);
    void traverse_devicedef_(const boost::property_tree::ptree& pt);

    void reload_autogen_(void);
    bool valid_identifier_(const std::string identifier, const std::string parent_tag_name);
    void add_configindex_and_keycode_to_symbol_map_(const boost::property_tree::ptree& pt,
                                                    const std::string& parent_tag_name,
                                                    bool handle_notsave);
    void traverse_identifier_(const boost::property_tree::ptree& pt,
                              const std::string& parent_tag_name);
    void traverse_autogen_(const boost::property_tree::ptree& pt,
                           const std::string& identifier,
                           const filter_vector& filter_vector,
                           std::vector<uint32_t>& initialize_vector);
    void handle_autogen(const std::string& autogen,
                        const std::string& raw_autogen,
                        const filter_vector& filter_vector,
                        std::vector<uint32_t>& initialize_vector);
    void add_to_initialize_vector(const std::string& params,
                                  uint32_t type,
                                  const filter_vector& filter_vector,
                                  std::vector<uint32_t>& initialize_vector);

    void reload_preferences_(void);

    const std::string system_xml_directory_;
    const std::string private_xml_directory_;

    std::string error_message_;
    int error_count_;

    symbol_map symbol_map_;
    pqrs::string::replacement replacement_;
    std::tr1::unordered_map<uint32_t, std::string> confignamemap_;
    remapclasses_initialize_vector remapclasses_initialize_vector_;
    uint32_t simultaneous_keycode_index_;

    std::vector<std::tr1::shared_ptr<appdef> > app_;

    preferences_node_tree<preferences_checkbox_node> preferences_checkbox_node_tree_;
    preferences_node_tree<preferences_number_node> preferences_number_node_tree_;
  };
}

#endif
