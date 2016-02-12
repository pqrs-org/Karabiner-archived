#include "pqrs/xml_compiler.hpp"
#include "pqrs/xml_compiler_bindings_clang.h"

// ------------------------------------------------------------
int pqrs_xml_compiler_initialize(pqrs_xml_compiler** out,
                                 const char* system_xml_directory,
                                 const char* private_xml_directory) {
  if (!out) return -1;
  // return if already initialized.
  if (*out) return -1;

  *out = reinterpret_cast<pqrs_xml_compiler*>(new pqrs::xml_compiler(system_xml_directory, private_xml_directory));

  return 0;
}

void pqrs_xml_compiler_terminate(pqrs_xml_compiler** out) {
  if (out && *out) {
    delete reinterpret_cast<pqrs::xml_compiler*>(*out);
    *out = nullptr;
  }
}

// ------------------------------------------------------------
void pqrs_xml_compiler_reload(pqrs_xml_compiler* p, const char* checkbox_xml_file_name) {
  pqrs::xml_compiler* xml_compiler = reinterpret_cast<pqrs::xml_compiler*>(p);
  if (!xml_compiler) return;

  if (!checkbox_xml_file_name) return;

  xml_compiler->reload(checkbox_xml_file_name);
}

// ------------------------------------------------------------
const char*
pqrs_xml_compiler_get_error_message(const pqrs_xml_compiler* p) {
  const pqrs::xml_compiler* xml_compiler = reinterpret_cast<const pqrs::xml_compiler*>(p);
  if (!xml_compiler) return nullptr;

  return (xml_compiler->get_error_information()).get_message().c_str();
}

size_t
pqrs_xml_compiler_get_error_count(const pqrs_xml_compiler* p) {
  const pqrs::xml_compiler* xml_compiler = reinterpret_cast<const pqrs::xml_compiler*>(p);
  if (!xml_compiler) return 0;

  return (xml_compiler->get_error_information()).get_count();
}

// ------------------------------------------------------------
uint32_t
pqrs_xml_compiler_get_symbol_map_value(const pqrs_xml_compiler* p, const char* name) {
  const pqrs::xml_compiler* xml_compiler = reinterpret_cast<const pqrs::xml_compiler*>(p);
  if (!xml_compiler) return 0;

  auto v = (xml_compiler->get_symbol_map()).get_optional(name);
  if (!v) return 0;

  return *v;
}

const char*
pqrs_xml_compiler_get_symbol_map_name(const pqrs_xml_compiler* p, const char* type, uint32_t value) {
  const pqrs::xml_compiler* xml_compiler = reinterpret_cast<const pqrs::xml_compiler*>(p);
  if (!xml_compiler) return 0;

  auto v = (xml_compiler->get_symbol_map()).get_name(type, value);
  if (!v) return nullptr;

  return v->c_str();
}

const char*
pqrs_xml_compiler_get_identifier(const pqrs_xml_compiler* p, int config_index) {
  const pqrs::xml_compiler* xml_compiler = reinterpret_cast<const pqrs::xml_compiler*>(p);
  if (!xml_compiler) return nullptr;

  auto v = xml_compiler->get_identifier(config_index);
  if (!v) return nullptr;

  return v->c_str();
}

int pqrs_xml_compiler_get_config_index(const pqrs_xml_compiler* p, const char* identifier) {
  const pqrs::xml_compiler* xml_compiler = reinterpret_cast<const pqrs::xml_compiler*>(p);
  if (!xml_compiler) return -1;

  auto v = xml_compiler->get_config_index(identifier);
  if (!v) return -1;

  return *v;
}

size_t pqrs_xml_compiler_get_app_vector_size(const pqrs_xml_compiler* p) {
  const pqrs::xml_compiler* xml_compiler = reinterpret_cast<const pqrs::xml_compiler*>(p);
  if (!xml_compiler) return 0;

  return xml_compiler->get_app_vector_size();
}

size_t pqrs_xml_compiler_get_inputsource_vector_size(const pqrs_xml_compiler* p) {
  const pqrs::xml_compiler* xml_compiler = reinterpret_cast<const pqrs::xml_compiler*>(p);
  if (!xml_compiler) return 0;

  return xml_compiler->get_inputsource_vector_size();
}

size_t pqrs_xml_compiler_get_window_name_vector_size(const pqrs_xml_compiler* p) {
  const pqrs::xml_compiler* xml_compiler = reinterpret_cast<const pqrs::xml_compiler*>(p);
  if (!xml_compiler) return 0;

  return xml_compiler->get_window_name_vector_size();
}

bool pqrs_xml_compiler_is_app_matched(const pqrs_xml_compiler* p, uint32_t* appid, size_t index, const char* application_identifier) {
  const pqrs::xml_compiler* xml_compiler = reinterpret_cast<const pqrs::xml_compiler*>(p);
  if (!xml_compiler) return false;

  if (!appid) return false;

  return xml_compiler->is_app_matched(*appid, index, application_identifier != nullptr ? application_identifier : "");
}

bool pqrs_xml_compiler_is_inputsource_matched(const pqrs_xml_compiler* p,
                                              uint32_t* inputsource,
                                              size_t index,
                                              const char* languagecode,
                                              const char* inputsourceid,
                                              const char* inputmodeid) {
  const pqrs::xml_compiler* xml_compiler = reinterpret_cast<const pqrs::xml_compiler*>(p);
  if (!xml_compiler) return false;

  if (!inputsource) return false;

  return xml_compiler->is_inputsource_matched(*inputsource,
                                              index,
                                              languagecode != nullptr ? languagecode : "",
                                              inputsourceid != nullptr ? inputsourceid : "",
                                              inputmodeid != nullptr ? inputmodeid : "");
}

bool pqrs_xml_compiler_is_window_name_matched(const pqrs_xml_compiler* p, uint32_t* windownameid, size_t index, const char* window_name) {
  const pqrs::xml_compiler* xml_compiler = reinterpret_cast<const pqrs::xml_compiler*>(p);
  if (!xml_compiler) return false;

  if (!windownameid) return false;

  return xml_compiler->is_window_name_matched(*windownameid, index, window_name != nullptr ? window_name : "");
}

uint32_t
pqrs_xml_compiler_is_vk_change_inputsource_matched(const pqrs_xml_compiler* p,
                                                   uint32_t keycode,
                                                   const char* languagecode,
                                                   const char* inputsourceid,
                                                   const char* inputmodeid) {
  const pqrs::xml_compiler* xml_compiler = reinterpret_cast<const pqrs::xml_compiler*>(p);
  if (!xml_compiler) return 0;

  return xml_compiler->is_vk_change_inputsource_matched(keycode,
                                                        languagecode != nullptr ? languagecode : "",
                                                        inputsourceid != nullptr ? inputsourceid : "",
                                                        inputmodeid != nullptr ? inputmodeid : "");
}

const char*
pqrs_xml_compiler_get_url(const pqrs_xml_compiler* p, uint32_t keycode) {
  const pqrs::xml_compiler* xml_compiler = reinterpret_cast<const pqrs::xml_compiler*>(p);
  if (!xml_compiler) return nullptr;

  auto v = (xml_compiler->get_url(keycode));
  if (!v) return nullptr;

  return v->c_str();
}

const char*
pqrs_xml_compiler_get_url_type(const pqrs_xml_compiler* p, uint32_t keycode) {
  const pqrs::xml_compiler* xml_compiler = reinterpret_cast<const pqrs::xml_compiler*>(p);
  if (!xml_compiler) return nullptr;

  auto v = (xml_compiler->get_url_type(keycode));
  if (!v) return nullptr;

  return v->c_str();
}

bool pqrs_xml_compiler_get_url_background(const pqrs_xml_compiler* p, uint32_t keycode) {
  const pqrs::xml_compiler* xml_compiler = reinterpret_cast<const pqrs::xml_compiler*>(p);
  if (!xml_compiler) return 0;

  return xml_compiler->get_url_background(keycode);
}

// ------------------------------------------------------------
const uint32_t*
pqrs_xml_compiler_get_remapclasses_initialize_vector_data(const pqrs_xml_compiler* p) {
  const pqrs::xml_compiler* xml_compiler = reinterpret_cast<const pqrs::xml_compiler*>(p);
  if (!xml_compiler) return nullptr;

  auto v = (xml_compiler->get_remapclasses_initialize_vector()).get();
  return &(v[0]);
}

size_t
pqrs_xml_compiler_get_remapclasses_initialize_vector_size(const pqrs_xml_compiler* p) {
  const pqrs::xml_compiler* xml_compiler = reinterpret_cast<const pqrs::xml_compiler*>(p);
  if (!xml_compiler) return 0;

  auto v = (xml_compiler->get_remapclasses_initialize_vector()).get();
  return v.size();
}

uint32_t
pqrs_xml_compiler_get_remapclasses_initialize_vector_config_count(const pqrs_xml_compiler* p) {
  const pqrs::xml_compiler* xml_compiler = reinterpret_cast<const pqrs::xml_compiler*>(p);
  if (!xml_compiler) return 0;

  return (xml_compiler->get_remapclasses_initialize_vector()).get_config_count();
}

// ------------------------------------------------------------
namespace {
const pqrs::xml_compiler::preferences_node_tree<pqrs::xml_compiler::preferences_checkbox_node>* get_checkbox_node_tree_from_indexes(const pqrs_xml_compiler* p, size_t indexes[], size_t indexes_size) {
  auto xml_compiler = reinterpret_cast<const pqrs::xml_compiler*>(p);
  if (!xml_compiler) return nullptr;

  auto node_tree = &(xml_compiler->get_preferences_checkbox_node_tree());

  for (size_t i = 0; i < indexes_size; ++i) {
    if (!node_tree) return nullptr;

    auto children = node_tree->get_children();
    if (!children) return nullptr;
    if (indexes[i] >= children->size()) return nullptr;

    node_tree = ((*children)[indexes[i]]).get();
  }

  return node_tree;
}
}

size_t pqrs_xml_compiler_get_preferences_checkbox_node_tree_children_count(const pqrs_xml_compiler* p, size_t indexes[], size_t indexes_size) {
  auto node_tree = get_checkbox_node_tree_from_indexes(p, indexes, indexes_size);
  if (!node_tree) return 0;

  auto children = node_tree->get_children();
  if (!children) return 0;

  return children->size();
}

const char* pqrs_xml_compiler_get_preferences_checkbox_node_tree_name(const pqrs_xml_compiler* p, size_t indexes[], size_t indexes_size) {
  auto node_tree = get_checkbox_node_tree_from_indexes(p, indexes, indexes_size);
  if (!node_tree) return nullptr;

  return (node_tree->get_node()).get_name().c_str();
}

const char* pqrs_xml_compiler_get_preferences_checkbox_node_tree_style(const pqrs_xml_compiler* p, size_t indexes[], size_t indexes_size) {
  auto node_tree = get_checkbox_node_tree_from_indexes(p, indexes, indexes_size);
  if (!node_tree) return nullptr;

  return (node_tree->get_node()).get_style().c_str();
}

bool pqrs_xml_compiler_is_preferences_checkbox_node_tree_name_icontains(const pqrs_xml_compiler* p, size_t indexes[], size_t indexes_size, const char* string) {
  auto node_tree = get_checkbox_node_tree_from_indexes(p, indexes, indexes_size);
  if (!node_tree) return false;

  if (!string) return false;

  auto& name = (node_tree->get_node()).get_name();
  return boost::icontains(name, string);
}

const char* pqrs_xml_compiler_get_preferences_checkbox_node_tree_identifier(const pqrs_xml_compiler* p, size_t indexes[], size_t indexes_size) {
  auto node_tree = get_checkbox_node_tree_from_indexes(p, indexes, indexes_size);
  if (!node_tree) return nullptr;

  return (node_tree->get_node()).get_identifier().c_str();
}

// ------------------------------------------------------------
namespace {
const pqrs::xml_compiler::preferences_node_tree<pqrs::xml_compiler::preferences_number_node>* get_number_node_tree_from_indexes(const pqrs_xml_compiler* p, size_t indexes[], size_t indexes_size) {
  auto xml_compiler = reinterpret_cast<const pqrs::xml_compiler*>(p);
  if (!xml_compiler) return nullptr;

  auto node_tree = &(xml_compiler->get_preferences_number_node_tree());

  for (size_t i = 0; i < indexes_size; ++i) {
    if (!node_tree) return nullptr;

    auto children = node_tree->get_children();
    if (!children) return nullptr;
    if (indexes[i] >= children->size()) return nullptr;

    node_tree = ((*children)[indexes[i]]).get();
  }

  return node_tree;
}
}

size_t pqrs_xml_compiler_get_preferences_number_node_tree_children_count(const pqrs_xml_compiler* p, size_t indexes[], size_t indexes_size) {
  auto node_tree = get_number_node_tree_from_indexes(p, indexes, indexes_size);
  if (!node_tree) return 0;

  auto children = node_tree->get_children();
  if (!children) return 0;

  return children->size();
}

const char* pqrs_xml_compiler_get_preferences_number_node_tree_name(const pqrs_xml_compiler* p, size_t indexes[], size_t indexes_size) {
  auto node_tree = get_number_node_tree_from_indexes(p, indexes, indexes_size);
  if (!node_tree) return nullptr;

  return (node_tree->get_node()).get_name().c_str();
}

const char* pqrs_xml_compiler_get_preferences_number_node_tree_identifier(const pqrs_xml_compiler* p, size_t indexes[], size_t indexes_size) {
  auto node_tree = get_number_node_tree_from_indexes(p, indexes, indexes_size);
  if (!node_tree) return nullptr;

  return (node_tree->get_node()).get_identifier().c_str();
}

int pqrs_xml_compiler_get_preferences_number_node_tree_default_value(const pqrs_xml_compiler* p, size_t indexes[], size_t indexes_size) {
  auto node_tree = get_number_node_tree_from_indexes(p, indexes, indexes_size);
  if (!node_tree) return 0;

  return (node_tree->get_node()).get_default_value();
}

int pqrs_xml_compiler_get_preferences_number_node_tree_step(const pqrs_xml_compiler* p, size_t indexes[], size_t indexes_size) {
  auto node_tree = get_number_node_tree_from_indexes(p, indexes, indexes_size);
  if (!node_tree) return 1;

  return (node_tree->get_node()).get_step();
}

const char* pqrs_xml_compiler_get_preferences_number_node_tree_base_unit(const pqrs_xml_compiler* p, size_t indexes[], size_t indexes_size) {
  auto node_tree = get_number_node_tree_from_indexes(p, indexes, indexes_size);
  if (!node_tree) return nullptr;

  return (node_tree->get_node()).get_base_unit().c_str();
}
