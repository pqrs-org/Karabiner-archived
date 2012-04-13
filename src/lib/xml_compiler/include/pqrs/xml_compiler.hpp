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
#include "pqrs/xml_compiler/detail/error_information.hpp"
#include "pqrs/xml_compiler/detail/replacement.hpp"
#include "pqrs/xml_compiler/detail/symbol_map.hpp"
#include "pqrs/xml_compiler/detail/app.hpp"
#include "pqrs/xml_compiler/detail/device.hpp"
#include "pqrs/xml_compiler/detail/preferences_node.hpp"
#include "pqrs/xml_compiler/detail/filter_vector.hpp"

    class remapclasses_initialize_vector;

    xml_compiler(const std::string& system_xml_directory, const std::string& private_xml_directory) :
      system_xml_directory_(system_xml_directory),
      private_xml_directory_(private_xml_directory)
    {}

    void reload(void);

    const remapclasses_initialize_vector& get_remapclasses_initialize_vector(void) const {
      return remapclasses_initialize_vector_;
    }

    const error_information& get_error_information(void) const {
      return error_information_;
    }

    boost::optional<uint32_t> get_symbol_map_value(const std::string& name) const {
      return symbol_map_.get_optional(name);
    }
    void dump_symbol_map(void) const {
      symbol_map_.dump();
    }

    boost::optional<const std::string&> get_identifier(int config_index) const;
    uint32_t get_appid(const std::string& application_identifier) const;

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
    class remapclasses_initialize_vector {
    public:
      remapclasses_initialize_vector(void);
      void clear(void);
      const std::vector<uint32_t>& get(void) const;
      uint32_t get_config_count(void) const;
      void add(const std::vector<uint32_t>& v, uint32_t config_index, const std::string& identifier);
      void freeze(void);

    private:
      enum {
        INDEX_OF_FORMAT_VERSION = 0,
        INDEX_OF_CONFIG_COUNT = 1,
      };

      std::vector<uint32_t> data_;
      std::tr1::unordered_map<uint32_t, bool> is_config_index_added_;
      uint32_t max_config_index_;
      bool freezed_;
    };

    // ============================================================
    const preferences_node_tree<preferences_checkbox_node>& get_preferences_checkbox_node_tree(void) const {
      return preferences_checkbox_node_tree_;
    }
    const preferences_node_tree<preferences_number_node>& get_preferences_number_node_tree(void) const {
      return preferences_number_node_tree_;
    }

  private:
    typedef std::tr1::shared_ptr<boost::property_tree::ptree> ptree_ptr;
    void read_xml_(ptree_ptr& out,
                   const std::string& base_diretory,
                   const std::string& relative_file_path,
                   const pqrs::string::replacement& replacement) const;
    void read_xmls_(std::vector<ptree_ptr>& pt_ptrs,
                    const std::vector<xml_file_path_ptr>& xml_file_path_ptrs) const;

    void extract_include_(ptree_ptr& out,
                          const boost::property_tree::ptree::value_type& it) const;

    void reload_autogen_(void);
    bool valid_identifier_(const std::string& identifier, const std::string& parent_tag_name);
    void add_config_index_and_keycode_to_symbol_map_(const boost::property_tree::ptree& pt,
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
                                  std::vector<uint32_t>& initialize_vector) const;

    const std::string system_xml_directory_;
    const std::string private_xml_directory_;

    mutable error_information error_information_;

    pqrs::string::replacement replacement_;
    symbol_map symbol_map_;
    std::tr1::unordered_map<uint32_t, std::string> identifier_map_;
    remapclasses_initialize_vector remapclasses_initialize_vector_;
    uint32_t simultaneous_keycode_index_;

    std::vector<std::tr1::shared_ptr<app> > app_vector_;

    preferences_node_tree<preferences_checkbox_node> preferences_checkbox_node_tree_;
    preferences_node_tree<preferences_number_node> preferences_number_node_tree_;
  };
}

#endif
