#ifndef PQRS_XML_COMPILER_HPP
#define PQRS_XML_COMPILER_HPP

#include <cstdlib>
#include <memory>
#include <regex>
#include <stack>
#include <stdexcept>
#include <string>
#include <vector>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wshadow"
#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>
#include <boost/iterator_adaptors.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/optional.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/unordered_map.hpp>
#pragma clang diagnostic pop

#include "pqrs/file_path.hpp"
#include "pqrs/string.hpp"

namespace pqrs {
class xml_compiler final {
public:
  typedef std::shared_ptr<boost::property_tree::ptree> ptree_ptr;
#include "pqrs/xml_compiler/detail/exception.hpp"
#include "pqrs/xml_compiler/detail/error_information.hpp"
#include "pqrs/xml_compiler/detail/extracted_ptree.hpp"
#include "pqrs/xml_compiler/detail/replacement.hpp"
#include "pqrs/xml_compiler/detail/symbol_map.hpp"
#include "pqrs/xml_compiler/detail/app.hpp"
#include "pqrs/xml_compiler/detail/bundle_identifier_override.hpp"
#include "pqrs/xml_compiler/detail/window_name.hpp"
#include "pqrs/xml_compiler/detail/ui_element_role.hpp"
#include "pqrs/xml_compiler/detail/shell_command.hpp"
#include "pqrs/xml_compiler/detail/device.hpp"
#include "pqrs/xml_compiler/detail/preferences_node.hpp"
#include "pqrs/xml_compiler/detail/essential_configuration.hpp"
#include "pqrs/xml_compiler/detail/filter_vector.hpp"
#include "pqrs/xml_compiler/detail/remapclasses_initialize_vector.hpp"
#include "pqrs/xml_compiler/detail/remapclasses_initialize_vector_prepare_loader.hpp"
#include "pqrs/xml_compiler/detail/loader_wrapper.hpp"
#include "pqrs/xml_compiler/detail/modifier.hpp"
#include "pqrs/xml_compiler/detail/inputsource.hpp"
#include "pqrs/xml_compiler/detail/url.hpp"

  xml_compiler(const std::string& system_xml_directory, const std::string& private_xml_directory) : system_xml_directory_(system_xml_directory),
                                                                                                    private_xml_directory_(private_xml_directory) {}

  void reload(const std::string& checkbox_xml_file_name);
  void reload(void) { reload("checkbox.xml"); }

  const remapclasses_initialize_vector& get_remapclasses_initialize_vector(void) const {
    return remapclasses_initialize_vector_;
  }

  const error_information& get_error_information(void) const {
    return error_information_;
  }

  const symbol_map& get_symbol_map(void) const { return symbol_map_; }

  boost::optional<const std::string&> get_identifier(int config_index) const;
  boost::optional<int> get_config_index(const std::string& identifier) const;
  boost::optional<const std::string&> override_bundle_identifier(const std::string& bundle_identifier,
                                                                 const std::string& window_name,
                                                                 const std::string& ui_element_role) const;
  size_t get_app_vector_size(void) const { return app_vector_.size(); }
  size_t get_inputsource_vector_size(void) const { return inputsource_vector_.size(); }
  size_t get_window_name_vector_size(void) const { return window_name_vector_.size(); }
  bool is_app_matched(uint32_t& appid, size_t index, const std::string& application_identifier) const;
  bool is_inputsource_matched(uint32_t& inputsource,
                              size_t index,
                              const std::string& languagecode,
                              const std::string& inputsourceid,
                              const std::string& inputmodeid) const;
  bool is_window_name_matched(uint32_t& windownameid, size_t index, const std::string& window_name) const;
  bool is_vk_change_inputsource_matched(uint32_t keycode,
                                        const std::string& languagecode,
                                        const std::string& inputsourceid,
                                        const std::string& inputmodeid) const;
  boost::optional<const std::string&> get_url(int keycode) const;
  boost::optional<const std::string&> get_url_type(int keycode) const;
  bool get_url_background(int keycode) const;

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

  // ----------------------------------------
  bool debug_get_initialize_vector(std::vector<uint32_t>& out, const std::string& raw_identifier) const;

private:
  void append_environments_to_replacement_(pqrs::string::replacement& r) const;

  void read_xml_(ptree_ptr& out,
                 const std::string& file_path,
                 const pqrs::string::replacement& replacement) const;
  void read_xml_(ptree_ptr& out,
                 const std::string& file_path) const {
    read_xml_(out, file_path, replacement_);
  }

  std::string make_file_path(const std::string& base_directory, const std::string& path) const {
    std::string p;
    if (!boost::starts_with(path, "/")) {
      p = base_directory + "/" + path;
    } else {
      p = path;
    }
    pqrs::file_path::normalize(p);
    return p;
  }

  extracted_ptree make_extracted_ptree(const boost::property_tree::ptree& pt,
                                       const std::string& xml_file_path,
                                       const pqrs::string::replacement& replacement) const {
    return extracted_ptree(*this, replacement, pt, xml_file_path);
  }

  extracted_ptree make_extracted_ptree(const boost::property_tree::ptree& pt,
                                       const std::string& xml_file_path) const {
    return make_extracted_ptree(pt, xml_file_path, replacement_);
  }

  static void normalize_identifier_(std::string& identifier) {
    pqrs::string::remove_whitespaces(identifier);
    boost::replace_all(identifier, ".", "_");
  }

  bool valid_identifier_(const std::string& identifier, const std::string& parent_tag_name) const;

  const std::string system_xml_directory_;
  const std::string private_xml_directory_;

  mutable error_information error_information_;
  mutable std::string replacement_warnings_;

  mutable std::vector<std::string> global_included_files_;

  pqrs::string::replacement replacement_;
  symbol_map symbol_map_;
  boost::unordered_map<uint32_t, std::shared_ptr<modifier>> modifier_map_;
  std::vector<std::shared_ptr<app>> app_vector_;
  std::vector<std::shared_ptr<bundle_identifier_override>> bundle_identifier_override_vector_;
  std::vector<std::shared_ptr<window_name>> window_name_vector_;
  std::vector<std::shared_ptr<shell_command>> shell_command_vector_;
  boost::unordered_map<uint32_t, std::shared_ptr<inputsource>> vk_change_inputsource_map_;
  std::vector<std::shared_ptr<inputsource>> inputsource_vector_;
  boost::unordered_map<uint32_t, std::shared_ptr<url>> vk_open_url_map_;
  boost::unordered_map<uint32_t, std::string> identifier_map_;
  std::vector<std::shared_ptr<essential_configuration>> essential_configurations_;
  remapclasses_initialize_vector remapclasses_initialize_vector_;

  preferences_node_tree<preferences_checkbox_node> preferences_checkbox_node_tree_;
  preferences_node_tree<preferences_number_node> preferences_number_node_tree_;
};
}

#endif
