#ifndef PQRS_XML_COMPILER_HPP
#define PQRS_XML_COMPILER_HPP

#include <stack>
#include <string>
#include <stdexcept>
#include <vector>
#include <tr1/memory>
#include <tr1/unordered_map>
#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>
#include <boost/iterator_adaptors.hpp>
#include <boost/optional.hpp>
#include <boost/property_tree/ptree.hpp>
#include "pqrs/string.hpp"

namespace pqrs {
  class xml_compiler {
  public:
    typedef std::tr1::shared_ptr<boost::property_tree::ptree> ptree_ptr;
#include "pqrs/xml_compiler/detail/exception.hpp"
#include "pqrs/xml_compiler/detail/error_information.hpp"
#include "pqrs/xml_compiler/detail/xml_file_path.hpp"
#include "pqrs/xml_compiler/detail/extracted_ptree.hpp"
#include "pqrs/xml_compiler/detail/replacement.hpp"
#include "pqrs/xml_compiler/detail/symbol_map.hpp"
#include "pqrs/xml_compiler/detail/app.hpp"
#include "pqrs/xml_compiler/detail/device.hpp"
#include "pqrs/xml_compiler/detail/preferences_node.hpp"
#include "pqrs/xml_compiler/detail/essential_configuration.hpp"
#include "pqrs/xml_compiler/detail/filter_vector.hpp"
#include "pqrs/xml_compiler/detail/remapclasses_initialize_vector.hpp"
#include "pqrs/xml_compiler/detail/remapclasses_initialize_vector_prepare_loader.hpp"
#include "pqrs/xml_compiler/detail/loader_wrapper.hpp"

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

    boost::optional<const essential_configuration&> get_essential_configuration(size_t index) const {
      if (index >= essential_configurations_.size()) return boost::none;
      return *(essential_configurations_[index]);
    }

    const preferences_node_tree<preferences_checkbox_node>& get_preferences_checkbox_node_tree(void) const {
      return preferences_checkbox_node_tree_;
    }
    const preferences_node_tree<preferences_number_node>& get_preferences_number_node_tree(void) const {
      return preferences_number_node_tree_;
    }

  private:
    void read_xml_(ptree_ptr& out,
                   const std::string& base_diretory,
                   const std::string& relative_file_path,
                   const pqrs::string::replacement& replacement) const;
    void read_xml_(ptree_ptr& out,
                   const xml_file_path& xml_file_path,
                   const pqrs::string::replacement& replacement) const;
    void read_xml_(ptree_ptr& out,
                   const xml_file_path& xml_file_path) const {
      read_xml_(out, xml_file_path, replacement_);
    }

    void extract_include_(ptree_ptr& out,
                          const boost::property_tree::ptree::value_type& it) const;

    static void normalize_identifier_(std::string& identifier);
    bool valid_identifier_(const std::string& identifier, const std::string& parent_tag_name) const;

    void traverse_identifier_(const extracted_ptree& pt,
                              const std::string& parent_tag_name);
    void traverse_identifier_(const boost::property_tree::ptree& pt,
                              const std::string& parent_tag_name) {
      traverse_identifier_(extracted_ptree(*this, pt), parent_tag_name);
    }

    void traverse_autogen_(const extracted_ptree& pt,
                           const std::string& identifier,
                           const filter_vector& filter_vector,
                           std::vector<uint32_t>& initialize_vector);
    void traverse_autogen_(const boost::property_tree::ptree& pt,
                           const std::string& identifier,
                           const filter_vector& filter_vector,
                           std::vector<uint32_t>& initialize_vector) {
      traverse_autogen_(extracted_ptree(*this, pt), identifier, filter_vector, initialize_vector);
    }

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
    std::vector<std::tr1::shared_ptr<app> > app_vector_;
    std::tr1::unordered_map<uint32_t, std::string> identifier_map_;
    std::vector<std::tr1::shared_ptr<essential_configuration> > essential_configurations_;
    remapclasses_initialize_vector remapclasses_initialize_vector_;
    uint32_t simultaneous_keycode_index_;

    preferences_node_tree<preferences_checkbox_node> preferences_checkbox_node_tree_;
    preferences_node_tree<preferences_number_node> preferences_number_node_tree_;
  };
}

#endif
